struct OpenGL {
	uint32 vao;
	uint32 elements;
	uint32 buffer;

	Matrix4 my_projection;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};
OpenGL opengl;

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

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  
void init_render() {
	glEnable(GL_DEPTH_TEST);
	
	// Generate OpenGL resources
	glGenVertexArrays(1, &opengl.vao);
	glGenBuffers(1, &opengl.buffer);
	glGenBuffers(1, &opengl.elements);

	// Describe the layout of our vertex buffer
	glBindVertexArray(opengl.vao);
	glBindBuffer(GL_ARRAY_BUFFER, opengl.buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl.elements);

	auto stride = 5 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//set_perspective_projection(100, -100, 100, -100, .1, 100);
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

	glBindVertexArray(opengl.vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, opengl.buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl.elements);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	auto view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	auto model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

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
	
	render_imgui();
	swap_buffers();
}
