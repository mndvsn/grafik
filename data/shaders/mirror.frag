#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Color;

uniform sampler2D u_Texture;
uniform float u_ColorAlpha;
uniform float u_ReflectDarken;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    vec4 mirror = vec4(u_ReflectDarken, u_ReflectDarken, u_ReflectDarken, 1.0f);
    color = mix(vec4(v_Color, 1.0f), texColor, u_ColorAlpha) * mirror;
}