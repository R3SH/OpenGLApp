#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out float v_TexIndex;

uniform float texIndex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//v_TexIndex = a_TexIndex;
	//gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
	gl_Position = projection * view * model * vec4(a_Position, 1.0);
	v_TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
	v_TexIndex = texIndex;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	if (index == 0)		//No texture
		o_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		o_Color = texture(u_Textures[index], v_TexCoord);		// * v_Color
};