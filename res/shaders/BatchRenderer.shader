#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

//uniform mat4 u_ViewProj;
//uniform mat4 u_Transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	//gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
	gl_Position = projection * view * model * vec4(a_Position, 1.0);
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	if (index == -1)		//No texture
		o_Color = v_Color;
	else
		o_Color = texture(u_Textures[index], v_TexCoord) * v_Color;
};