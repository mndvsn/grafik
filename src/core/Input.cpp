﻿/**
 * Grafik
 * Input
 * Copyright 2023 Martin Furuberg
 */
#include "gpch.h"
#include "core/Input.h"

#include "core/Application.h"
#include "components/Window.h"

#include <GLFW/glfw3.h>


namespace Input
{
    bool IsButtonPressed(Mouse::Button button)
    {
        const auto window = Application::Get().GetWindow()->GetNativeWindow();
        const auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    bool IsKeyPressed(Keyboard::Key key)
    {
        const auto window = Application::Get().GetWindow()->GetNativeWindow();
        const auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }
}
