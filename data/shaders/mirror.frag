#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
flat in float v_TexId;
in vec3 v_Color;

uniform sampler2D u_Textures[2];
uniform float u_ColorAlpha;
uniform float u_ReflectDarken;

void main()
{
    int texId = int(v_TexId);
    vec4 texColor = texture(u_Textures[texId], v_TexCoord);
    vec4 mirror = vec4(u_ReflectDarken, u_ReflectDarken, u_ReflectDarken, 1.0f);
    color = mix(vec4(v_Color, 1.0f), texColor, u_ColorAlpha) * mirror;
//    if (u_ReflectDarken < 1)
//        color.a = 1-u_ReflectDarken;
}