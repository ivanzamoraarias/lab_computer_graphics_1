#version 420

layout(location = 0) in vec4 position;
uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(position.xyz, 1.0);
}
