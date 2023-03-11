struct Texture {
	hash_t hash;
	uint32 handle;
	int32 width;
	int32 height;
	int32 channels;
};
Array<Texture> textures;

Texture* find_texture(const char* name) {
	auto hash = hash_string(name);
	arr_for(textures, texture) {
		if (texture->hash == hash) return texture;
	}

	return nullptr;
}

void add_texture(const char* name) {
	auto path = gfx_path::texture(name);
	
	auto texture = arr_push(&textures);
	texture->hash = hash_string(name);
	
	stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(path, &texture->width, &texture->height, &texture->channels, 0);
	defer { stbi_image_free(data); };

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	glGenTextures(1, &texture->handle);
	glBindTexture(GL_TEXTURE_2D, texture->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


}

void init_textures() {
	arr_init(&textures, 64);

	add_texture("wall.png");
	add_texture("face.png");
	add_texture("crate-diffuse.png");
}
