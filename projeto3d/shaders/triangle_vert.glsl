#version 330 core

// Declarando uma variável de entrada do tipo vec3
layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec2 InUV;

uniform mat4 ModelViewProjection;

out vec3 Color;
out vec2 UV;

void main(){
	
	vec4 Position = ModelViewProjection * vec4(InPosition, 1.0);
	Color = InColor;
	UV = InUV;
	gl_Position = Position;

}