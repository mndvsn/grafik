﻿#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 v_Color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_Color = color;
}