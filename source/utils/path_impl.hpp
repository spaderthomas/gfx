void normalize_path(char* str) {
	int i = 0;
	while (true) {
		if (str[i] == 0) break;
		if (str[i] == '\\') {
			str[i] = '/';
		}
		i++;
	}
}

char* extract_file_name(char* full_path) {
	auto size = strlen(full_path);
	auto index = size - 1;
	while (true) {
		if (index < 0) break;
		if (full_path[index] == '/') { index += 1; break; }
		index -= 1;
	}
	
	auto length = size - index; 
	char* file_name = (char*)temporary_storage.alloc(length + 1);
	strncpy(file_name, full_path + index, length);
	return file_name;
} 

bool is_directory(char* path) {
	auto attribute = GetFileAttributesA(path);
	if (attribute == INVALID_FILE_ATTRIBUTES) return false;
	return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

char* wide_to_utf8(uint16* string, uint32 length) {
	char* utf8 = (char*)temporary_storage.alloc(length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)string, length, utf8, length, NULL, NULL);
	return utf8;
}
