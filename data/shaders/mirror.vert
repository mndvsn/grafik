#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texId;

out vec2 v_TexCoord;
out vec3 v_Color;
flat out float v_TexId;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_TexId = texId;
    v_Color = color;
}