void update_sandbox() {
	ImGui::Begin("GFX");

	ImGui::SliderFloat("Near Plane", &options::near_plane, -100.0f, 100.0f);
	ImGui::SliderFloat("Far Plane", &options::far_plane, -100.0f, 100.0f);
	ImGui::SliderFloat("FOV", &options::fov, 0.f, 180.0f);
	ImGui::Checkbox("Use GLM", &options::use_glm);
	
	ImGui::End();
}
