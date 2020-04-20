#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct DirLight
{
	vec3 direction;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	vec3 lightcolor;
};

uniform sampler2D skybox[6];
uniform vec3 viewPos;
uniform float refractionMaterial;
uniform int lightStatus;
uniform int isUsingPhong;
uniform vec3 color;
uniform int num_obj;

uniform DirLight dirLight[16];

float r = 1.0 / refractionMaterial;
vec4 ReflectionMapping(vec3 pos);
vec4 RefractionMapping(vec3 pos);
vec3 ReflectionVec();
vec3 RefractionVec();
float FindMax(float a, float b, float c);
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 RefractionVec()
{
    vec3 refract;
    vec3 Ivec = normalize(FragPos - viewPos);

    float k = 1.0 - r * r * (1.0 - dot(Normal, Ivec) * dot(Normal, Ivec));

    if(k < 0.0)
        refract = vec3(0,0,0);
    else
        refract = r * Ivec - (r * dot(Normal, Ivec) + sqrt(k)) * Normal;

    return refract;
}

vec3 ReflectionVec()
{
    vec3 Ivec = normalize(FragPos - viewPos);

    vec3 reflect = 2 * dot(Ivec, Normal) * Normal - Ivec;
    return reflect;
}
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = 2 * dot(lightDir, normal) * normal - lightDir;

	float spec = pow(max(dot(normal, reflectDir), 0.0), 32);

	vec3 ambient = light.ambientStrength * light.lightcolor;
	vec3 diffuse = light.diffuseStrength * diff * light.lightcolor;
	vec3 specular = light.specularStrength * spec * light.lightcolor;
	return (ambient + diffuse + specular);
}
float FindMax(float a, float b, float c)
{
    float max = a;
    if(b > max)
        max = b;
    if(c > max)
        max = c;
    return max;
}
vec4 ReflectionMapping(vec3 pos)
{
    vec4 REALRESULT;
    vec2 resultCoord;
    vec3 temp;

    float absX = abs(pos.x);
    float absY = abs(pos.y);
    float absZ = abs(pos.z);
    float maxAxis = FindMax(absX, absY, absZ);

    int isXPositive = pos.x > 0 ? 1 : 0;
    int isYPositive = pos.y > 0 ? 1 : 0;
    int isZPositive = pos.z > 0 ? 1 : 0;

    if(maxAxis == absX)
    {
        if(isXPositive == 1) // right
        {
            resultCoord.x = (pos.z / absX + 1) * 0.5;
            resultCoord.y = (-pos.y / absX + 1) * 0.5;
            REALRESULT = (texture(skybox[0], resultCoord));
        }
        else // left
        {
            resultCoord.x = (-pos.z / absX) * 0.5 + 0.5;
            resultCoord.y = (-pos.y / absX) * 0.5 + 0.5;
            REALRESULT = (texture(skybox[5], resultCoord));
        }
    }
    if(maxAxis == absY)
    {
        if(isYPositive == 1) // up
        {
            resultCoord.y = (-pos.x / absY) * 0.5 + 0.5;
            resultCoord.x = (-pos.z / absY) * 0.5 + 0.5;
            REALRESULT = (texture(skybox[4], resultCoord));
        }
        else
        {
            resultCoord.y = (-pos.x / absY) * 0.5 + 0.5;
            resultCoord.x = (pos.z / absY) * 0.5 + 0.5;
            REALRESULT = (texture(skybox[3], resultCoord));
        }
    }
    if(maxAxis == absZ)
    {
        if(isZPositive == 1) // front
        {
            resultCoord.x = (-pos.x / absZ) * 0.5 + 0.5;
            resultCoord.y = (-pos.y / absZ) * 0.5 + 0.5;
            REALRESULT = (texture(skybox[2], resultCoord));
        }
        else
        {
            resultCoord.x = (pos.x / absZ) * 0.5 + 0.5;
            resultCoord.y = (-pos.y / absZ) * 0.5 + 0.5;
            REALRESULT = (texture(skybox[1], resultCoord));
        }
    }
    return REALRESULT;
}
vec4 RefractionMapping(vec3 pos)
{
    vec4 REALRESULT;
    vec2 resultCoord;

    float absX = abs(pos.x);
    float absY = abs(pos.y);
    float absZ = abs(pos.z);

    int isXPositive = pos.x > 0 ? 1 : 0;
    int isYPositive = pos.y > 0 ? 1 : 0;
    int isZPositive = pos.z > 0 ? 1 : 0;

    if(isXPositive == 1 && absX >= absY && absX >= absZ) // positive X
    {
        resultCoord.x = 0.5f * (pos.z / absX + 1.f);
        resultCoord.y = 0.5f * (pos.y / absX + 1.f);
        REALRESULT = texture(skybox[5], resultCoord);
    }
    if(isXPositive == 0 && absX >= absY && absX >= absZ) // negative X
    {
        resultCoord.x = 0.5f * (-pos.z / absX + 1.f);
        resultCoord.y = 0.5f * (pos.y / absX + 1.f);
        REALRESULT = texture(skybox[0], resultCoord);
    }
    if(isYPositive == 1 && absY >= absX && absY >= absZ) // positive Y
    {
        resultCoord.y = 0.5f * (pos.x / absY + 1.f);
        resultCoord.x = 0.5f * (-pos.z / absY + 1.f);
        REALRESULT = texture(skybox[3], resultCoord);
    }
    if(isYPositive == 0 && absY >= absX && absY >= absZ) // negative Y
    {
        resultCoord.y = 0.5f * (pos.x / absY + 1.f);
        resultCoord.x = 0.5f * (pos.z / absY + 1.f);
        REALRESULT = texture(skybox[4], resultCoord);
    }
    if(isZPositive == 1 && absZ >= absX && absZ >= absY) // positive Z
    {
        resultCoord.x = 0.5f * (-pos.x / absZ + 1.f);
        resultCoord.y = 0.5f * (pos.y / absZ + 1.f);
        REALRESULT = texture(skybox[1], resultCoord);
    }
    if(isZPositive == 0 && absZ >= absX && absZ >= absY) // negative Z
    {
        resultCoord.x = 0.5f * (pos.x / absZ + 1.f);
        resultCoord.y = 0.5f * (pos.y / absZ + 1.f);
        REALRESULT = texture(skybox[2], resultCoord);
    }
    return REALRESULT;
}
void main()
{
    vec4 result = vec4(0,0,0,0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 mix_color;
    if(lightStatus == 0) // Reflect
    {
        vec3 reflect = ReflectionVec();
        result = ReflectionMapping(reflect);
    }
    else if(lightStatus == 1) // Refraction
    {
        vec3 refract = RefractionVec();
        result = RefractionMapping(refract);
    }
    else if(lightStatus == 2) // Mix
    {
        vec3 reflect = ReflectionVec();
        vec3 refract = RefractionVec();
        vec3 reflectResult = vec3(ReflectionMapping(reflect).rgb);
        vec3 refractResult = vec3(RefractionMapping(refract).rgb);

        result = vec4(mix(reflectResult, refractResult, r), 1.0);
    }
    if(isUsingPhong == 1)
	{
		vec3 light = vec3(0,0,0);
		for(int i = 0; i < num_obj; ++i)
			light += CalculateDirLight(dirLight[i], Normal, viewDir);
			
		vec3 light_mix_color = mix(vec3(result), light, 0.5);
        result = vec4(light_mix_color, 1.0);
	}
    
	FragColor = result;
}