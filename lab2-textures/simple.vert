#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordIn;


uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

out vec2 texCoord;
out vec2 explotionTexCoord;

// >>> @task 3.2

void main()
{
	vec4 pos = vec4(position.xyz - cameraPosition.xyz, 1);

	gl_Position = projectionMatrix * pos;

	// >>> @task 3.3
	texCoord = texCoordIn;
}