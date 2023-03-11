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

	if (ImGui::TreeNode("Materials")) {
		for (uint32 i = 0; i < materials.size; i++) {
			auto material = materials[i];
			if (ImGui::TreeNode((void*)(int32*)i, "Material %d", i)) {
				ImGui::SliderFloat3("Ambient",   glm::value_ptr(material->ambient), 0.0f, 1.0f);
				ImGui::SliderFloat3("Diffuse",   glm::value_ptr(material->diffuse), 0.0f, 1.0f);
				ImGui::SliderFloat3("Specular",  glm::value_ptr(material->specular), 0.0f, 1.0f);
				ImGui::SliderFloat( "Shininess", &material->shininess, 0.0f, 256.0f);
				ImGui::TreePop();
				
				if (ImGui::Button("Use")) {
					active_material = i;
				}
			}
		}
		
		if (ImGui::Button("Add")) {
			auto last_material = arr_back(&materials);
			auto material = arr_push(&materials, *last_material);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light")) {
		ImGui::SliderFloat3("Ambient",  glm::value_ptr(light.ambient), 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse",  glm::value_ptr(light.diffuse), 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", glm::value_ptr(light.specular), 0.0f, 1.0f);
		ImGui::SliderFloat3("Position", glm::value_ptr(light.position), 0.0f, 1.0f);
		ImGui::Checkbox(    "Move",     &light.move);
		ImGui::TreePop();
	}

	
	ImGui::End();

	if (input.was_pressed(GLFW_KEY_F1)) {
		input.toggle_game_input();
	}
}
