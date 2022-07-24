#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 texCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	v_Color = a_Color;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	//o_Color = texColor;
	o_Color = vec4(v_TexCoord, 0.0, 1.0);
	//o_Color = v_Color;
	//color = vec4(1.0f);
};