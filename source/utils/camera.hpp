struct Camera {
	float32 yaw = -90.0f;
	float32 pitch = 0;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	void set_position(glm::vec3 position);
	void set_up_axis(glm::vec3 axis);
	void move(glm::vec3 axes);
	glm::vec3 make_forward_vector();
	glm::vec3 make_right_vector();
	glm::vec3 make_up_vector();
	glm::mat4 make_view_matrix();
};
Camera camera;
