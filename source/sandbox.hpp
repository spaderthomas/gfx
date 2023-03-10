void update_sandbox() {
	ImGui::Begin("GFX");

	auto& input = get_input_manager();
	ImGui::Text("Mouse: (%f, %f)", input.mouse.x, input.mouse.y);
	ImGui::Text("Camera: (%f, %f, %f)", camera.position.x, camera.position.y, camera.position.z);
	ImGui::Text("Yaw: %f", camera.yaw);
	ImGui::Text("Pitch: %f", camera.pitch);

	ImGui::SliderFloat("Near Plane", &options::near_plane, -100.0f, 100.0f);
	ImGui::SliderFloat("Far Plane", &options::far_plane, -100.0f, 100.0f);
	ImGui::SliderFloat("FOV", &options::fov, 0.f, 180.0f);
	ImGui::SliderFloat("Camera Speed", &options::camera_speed, 0.05f, 0.2f);
	ImGui::SliderFloat("Sensitivity", &options::sensitivity, 0.1f, 0.5f);
	ImGui::Checkbox("Use GLM", &options::use_glm);
	
	ImGui::End();
}
