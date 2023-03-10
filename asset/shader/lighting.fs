#version 330 core
out vec4 FragColor;

in vec3 f_normal;
in vec3 f_position;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera;

void main()
{
	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	// diffuse
	vec3 normal = normalize(f_normal);
	vec3 light_direction = normalize(light_position - f_position);
	float diff = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	// specular
	float specular_strength = 0.5;
	vec3 look_direction = normalize(camera - f_position);
	vec3 r = reflect(-light_direction, normal);
	float d = dot(look_direction, r);
	float spec = pow(d, 256);
	vec3 specular = spec * specular_strength * light_color;
	
	
	vec3 color = (ambient + diffuse + specular) * object_color;
    FragColor = vec4(color, 1.0);
	
    //FragColor = vec4(light_color * object_color, 1.0);
}
