#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
	vec3 ambient;	
};
struct DirLight
{
	vec3 direction;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	vec3 lightcolor;
};

struct PointLight
{
	vec3 lightPosition;

	float constant;
	float linear;
	float quadratic;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	vec3 lightcolor;
};

struct SpotLight {
    vec3 lightPosition;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    vec3 lightcolor;
    float fallOff;
};
#define NR_POINT_LIGHTS 16

uniform int num_obj;
uniform Material material;
uniform DirLight dirLight[NR_POINT_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_POINT_LIGHTS];

uniform int type[NR_POINT_LIGHTS];
uniform vec3 viewPos;
uniform vec3 color;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;

	float spec = pow(max(dot(normal, reflectDir), 0.0), 64);

	vec3 ambient = light.ambientStrength * material.ambient * light.lightcolor;
	vec3 diffuse = light.diffuseStrength * diff * light.lightcolor;
	vec3 specular = light.specularStrength * spec * light.lightcolor;
	return (ambient + diffuse + specular);
}
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPosition - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;
	float spec = pow(max(dot(normal, reflectDir), 0.0), 64);

	float distance = length(light.lightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = (light.ambientStrength * material.ambient )* light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff)* light.lightcolor;
	vec3 specular = (light.specularStrength * spec)* light.lightcolor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPosition - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;
	float spec = pow(max(dot(normal, reflectDir), 0.0), 64);

	float distance = length(light.lightPosition - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = pow(clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0), light.fallOff);

	vec3 ambient = (light.ambientStrength * material.ambient )* light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff)* light.lightcolor;
	vec3 specular = (light.specularStrength * spec)* light.lightcolor;

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{  
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = color; // I_emissive

   	for(int i = 0; i < num_obj; ++i)
    {
    	if(type[i] == 1)
    		result += CalculateDirLight(dirLight[i], norm, viewDir);
    }

   	for(int i = 0; i < num_obj; ++i)
    {
    	if(type[i] == 0)
    		result += CalculatePointLight(pointLight[i], norm, FragPos, viewDir);
    }

    for(int i = 0; i < num_obj; ++i)
    {
    	if(type[i] == 2)
    		result += CalculateSpotLight(spotLight[i], norm, FragPos, viewDir);
    }

	// Send it to FragColor
	FragColor = vec4(result, 1.f);
}