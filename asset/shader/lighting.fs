#version 330 core
out vec4 FragColor;

in vec3 f_normal;
in vec3 f_position;
in vec2 f_uv;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 camera;

void main()
{
	vec3 light_direction = normalize(light.position - f_position);
	vec3 diffuse_sample = vec3(texture(material.diffuse, f_uv));
	vec3 specular_sample = vec3(texture(material.specular, f_uv));
	vec3 emission_sample = vec3(texture(material.emission, f_uv));

	// ambient
	vec3 ambient = diffuse_sample * light.ambient;

	// diffuse
	vec3 normal = normalize(f_normal);
	float diffuse_strength = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = light.diffuse * (diffuse_strength * diffuse_sample);

	// specular
	vec3 look_direction = normalize(camera - f_position);
	vec3 r = reflect(-light_direction, normal);
	float d = max(dot(look_direction, r), 0.0);
	float specular_strength = pow(d, material.shininess);
	vec3 specular = light.specular * (specular_strength * specular_sample);

	// emission
	vec3 emission = emission_sample;
	
	vec3 color = ambient + diffuse + specular + emission;
    FragColor = vec4(color, 1.0);
	
    //FragColor = vec4(light_color * object_color, 1.0);
}
