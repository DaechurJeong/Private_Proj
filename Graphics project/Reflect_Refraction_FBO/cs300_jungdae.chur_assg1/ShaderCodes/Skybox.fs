#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 TexCoord;

uniform sampler2D skybox[6];

vec4 Convert_to_uv(vec3 pos);

vec4 Convert_to_uv(vec3 pos)
{
	vec4 REALRESULT = vec4(0,0,0,0);
	vec2 resultCoord = vec2(0,0);
	float absX = abs(pos.x);
	float absY = abs(pos.y);
	float absZ = abs(pos.z);

	int isXPositive = pos.x > 0 ? 1 : 0;
	int isYPositive = pos.y > 0 ? 1 : 0;
	int isZPositive = pos.z > 0 ? 1 : 0;

	if(isXPositive == 1 && absX >= absY && absX >= absZ) // positive X
	{
		resultCoord.x = 0.5 * (pos.z / absX + 1.0);
		resultCoord.y = 0.5 * (-pos.y / absX + 1.0);
		REALRESULT = texture(skybox[0], resultCoord);
	}
	else if(isXPositive == 0 && absX >= absY && absX >= absZ) // negative X
	{
		resultCoord.x = 0.5 * (-pos.z / absX + 1.0);
		resultCoord.y = 0.5 * (-pos.y / absX + 1.0);
		REALRESULT = texture(skybox[1], resultCoord);
	}
	else if(isYPositive == 1 && absY >= absX && absY >= absZ) // positive Y
	{
		resultCoord.y = 0.5 * (pos.x / absY + 1.0);
		resultCoord.x = 0.5 * (pos.z / absY + 1.0);
		REALRESULT = texture(skybox[2], resultCoord);
	}
	else if(isYPositive == 0 && absY >= absX && absY >= absZ) // negative Y
	{
		resultCoord.y = 0.5 * (-pos.x / absY + 1.0);
		resultCoord.x = 0.5 * (pos.z / absY + 1.0);
		REALRESULT = texture(skybox[3], resultCoord);
	}
	else if(isZPositive == 1 && absZ >= absX && absZ >= absY) // positive Z
	{
		resultCoord.x = 0.5 * (-pos.x / absZ + 1.0);
		resultCoord.y = 0.5 * (-pos.y / absZ + 1.0);
		REALRESULT = texture(skybox[4], resultCoord);
	}
	else if(isZPositive == 0 && absZ >= absX && absZ >= absY) // negative Z
	{
		resultCoord.x = 0.5 * (pos.x / absZ + 1.0);
		resultCoord.y = 0.5 * (-pos.y / absZ + 1.0);
		REALRESULT = texture(skybox[5], resultCoord);
	}
	return REALRESULT;
}
void main()
{    
	FragColor = Convert_to_uv(FragPos);
}