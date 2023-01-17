#shader vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform float is_textured;
uniform sampler2D texture_diffuse1;

void main()
{
    if (is_textured == 1.0)
        FragColor = texture(texture_diffuse1, TexCoords);
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}