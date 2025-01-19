#version 330 core
// "Linking" the vertex data location with the input of the first shader of the pipeline
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;    // texture coordinates

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transformMatrix;

void main()
{
    gl_Position = transformMatrix * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}