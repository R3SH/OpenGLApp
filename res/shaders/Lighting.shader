#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform float texIndex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_Color = a_Color;
    gl_Position = projection * view * model * vec4(a_Position, 1.0);
	v_TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
	v_TexIndex = texIndex;
	v_Normal = mat3(transpose(inverse(model))) * a_Normal;
	v_FragPos = vec3(model * vec4(a_Position, 1.0));
}


#shader fragment
#version 450 core


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 viewPos;
uniform sampler2D u_Textures[32];

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform Material material;

void main()
{
	int index = int(v_TexIndex);
	if (index == 0)		//No texture
	{
		vec3 norm = normalize(v_Normal);
		vec3 lightDir = normalize(lightPos - v_FragPos);


		//ambient
		vec3 ambient = lightColor * material.ambient;
		
		//diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = lightColor * (diff * material.diffuse);

		//specular
		vec3 viewDir = normalize(viewPos - v_FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = lightColor * (spec * material.specular);


		vec3 result = ambient + diffuse + specular;
		o_Color = vec4(result, 1.0);
	}
	else
		o_Color = texture(u_Textures[index], v_TexCoord);
}