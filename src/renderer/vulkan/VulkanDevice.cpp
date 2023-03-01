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
        vk::DeviceQueueCreateInfo queueCreateInfo { };
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    auto deviceFeatures = vk::PhysicalDeviceFeatures { };
    deviceFeatures.samplerAnisotropy = true;

    auto createInfo = vk::DeviceCreateInfo { };
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.enabledLayerCount = 0;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = _deviceExtensions.data();
#ifdef _DEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
    createInfo.ppEnabledLayerNames = _validationLayers.data();
#endif

    try
    {
        _device = _physicalDevice.createDevice(createInfo);
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
    
    vk::CommandPoolCreateInfo poolInfo { };
    poolInfo.queueFamilyIndex = queueFamilies.graphicsFamily.value();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try
    {
        _commandPool = _device.createCommandPool(poolInfo);
    }
    catch (vk::SystemError& error)
    {
        std::cerr << "Failed to create Vulkan Command Pool: " << error.what() << std::endl;
    }
}

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

void VulkanDevice::Shutdown() const
{
    _device.destroyCommandPool(_commandPool);
    _device.destroy();
}

VulkanDevice::~VulkanDevice() = default;
