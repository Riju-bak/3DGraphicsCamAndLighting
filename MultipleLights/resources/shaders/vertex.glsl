#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lampPos;
uniform vec3 viewPos;

// 'out vec4 gl_Position' is optional, but why? If it's not mentioned as input, how will the vertex shader pass this information onto the rest of the graphics pipeline?
// Answer: gl_Position is a built-in output variable in GLSL (OpenGL Shading Language) that represents the final position of a vertex in clip space.
out vec4 gl_Position;
out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec3 ViewPos;

out vec2 TexCoord;

void main()
{
	mat4 transform = projection * view * model;
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	ViewPos = viewPos;
	LightPos = lampPos;
	Normal = mat3(transpose(inverse(model))) * aNormal;	//TODO: Figure out why mat3() will not lead to loss of info
	TexCoord = aTexCoord;
}