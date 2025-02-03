#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 DebugColor;

uniform mat4 WVPmatrix;

void main()
{
	Normal = aNormal;
	TexCoords = aTexCoords;
	gl_Position = WVPmatrix * vec4(aPos, 1.0f);
//	DebugColor = vec3(aTexCoords, 0.0); // Solo las UV como color
}