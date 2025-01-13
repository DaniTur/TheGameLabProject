#version 330 core
// "Linking" the vertex data location with the input of the first shader of the pipeline
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

uniform float xOffset;
out vec3 ourColor;   // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);    // see how we directly give a vec3 to vec4's constructor
    ourColor = aColor; // set the output variable to a dark-red color
}