#version 330 core


out vec4 FragmentColor;

uniform vec3 Color;
uniform sampler2D Texture;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 lightPosition;
    vec3 intensities; //a.k.a the color of the light
    float attenuation; 
    float ambientCoefficient; 
};


void main(void)
{
	FragmentColor = vec4(1.0, 0.0, 1.0, 1.0);
}