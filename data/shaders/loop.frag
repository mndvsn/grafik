#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
flat in float v_TexId;
in vec4 v_Color;

uniform sampler2D u_Textures[3];
uniform int u_TexId;
uniform vec4 u_Color;

void main()
{
    float texId2 = v_TexId;
    vec4 texColor = texture(u_Textures[u_TexId], v_TexCoord);
    color = v_Color * u_Color * texColor;
    if (color.a < 0.01) discard;
}