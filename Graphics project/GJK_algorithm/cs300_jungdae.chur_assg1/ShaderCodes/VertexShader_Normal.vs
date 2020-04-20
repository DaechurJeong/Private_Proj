#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 normal;
} vs_out;
out vec3 outColor;

uniform mat4 model;	
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	outColor = vec3(1.0, 0.0, 0.0);
}