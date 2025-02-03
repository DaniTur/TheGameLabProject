#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 DebugColor;

// texture samplers
//uniform sampler2D ourtexture;
uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;
//uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;
uniform sampler2D texture_normal1;

void main()
{
//	vec3 lightColor = vec3(1.0);
//	vec3 objectColor = texture(texture_diffuse1, TexCoord).rgb;
//	vec3 result = lightColor * objectColor;

//	vec3 color = normalize(Normal) * 0.5 + 0.5; // Convertir de [-1,1] a [0,1]
//    FragColor = vec4(color, 1.0);

	//FragColor = vec4(DebugColor, 1.0);

//	FragColor = vec4(TexCoord, 0.0, 1.0);

	FragColor = texture(texture_diffuse1, TexCoords);
}