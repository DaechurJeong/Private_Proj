#version 400 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gSpec;
uniform sampler2D gDiff;

struct DirLight
{
	vec3 direction;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	vec3 lightcolor;
	float radius;
};
struct PointLight {
    vec3 lightPosition;
    vec3 lightcolor;
    
    float ambientStrength;
	float diffuseStrength;
	float specularStrength;

    float constant;
    float linear;
    float quadratic;
    float radius;
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
    float radius;
};
uniform int num_obj;
uniform PointLight pointLight[16];
uniform DirLight dirLight[16];
uniform SpotLight spotLight[16];
uniform vec3 viewPos;
uniform int DrawStatus;
uniform int type[16];

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;

	float spec = pow(max(dot(normal, reflectDir), 0.0), 16.0);

	vec3 ambient = light.ambientStrength * light.lightcolor;
	vec3 diffuse = light.diffuseStrength * diff * light.lightcolor;
	vec3 specular = light.specularStrength * spec * light.lightcolor;
	return (ambient + diffuse + specular);
}
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPosition - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;
	float spec = pow(max(dot(normal, reflectDir), 0.0), 16.0);

	float distance = length(light.lightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambientStrength * light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff ) * light.lightcolor;
	vec3 specular = (light.specularStrength * spec ) * light.lightcolor;

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
	float spec = pow(max(dot(normal, reflectDir), 0.0), 16.0);

	float distance = length(light.lightPosition - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = pow(clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0), light.fallOff);

	vec3 ambient = light.ambientStrength * light.lightcolor;
	vec3 diffuse = (light.diffuseStrength * diff )* light.lightcolor;
	vec3 specular = (light.specularStrength * spec )* light.lightcolor;

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Diffuse = texture(gDiff, TexCoord).rgb;
    float Specular = texture(gSpec, TexCoord).r;
    
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    if(DrawStatus == 0)
    {
        for(int i = 0; i < num_obj; ++i)
    	{
    		float distance = length(dirLight[i].direction - FragPos);
    		if(distance < dirLight[i].radius)
    	   	{
    			if(type[i] == 1)
    				lighting += CalculateDirLight(dirLight[i], Normal, viewDir);
    		}
    	}

   		for(int i = 0; i < num_obj; ++i)
    	{
    		float distance = length(pointLight[i].lightPosition - FragPos);
    		if(distance < pointLight[i].radius)
    	   	{
    			if(type[i] == 0)
    				lighting += CalculatePointLight(pointLight[i], Normal, FragPos, viewDir);
    		}
    	}

    	for(int i = 0; i < num_obj; ++i)
    	{
    		float distance = length(spotLight[i].lightPosition - FragPos);
    		if(distance < spotLight[i].radius)
    	   	{
    			if(type[i] == 2)
    				lighting += CalculateSpotLight(spotLight[i], Normal, FragPos, viewDir);
    		}
    	}
    }
    FragColor = vec4(lighting, 1.0);
    if(DrawStatus == 1)
        FragColor = vec4(FragPos, 1.0);
    else if(DrawStatus == 2)
        FragColor = vec4(Normal, 1.0);
    else if(DrawStatus == 3)
        FragColor = texture(gDiff, TexCoord);
    else if(DrawStatus == 4)
        FragColor = texture(gSpec, TexCoord);
}
