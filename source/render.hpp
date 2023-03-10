float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

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

	Matrix4 my_projection;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};
OpenGL opengl;

void init_render() {
	glEnable(GL_DEPTH_TEST);

	opengl.geometry.init();
	opengl.geometry.add_attribute(GL_FLOAT, 3);
	opengl.geometry.add_attribute(GL_FLOAT, 2);
	opengl.geometry.build();
}

void clear_render_target() {
	glClearColor(CLEAR_COLOR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swap_buffers() {
	glfwSwapBuffers(window.handle);
}

Matrix4 make_frustum_matrix(float32 r, float32 l, float32 t, float32 b, float32 n, float32 f) {
	Matrix4 matrix = {0};
	matrix[0][0] = 2 * n / (r - l);
	matrix[0][2] = (r + l) / (r - l);
	matrix[1][1] = 2 * n / (t - b);
	matrix[1][2] = (t + b) / (t - b);
	matrix[2][2] = -1 * (f + n) / (f - n);
	matrix[2][2] = -2 * f * n / (f - n);
	matrix[3][2] = -1;

	return matrix;
}

Matrix4 make_projection_matrix(float32 fov, float32 aspect, float32 n, float32 f) {	
	float32 t = tanf(fov * 0.5f);

	Matrix4 matrix = {0};
	matrix.zero();
	matrix[0][0] = 1 / (t * aspect);
	matrix[1][1] = 1 / (t);
	matrix[2][2] = -1 * (f + n) / (f - n);
	matrix[2][3] = -1;
	matrix[3][2] = -2 * f * n / (f - n);

	return matrix;
}

void update_render() {
	clear_render_target();

	opengl.geometry.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);


	opengl.projection = glm::perspective(math::radians(options::fov), window.get_aspect_ratio(), options::near_plane, options::far_plane);
	opengl.my_projection = make_projection_matrix(math::radians(options::fov), window.get_aspect_ratio(), options::near_plane, options::far_plane);

	auto texture0 = find_texture("wall.png");
	auto texture1 = find_texture("face.png");
	
	auto shader = find_shader("texture");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0->handle);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1->handle);
	
	shader->begin();
	shader->set_int("texture0", 0);
	shader->set_int("texture1", 1);
	if (options::use_glm) {
		shader->set_mat4("projection", opengl.projection);
	}
	else {
		shader->set_mat4("projection", opengl.my_projection);
	}

	const float radius = 40.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;

	glm::mat4 view = camera.make_view_matrix();
	shader->set_mat4("view", view);
		
	for (uint32 i = 0; i < 10; i++) {
		auto model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
	
		float32 angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
		shader->set_mat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	shader->end();
	
	opengl.geometry.unbind();
	
	render_imgui();
	swap_buffers();
}
