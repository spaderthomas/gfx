#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 f_position;
out vec3 f_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 homogeneous = vec4(position, 1.0);
	
	f_position = vec3(model * homogeneous);
	f_normal = normal;
	
    gl_Position = projection * view * model * homogeneous;
} 
