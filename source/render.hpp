float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float32 shininess;
};
Array<Material> materials;
uint32 active_material;

struct Light {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	bool move;
};
Light light;

struct GpuBufferDescriptor {
	struct Attribute {
		uint32 offset;
		uint32 count;
		uint32 kind;
		uint32 size;
	};

	constexpr static uint32 max_attributes = 16;
	
	uint32 vao;
	uint32 buffer;
	uint32 stride;
	Array<Attribute> attributes;
	
	void init();
	void build();
	void bind();
	void unbind();
	void add_attribute(uint32 kind, uint32 count);
};

void GpuBufferDescriptor::init() {
	arr_init(&attributes, max_attributes);
}

void GpuBufferDescriptor::add_attribute(uint32 kind, uint32 count) {
	auto attribute = arr_push(&attributes);
	attribute->offset = stride;
	attribute->count   = count;
	attribute->kind   = kind;

	if (attribute->kind == GL_FLOAT) attribute->size = sizeof(float) * count;

	stride += attribute->size;
}

void GpuBufferDescriptor::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void GpuBufferDescriptor::unbind() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GpuBufferDescriptor::build() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &buffer);
	
	bind();

	for (uint32 i = 0; i < attributes.size; i++) {
		auto attribute = attributes[i];
		glVertexAttribPointer(i, attribute->count, attribute->kind, GL_FALSE, (GLsizei)stride, (void*)(attribute->offset));
		glEnableVertexAttribArray(i);
	}

	unbind();
}

struct OpenGL {
	GpuBufferDescriptor geometry;
	GpuBufferDescriptor lights;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};
OpenGL opengl;

void init_render() {
	glEnable(GL_DEPTH_TEST);

	opengl.geometry.init();
	opengl.geometry.add_attribute(GL_FLOAT, 3);
	opengl.geometry.add_attribute(GL_FLOAT, 3);
	opengl.geometry.build();

	opengl.lights.init();
	opengl.lights.add_attribute(GL_FLOAT, 3);
	opengl.lights.add_attribute(GL_FLOAT, 3);
	opengl.lights.build();

	arr_init(&materials, 16);
	auto material = arr_push(&materials);
	material->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material->shininess = 32;

	light.ambient  = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse  = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(1.2f, 1.0f, 2.0f);
}

void clear_render_target() {
	glClearColor(CLEAR_COLOR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swap_buffers() {
	glfwSwapBuffers(window.handle);
}


void update_render() {
	auto now = glfwGetTime();
	auto c = cos(now);
	auto s = sin(now);
	if (light.move) {
		auto c = cos(glfwGetTime());
		auto s = sin(glfwGetTime());
		light.position.x = (float32)(c * 1.0f);
		light.position.z = (float32)(s * 1.0f);
	}
	
	glm::mat4 projection = glm::perspective(math::radians(options::fov), window.get_aspect_ratio(), options::near_plane, options::far_plane);
	glm::mat4 view = camera.make_view_matrix();
	glm::mat4 model = glm::mat4(1.0f);

	clear_render_target();

	opengl.geometry.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	auto shader = find_shader("lighting");
	shader->begin();
	
	shader->set_mat4("projection", projection);
	shader->set_mat4("view", view);
	shader->set_mat4("model", model);
	shader->set_vec3("camera", camera.position);

	auto material = materials[active_material];
	shader->set_vec3("material.ambient", material->ambient);
	shader->set_vec3("material.diffuse", material->diffuse);
	shader->set_vec3("material.specular", material->specular);
	shader->set_float("material.shininess", material->shininess);

	shader->set_vec3("light.ambient",  light.ambient);
	shader->set_vec3("light.diffuse",  light.diffuse);
	shader->set_vec3("light.specular", light.specular);
	shader->set_vec3("light.position", light.position);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader->end();

	opengl.geometry.unbind();

	opengl.lights.bind();

	shader = find_shader("light_source");
	shader->begin();
	shader->set_mat4("projection", projection);
	shader->set_mat4("view", view);

	model = glm::mat4(1.0f);
	model = glm::translate(model, light.position);
	model = glm::scale(model, glm::vec3(0.2f));
	shader->set_mat4("model", model);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	opengl.lights.unbind();
	shader->end();

	render_imgui();
	swap_buffers();
}
