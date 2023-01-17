#shader vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    v_Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    v_FragPos = vec3(model * vec4(aPos, 1.0));
}


#shader fragment
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform float is_textured;
uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    int texId = int(is_textured);

    if (texId == 1)
        FragColor = texture(texture_diffuse1, TexCoords);
    else
    {
        vec3 norm = normalize(v_Normal);
        vec3 lightDir = normalize(lightPos - v_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        vec3 result = (ambient + diffuse) * objectColor;
        FragColor = vec4(result, 1.0);
    }
}