void init_render() {
	
}

void clear_render_target() {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void swap_buffers() {
	glfwSwapBuffers(window.handle);
}

void update_render() {
	clear_render_target();
	render_imgui();
	swap_buffers();
}
