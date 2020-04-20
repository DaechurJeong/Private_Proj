#version 400 core
in vec4 FragColor_;

out vec4 FragColor;

void main()
{ 
	// Send it to FragColor
	FragColor = FragColor_;
}