#version 330 core
layout (location = 0) in vec3 v_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(v_pos.x, v_pos.y, v_pos.z, 1.0);
}
