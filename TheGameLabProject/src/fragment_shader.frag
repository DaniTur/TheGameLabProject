#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
//uniform sampler2D ourtexture;
uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture_diffuse1, TexCoord);
}