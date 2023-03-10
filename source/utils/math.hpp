namespace math {
	constexpr float32 pi = 3.1415927f;
	
	float32 radians(float32 degrees) {
		return degrees * pi / 180;
	}

	template <typename T>
	T max(T a, T b) {
		return a > b ? a : b;
	}
	
	template <typename T>
	T min(T a, T b) {
		return a < b ? a : b;
	}
	
	float32 clamp(float32 value, float32 min_value, float32 max_value) {
		return min(max(value, min_value), max_value);
	}
}
