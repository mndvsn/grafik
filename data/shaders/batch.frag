#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
flat in float v_TexId;
in vec4 v_Color;

uniform sampler2D u_Textures[3];

void main()
{
    int texId = int(v_TexId);
    vec4 texColor = texture(u_Textures[texId], v_TexCoord);
    color = v_Color * texColor;
}