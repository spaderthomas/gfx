void Camera::set_position(glm::vec3 position) {
	this->position = position;
}

void Camera::set_up_axis(glm::vec3 axis) {
	up = axis;
}

void Camera::move(glm::vec3 delta) {
	glm::vec3 right   = make_right_vector() * delta.x;
	glm::vec3 up      = make_up_vector() * delta.y;
	glm::vec3 forward = make_forward_vector() * delta.z;
	
	position = position + right + up + forward;
}

glm::vec3 Camera::make_forward_vector() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::normalize(direction);
}

glm::vec3 Camera::make_right_vector() {
	auto orthogonal = glm::cross(make_forward_vector(), up);
	auto orthonormal = glm::normalize(orthogonal);
	return orthonormal;
}

glm::vec3 Camera::make_up_vector() {
	auto orthogonal = glm::cross(make_forward_vector(), make_right_vector());
	auto orthonormal = glm::normalize(orthogonal);
	return orthonormal;
}

glm::mat4 Camera::make_view_matrix() {
	/*
	  First, you need an orthonormal basis of the vector space defined by the camera. That is,
	  with one axis being the direction the camera is looking (D), plus an orthogonal right vector (R)
	  and an orthogonal up vector (U).

	  To get the right vector: Since we know that the cross product of D and the Y axis is a vector 
	  orthogonal to both, we'll get a vector pointing in the positive X direction of the camera's
	  vector space. This vector is R.

	  To get the up vector: We have the equivalent of a Z axis and an X axis in the camera's vector
	  space. We need a Y axis. The Y axis must be orthogonal to the Z and X axis, so again we can
	  use a cross product. U = D x R.

	  Once we have an orthonormal basis, we can just stuff them in a matrix that represents a projection
	  from the standard axis vector space into the camera's vector space
	  [ Rx  Ry  Rz  0 ]     [ 1   0   0   -Px ]
	  | Ux  Uy  Uz  0 |     | 0   1   0   -Py |
	  | Dx  Dy  Dz  0 |     | 0   0   1   -Pz |
	  [ 0   0   0   0 ]     [ 0   0   1   1   ]
	 */

	auto forward = make_forward_vector();
	return glm::lookAt(position, position + forward, up);
}

void update_camera() {
	auto& input = get_input_manager();
	if (!input.is_channel_enabled(INPUT_MASK_GAME)) return;
	
	camera.yaw += input.mouse_delta.x * options::sensitivity * -1;
	camera.yaw = fmodf(camera.yaw, 360.0f);
	camera.pitch += input.mouse_delta.y * options::sensitivity;
	camera.pitch = math::clamp(camera.pitch, -90.0f, 90.0f);

	auto move = glm::vec3(0.0f, 0.0f, 0.0f);
	if (input.is_down(GLFW_KEY_W)) {
		move.z = 1 * options::camera_speed;
	}
	if (input.is_down(GLFW_KEY_S)) {
		move.z = -1 * options::camera_speed;
	}
	if (input.is_down(GLFW_KEY_D)) {
		move.x = 1 * options::camera_speed;
	}
	if (input.is_down(GLFW_KEY_A)) {
		move.x = -1 * options::camera_speed;
	}
	if (input.is_down(GLFW_KEY_Q)) {
		move.y = 1 * options::camera_speed;
	}
	if (input.is_down(GLFW_KEY_E)) {
		move.y = -1 * options::camera_speed;
	}
	camera.move(move);

	options::fov -= input.scroll.y;
	options::fov = math::clamp(options::fov, 1.0, 180.f);
}
