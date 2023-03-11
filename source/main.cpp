#include "libs.hpp"

#include "utils/types.hpp"
#include "utils/config.hpp"
#include "utils/assert.hpp"
#include "utils/enum.hpp"
#include "utils/array.hpp"
#include "utils/ring_buffer.hpp"
#include "utils/hash.hpp"
#include "utils/memory.hpp"
#include "utils/path.hpp"
#include "utils/math.hpp"
#include "utils/vector.hpp"
#include "utils/matrix.hpp"
#include "utils/camera.hpp"
#include "utils/shader.hpp"
#include "utils/file_monitor.hpp"
#include "utils/time.hpp"
#include "utils/window.hpp"
#include "utils/input.hpp"
#include "utils/imgui.hpp"
#include "utils/defer.hpp"
#include "utils/texture.hpp"
#include "options.hpp"
#include "vertices.hpp"
#include "render.hpp"
#include "sandbox.hpp"

#include "utils/camera_impl.hpp"
#include "utils/file_monitor_impl.hpp"
#include "utils/memory_impl.hpp"
#include "utils/path_impl.hpp"
#include "utils/window_impl.hpp"
#include "utils/shader_impl.hpp"
#include "utils/time_impl.hpp"
#include "utils/input_impl.hpp"

int main(int arg_count, char** args) {
	init_temporary_storage();
	init_paths();
	init_time();
	init_file_monitors();
	init_window();
	init_textures();
	init_render();
	init_shaders();
	init_imgui();

	while (!glfwWindowShouldClose(window.handle)) {
		update_temporary_storage();
		update_file_monitors();
		update_input();
		update_camera();
		update_imgui();
		update_sandbox();
		update_render();
		update_time();
	}
}
