/**
 * Grafik
 * Vulkan: Device
 * Copyright Martin Furuberg
 */
#include "gpch.h"
#include "VulkanDevice.h"


VulkanDevice::VulkanDevice(vk::Instance& instance, vk::SurfaceKHR& surface, std::vector<const char*>& validationLayers)
    : _instance { instance }
    , _surface { surface }
    , _validationLayers { validationLayers }
{
    SelectPhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPool();
}

void VulkanDevice::SelectPhysicalDevice()
{
    const auto devices = _instance.enumeratePhysicalDevices();
    if (devices.empty())
    {
        throw std::runtime_error("No GPUs with Vulkan support found!");
    }
    std::cout << "Devices found: \t"   << devices.size() << std::endl;

    for (const auto& device : devices)
    {
        if (CheckDevice(device))
        {
            _physicalDevice = device;
            break;
        }
    }

    if (!_physicalDevice)
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    const auto props = _physicalDevice.getProperties();
    std::cout << "Adapter: \t"    << props.deviceName << std::endl;
}

void VulkanDevice::CreateLogicalDevice()
{
    const QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);
    if (!indices.graphicsFamily || !indices.presentFamily)
    {
        throw std::runtime_error { "Physical device queue families error" };
    }
    
    const std::set uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos { };

    constexpr float queuePriority = 1.0f;
    for (const uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo
        {
            .queueFamilyIndex   = queueFamily,
            .queueCount         = 1,
            .pQueuePriorities   = &queuePriority
        };
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    auto deviceFeatures = vk::PhysicalDeviceFeatures { .samplerAnisotropy = true };

    auto createInfo = vk::DeviceCreateInfo
    {
        .queueCreateInfoCount       = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos          = queueCreateInfos.data(),
#ifdef _DEBUG
        .enabledLayerCount          = static_cast<uint32_t>(_validationLayers.size()),
        .ppEnabledLayerNames        = _validationLayers.data(),
#endif
        .enabledExtensionCount      = static_cast<uint32_t>(_deviceExtensions.size()),
        .ppEnabledExtensionNames    = _deviceExtensions.data(),
        .pEnabledFeatures           = &deviceFeatures
    };

    try
    {
        _device = _physicalDevice.createDevice(createInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(_device);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create logical vulkan device: " << error.what() << std::endl;
    }

    _graphicsQueue = _device.getQueue(indices.graphicsFamily.value(), 0);
    _presentQueue = _device.getQueue(indices.presentFamily.value(), 0);
}

void VulkanDevice::CreateCommandPool()
{
    const QueueFamilyIndices queueFamilies = FindQueueFamilies(_physicalDevice);
    if (!queueFamilies.graphicsFamily || !queueFamilies.presentFamily)
    {
        throw std::runtime_error { "Physical device queue families error" };
    }

    const vk::CommandPoolCreateInfo poolInfo
    {
        .flags              = vk::CommandPoolCreateFlagBits::eTransient
                            | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex   = queueFamilies.graphicsFamily.value()
    };

    try
    {
        _commandPool = _device.createCommandPool(poolInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan Command Pool: " << error.what() << std::endl;
    }
}

/* -------------------------- */

void VulkanDevice::CreateImage(const vk::ImageCreateInfo& imageInfo, vk::MemoryPropertyFlags properties,
    vk::Image& image, vk::DeviceMemory& memory) const
{
    try
    {
        image = _device.createImage(imageInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan image: " << error.what() << std::endl;
    }

    const auto requirements = _device.getImageMemoryRequirements(image);

    const vk::MemoryAllocateInfo allocInfo
    {
        .allocationSize = requirements.size,
        .memoryTypeIndex = FindMemoryType(requirements.memoryTypeBits, properties)
    };

    try
    {
        memory = _device.allocateMemory(allocInfo);
        _device.bindImageMemory(image, memory, 0);
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to allocate memory for vulkan image: " << err.what() << std::endl;
    }
}

vk::DescriptorPool VulkanDevice::CreateDescriptorPool(const std::vector<vk::DescriptorPoolSize>& poolSizes, unsigned maxSets) const
{
    const vk::DescriptorPoolCreateInfo poolInfo
    {
        .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
        .maxSets = maxSets * static_cast<uint32_t>(std::end(poolSizes) - std::begin(poolSizes)),
        .poolSizeCount = static_cast<uint32_t>(std::end(poolSizes) - std::begin(poolSizes)),
        .pPoolSizes = poolSizes.data(),
    };
    return _device.createDescriptorPool(poolInfo);
}

/* -------------------------- */

bool VulkanDevice::CheckDevice(const vk::PhysicalDevice& device) const
{
    const QueueFamilyIndices indices = FindQueueFamilies(device);

    const bool extensionSupport = CheckDeviceExtensionSupport(device);

    bool swapchainAdequate { false };
    if (extensionSupport)
    {
        const SwapChainSupportDetails swapChainSupport = CheckSwapChainSupport(device);
        swapchainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.IsComplete() && extensionSupport && swapchainAdequate;
}

QueueFamilyIndices VulkanDevice::FindQueueFamilies(const vk::PhysicalDevice& device) const
{
    QueueFamilyIndices indices { };

    const auto queueFamilies = device.getQueueFamilyProperties();
    for (int i = 0; const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = i;
        }
        
        if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, _surface))
        {
            indices.presentFamily = i;
        }

        if (indices.IsComplete()) break;
        i++;
    }
    return indices;
}

bool VulkanDevice::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const
{
    std::set<std::string> requiredExtensions { _deviceExtensions.begin(), _deviceExtensions.end() };
    for (const auto& ext : device.enumerateDeviceExtensionProperties())
    {
        requiredExtensions.erase(ext.extensionName);
    }
    return requiredExtensions.empty();
}

SwapChainSupportDetails VulkanDevice::CheckSwapChainSupport(const vk::PhysicalDevice& device) const
{
    SwapChainSupportDetails details
    {
        device.getSurfaceCapabilitiesKHR(_surface),
        device.getSurfaceFormatsKHR(_surface),
        device.getSurfacePresentModesKHR(_surface)
    };
    return details;
}

vk::Format VulkanDevice::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
    vk::FormatFeatureFlagBits features) const
{
    for (const vk::Format& format : candidates)
    {
        vk::FormatProperties props = _physicalDevice.getFormatProperties(format);

        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        
        if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }
    throw std::runtime_error("Failed to find supported vulkan format!");
}

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties) const
{
    const auto memoryProperties = _physicalDevice.getMemoryProperties();
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error { "Failed to find suitable memory type for vulkan image!" };
}

/* -------------------------- */

void VulkanDevice::CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
    vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) const
{
    vk::BufferCreateInfo bufferInfo
    {
        .size           = size,
        .usage          = usage,
        .sharingMode    = vk::SharingMode::eExclusive
    };

    try
    {
        buffer = _device.createBuffer(bufferInfo);
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to create vulkan buffer: " << err.what() << std::endl;
    }

    const auto requirements = _device.getBufferMemoryRequirements(buffer);

    const vk::MemoryAllocateInfo allocInfo
    {
        .allocationSize     = requirements.size,
        .memoryTypeIndex    = FindMemoryType(requirements.memoryTypeBits, properties)
    };

    try
    {
        bufferMemory = _device.allocateMemory(allocInfo);
        _device.bindBufferMemory(buffer, bufferMemory, 0);
    }
    catch (vk::SystemError& err)
    {
        std::cerr << "Failed to allocate memory for vulkan buffer: " << err.what() << std::endl;
    }
}

vk::CommandBuffer VulkanDevice::BeginSingleTimeCommands() const
{
    const vk::CommandBufferAllocateInfo allocInfo
    {
        .commandPool        = _commandPool,
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1
    };

    const auto commandBuffers = _device.allocateCommandBuffers(allocInfo);
    constexpr vk::CommandBufferBeginInfo beginInfo { .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    for (const auto& buffer : commandBuffers)
    {
        buffer.begin(beginInfo);
    }
    
    return commandBuffers.front();
}

void VulkanDevice::EndSingleTimeCommands(vk::CommandBuffer commandBuffer) const
{
    commandBuffer.end();

    const vk::SubmitInfo submitInfo
    {
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer
    };

    _graphicsQueue.submit(submitInfo);
    _graphicsQueue.waitIdle();

    _device.freeCommandBuffers(_commandPool, commandBuffer);
}

void VulkanDevice::CopyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size)
{
    const vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();
    const vk::BufferCopy copyRegion
    {
        .srcOffset  = 0,
        .dstOffset  = 0,
        .size       = size
    };
    commandBuffer.copyBuffer(sourceBuffer, destBuffer, 1, &copyRegion);
    EndSingleTimeCommands(commandBuffer);
}

void VulkanDevice::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height, uint32_t layerCount)
{
    const vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();
    const vk::BufferImageCopy region
    {
        .bufferOffset       = 0,
        .bufferRowLength    = 0,
        .bufferImageHeight  = 0,
        .imageSubresource   = {
            .aspectMask     = vk::ImageAspectFlagBits::eColor,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = layerCount 
        },
        .imageOffset        = { 0, 0, 0 },
        .imageExtent        = {
            .width          = width,
            .height         = height,
            .depth          = 1
        }
    };
    commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);
    EndSingleTimeCommands(commandBuffer);
}

/* -------------------------- */

VulkanDevice::~VulkanDevice()
{
    _device.destroyCommandPool(_commandPool);
    _device.destroy();
}
