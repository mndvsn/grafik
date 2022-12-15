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
    vec2 UVs = v_TexCoord;
    if (gl_FrontFacing)
        UVs =  vec2(1 - v_TexCoord.x, v_TexCoord.y);
    vec4 texColor = texture(u_Textures[u_TexId], UVs);
    color = v_Color * u_Color * texColor;
}