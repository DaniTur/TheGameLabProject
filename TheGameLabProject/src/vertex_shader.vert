#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Send vertex info needed in the fragment shader
	// Fragment position at world space
	FragPos = vec3(model * vec4(aPos, 1.0));
	// Normals at world space and re-calculate the normals in case the model was not uniform scaled
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	TexCoords = aTexCoords;

	// Transform to 2D screen
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}