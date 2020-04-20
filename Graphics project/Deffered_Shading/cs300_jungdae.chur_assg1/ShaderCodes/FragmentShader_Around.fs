#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct PointLight
{
	vec3 lightPosition;
	vec3 lightColor;
	float diffuseStrength;
};

#define NR_POINT_LIGHTS 16
uniform PointLight pointLight[NR_POINT_LIGHTS];

uniform vec3 color;

void main()
{  
	vec3 norm = normalize(Normal);

    vec3 result = color;

    vec3 lightDir = normalize(pointLight[0].lightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

    vec3 mixUp = pointLight[0].lightColor;

	vec3 diffuse = pointLight[0].diffuseStrength * diff * mixUp;

    result += diffuse;

	// Send it to FragColor
	FragColor = vec4(result, 1.f);
}