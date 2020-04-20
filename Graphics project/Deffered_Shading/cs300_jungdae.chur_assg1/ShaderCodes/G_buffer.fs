#version 400 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gSpec;
layout (location = 3) out vec4 gDiff;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

float FindMax(vec3 m_vec);
vec2 Projection(vec3 normals);

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
	return resultCoord;
}
void main()
{
    vec2 TexCoord_ = Projection(Normal);

    gPosition = FragPos;
    gNormal = normalize(Normal);
        
    gDiff.rgb = texture(ourTexture1, TexCoord_).rgb;
    gSpec.rgb = texture(ourTexture2, TexCoord_).rgb;
}