/**
 * Grafik
 * RendererAPI
 * Copyright 2023 Martin Furuberg 
 */
#pragma once


class RendererAPI
{
public:
    enum class API : char
    {
        None = 0,
        OpenGL = 1,
        Vulkan = 2,
    };
    inline static constexpr API APIs[] = { API::OpenGL, API::Vulkan };

    virtual ~RendererAPI() = default;

    virtual void BeginFrame() const = 0;
    virtual void EndFrame() const = 0;

    virtual void ClearBuffer() const = 0;

    virtual void SetClearColor(float r, float g, float b, float alpha = 1.0f) = 0;
    virtual void SetWireframeMode(bool bUseLineDraw) = 0;

    virtual void SetViewport(int width, int height) = 0;

    static std::unique_ptr<RendererAPI> Create(API api);
    static API GetAPI() { return _api; }
    static std::string_view GetAPIString(API api = _api);

private:
    inline static API _api { API::None };
};
