#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
	v_Color = a_Color;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_TexIndex = a_TexIndex;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
	int index = int(v_TexIndex);
	if(index == -1)		//No texture
		o_Color = v_Color;
	else
		o_Color = texture(u_Textures[index], v_TexCoord);
};