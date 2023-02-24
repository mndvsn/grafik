/**
 * Grafik
 * Lab
 * Copyright 2012-2022 Martin Furuberg
 */
#pragma once
#include "components/Component.h"
#include "events/ApplicationEvent.h"

#include <glm/glm.hpp>


namespace labb
{
    struct Vertex
    {
        glm::vec3   Position    { 0.0f, 0.0f, 0.0f };
        glm::vec4   Color       { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec2   TexCoords   { 0.0f, 0.0f };
        float       TexId       { 0.0f };
    };

    class LLab : public Component
    {
    public:
        ~LLab() override;

        void OnAttach(int& eventMask) override;
        void OnDetach() override { }
        void OnEvent(Event& /*event*/) override;

        virtual void OnTick(TickEvent& e);
        virtual void OnRender(RenderEvent& e);
        virtual void OnUI(UIEvent& e);

    protected:
        void RenderError(const std::string_view& error);

    private:
        bool _bHasError { false };
        std::string _errorString { };
    };
}

