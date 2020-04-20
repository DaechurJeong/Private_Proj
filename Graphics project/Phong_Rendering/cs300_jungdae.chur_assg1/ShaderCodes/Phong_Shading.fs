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
#define TWOPI  6.283185308
#define PI 3.141592654
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform int projtype;
uniform int calculateByNormal;
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
float FindMax(vec3 m_vec);
vec2 Projection(vec3 normals);

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;

	// shiness = vec3(texture(ourTexture1, TexCoord)).r;
	float spec = pow(max(dot(normal, reflectDir), 0.0), vec3(texture(ourTexture2, TexCoord)).r);

	vec3 ambient = light.ambientStrength * material.ambient * light.lightcolor;
	vec3 diffuse = light.diffuseStrength * diff * vec3(texture(ourTexture1, TexCoord))* light.lightcolor;
	vec3 specular = light.specularStrength * spec * vec3(texture(ourTexture2, TexCoord))* light.lightcolor;
	return (ambient + diffuse + specular);
}
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPosition - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;
	float spec = pow(max(dot(normal, reflectDir), 0.0), vec3(texture(ourTexture2, TexCoord)).r);

	float distance = length(light.lightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = (light.ambientStrength * material.ambient )* light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff * vec3(texture(ourTexture1, TexCoord)))* light.lightcolor;
	vec3 specular = (light.specularStrength * spec * vec3(texture(ourTexture2, TexCoord)))* light.lightcolor;

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
	float spec = pow(max(dot(normal, reflectDir), 0.0), vec3(texture(ourTexture2, TexCoord)).r);

	float distance = length(light.lightPosition - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = pow(clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0), light.fallOff);

	vec3 ambient = (light.ambientStrength * material.ambient )* light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff * vec3(texture(ourTexture1, TexCoord)))* light.lightcolor;
	vec3 specular = (light.specularStrength * spec * vec3(texture(ourTexture2, TexCoord)))* light.lightcolor;

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
float FindMax(vec3 m_vec)
{
	float max_ = abs(m_vec.x);
	if(abs(m_vec.y) > max_)
		max_ = m_vec.y;
	if(abs(m_vec.z) > max_)
		max_ = m_vec.z;
	return max_;
}
vec2 Projection(vec3 normals)
{
	vec2 resultCoord = vec2(0,0);

	if(projtype == 0) // planar
    {
    	float max_value = FindMax(normals);
    	if(max_value == normals.x)
    	{
    		resultCoord.x = -normals.z / max_value;
    		resultCoord.y = normals.y / max_value;
    	}
    	else if(max_value == -normals.x)
    	{
    		resultCoord.x = normals.z / max_value;
    		resultCoord.y = normals.y / max_value;
    	}
    	else if(max_value == normals.y)
    	{
    		resultCoord.x = normals.x / max_value;
    		resultCoord.y = -normals.z / max_value;
    	}
    	else if(max_value == -normals.y)
    	{
    		resultCoord.x = normals.x / max_value;
    		resultCoord.y = normals.z / max_value;
    	}
    	else if(max_value == normals.z)
    	{
    		resultCoord.x = normals.x / max_value;
    		resultCoord.y = normals.y / max_value;
    	}
    	else if(max_value == -normals.z)
    	{
    		resultCoord.x = -normals.x / max_value;
    		resultCoord.y = normals.y / max_value;
    	}
		if(calculateByNormal == 1)
    	{
    		resultCoord.x = (resultCoord.x + 1) / 2;
    		resultCoord.y = (resultCoord.y + 1) / 2;
    	}
		return resultCoord;
    }
    else if(projtype == 1) // Cylindrical
	{
		float theta = atan(normals.y / normals.x);

		resultCoord = vec2(theta / TWOPI, (normals.z + 1) * 0.5);
		return resultCoord;
	}
	else if(projtype == 2) // Spherical
	{
		float radius = sqrt(normals.x * normals.x + normals.y * normals.y + normals.z * normals.z);
		float theta = atan(normals.y / normals.x);
		float phi = acos(normals.z / radius);

		resultCoord = vec2(theta / TWOPI, phi / PI);
		return resultCoord;
	}
	return resultCoord;
}
void main()
{  
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = color; // I_emissive

    vec2 TexCoord_ = vec2(0,0);
    if(calculateByNormal == 1) // by normal
    {
    	TexCoord_ = Projection(norm);
	}
	else if (calculateByNormal == 0) // by position
	{
		TexCoord_ = Projection(FragPos);
	}

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
	//FragColor = vec4(result, 1.f);
	//FragColor = texture(ourTexture1, TexCoord_) * vec4(result, 1.f);

	FragColor = texture(ourTexture1 + ourTexture2, TexCoord_) * vec4(result, 1.f);
}