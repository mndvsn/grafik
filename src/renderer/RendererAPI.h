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

    virtual ~RendererAPI() = default;

    virtual void ResetState() const = 0;

    virtual void ClearBuffer() const = 0;

    virtual void SetClearColor(float r, float g, float b, float alpha = 1.0f) = 0;
    virtual void SetWireframeMode(bool bUseLineDraw) = 0;

    static std::unique_ptr<RendererAPI> Create(API api);
    static API GetAPI() { return _api; }
    static std::string_view GetAPIString();

private:
    inline static API _api { API::None };
};
