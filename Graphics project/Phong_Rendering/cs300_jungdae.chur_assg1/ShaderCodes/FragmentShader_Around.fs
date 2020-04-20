#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct PointLight
{
	vec3 lightPosition;

	float constant;
	float linear;
	float quadratic;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};

#define NR_POINT_LIGHTS 16
uniform int num_obj;
uniform PointLight pointLight[NR_POINT_LIGHTS];

uniform vec3 lightColor;
uniform int type;
uniform vec3 viewPos;
uniform vec3 color;

void main()
{  
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = color;

    vec3 lightDir = normalize(pointLight[0].lightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

    vec3 mixUp = lightColor;

	vec3 diffuse = pointLight[0].diffuseStrength * diff * mixUp;

    result += diffuse;

	// Send it to FragColor
	FragColor = vec4(result, 1.f);
}