/********************************
 * Project: Cidr                *
 * File: tensorMath.hpp         *
 * Date: 13.9.2020              *
 ********************************/

#ifndef TENSOR_MATH_HPP
#define TENSOR_MATH_HPP
#include <cmath>
#include <initializer_list>

namespace tem {

class mat2x2;
class mat3x3;
class mat4x4;

class vec2 {
public:
	float x;
	float y;
	
	vec2();
	vec2(float x, float y);
	vec2(const vec2&) = default;
	
	inline float length() const {
		return sqrt((x * x) + (y * y));
	}
	inline void normalize() {
		float l = length();
		x /= l;
		y /= l;
	}    
	void setLength(float value);
	void addLength(float value);
	void subLength(float value);
	
	vec2& operator=(const vec2& other);
	vec2& operator+=(const vec2& other);
	vec2& operator-=(const vec2& other);
	float operator^=(const vec2& other);
	vec2& operator/=(const float other);
	vec2 operator+(const vec2& other) const;
	vec2 operator-(const vec2& other) const;
	float operator^(const vec2& other) const;
	float operator*(const vec2& other) const;
	vec2 operator*(const float other) const;
	vec2 operator/(const float other) const;
};
class vec3 {
public:
	float x;
	float y;
	float z;
	
	vec3();
	vec3(float x, float y, float z);
	vec3(const vec3&) = default;

	inline float length() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	inline void normalize() {
		float l = length();
		x /= l;
		y /= l;
		z /= l;
	}
	void setLength(float value);
	void addLength(float value);
	void subLength(float value);
	
	vec3& operator=(const vec3& other);
	vec3& operator+=(const vec3& other);
	vec3& operator-=(const vec3& other);
	vec3& operator^=(const vec3& other);
	vec3& operator*=(const vec3& other);
	vec3& operator*=(const float other);
	vec3& operator/=(const float other);
	vec3 operator+(const vec3& other) const;
	vec3 operator-(const vec3& other) const;
	vec3 operator^(const vec3& other) const;
	float operator*(const vec3& other) const;
	vec3 operator*(const float other) const;
	vec3 operator/(const float other) const;
	operator vec2() const { 
		return vec2{this->x, this->y};
	}
};
class vec4 {
public:
	float x;
	float y;
	float z;
	float w = 1.0f;

	vec4();
	vec4(float x, float y, float z, float w = 1.0f);
	vec4(const vec4&) = default;

	inline float length() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	inline void normalize() {
		float l = length();
		x /= l;
		y /= l;
		z /= l;
		// w /= l;
	}
	void setLength(float value);
	void addLength(float value);
	void subLength(float value);

	vec4& operator=(const vec4& other);
	vec4& operator+=(const vec4& other);
	vec4& operator-=(const vec4& other);
	vec4& operator*=(const vec4& other);
	vec4& operator*=(const float other);
	vec4& operator^=(const vec4& other);
	vec4& operator/=(const float other);
	vec4 operator+(const vec4& other) const;
	vec4 operator-(const vec4& other) const;
	float operator*(const vec4& other) const;
	vec4 operator*(const float other) const;
	vec4 operator^(const vec4& other) const;
	vec4 operator/(const float other) const;
	operator vec3() const { 
		return vec3{this->x, this->y, this->z};
	}
};





//*****************************************************************//

class ivec2 {
public:
	int x;
	int y;
	
	ivec2();
	ivec2(int x, int y);
	
	ivec2(const ivec2&) = default;
	
	inline float length() const {
		return sqrt((x * x) + (y * y));
	}
	void setLength(int value);
	void addLength(int value);
	void subLength(int value);
	
	ivec2& operator=(const ivec2& other);
	ivec2& operator+=(const ivec2& other);
	ivec2& operator-=(const ivec2& other);
	int operator^=(const ivec2& other);
	ivec2& operator*=(const int other);
	ivec2& operator/=(const int other);
	ivec2 operator+(const ivec2& other);
	ivec2 operator-(const ivec2& other);
	int operator^(const ivec2& other);
	int operator*(const ivec2& other);
	ivec2 operator*(const int other);
	ivec2 operator/(const int other);
};
class ivec3 {
public:
	int x;
	int y;
	int z;
	
	ivec3();
	ivec3(int x, int y, int z);
	ivec3(const ivec3&) = default;
	
	inline float length() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	void setLength(int value);
	void addLength(int value);
	void subLength(int value);
	
	ivec3& operator=(const ivec3& other);
	ivec3& operator+=(const ivec3& other);
	ivec3& operator-=(const ivec3& other);
	ivec3& operator^=(const ivec3& other);
	ivec3& operator*=(const ivec3& other);
	ivec3& operator*=(const int other);
	ivec3& operator/=(const int other);
	ivec3 operator+(const ivec3& other);
	ivec3 operator-(const ivec3& other);
	ivec3 operator^(const ivec3& other);
	int operator*(const ivec3& other);
	ivec3 operator*(const int other);
	ivec3 operator/(const int other);
};
class ivec4 {
public:
	int x;
	int y;
	int z;
	int w;

	ivec4();
	ivec4(int x, int y, int z, int w);
	ivec4(const ivec4&) = default;
	
	inline int length() const {
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}
	void setLength(int value);
	void addLength(int value);
	void subLength(int value);

	ivec4& operator=(const ivec4& other);
	ivec4& operator+=(const ivec4& other);
	ivec4& operator-=(const ivec4& other);
	ivec4& operator*=(const ivec4& other);
	ivec4& operator*=(const int other);
	ivec4& operator/=(const int other);
	ivec4 operator+(const ivec4& other);
	ivec4 operator-(const ivec4& other);
	int operator*(const ivec4& other);
	ivec4 operator*(const int other);
	ivec4 operator/(const int other);
};





// TODO: make base class for matrices

class mat2x2 {
public:
	float mat[4];
	struct row {
	private: 
		float* data = nullptr;
		const float* cdata = nullptr;
		int x = 0;
	public:
		row(float* data, int x) : data(data), x(x) {}
		row(const float* cdata, int x) : cdata(cdata), x(x) {}
		float& operator[](int y) {return data[x + y * 2];}
		float operator[](int y) const {return cdata[x + y * 2];}
	};
	
	mat2x2();
	mat2x2(const float initValue);
	mat2x2(const mat2x2& other);
	mat2x2(const float mat[4]);
	mat2x2(std::initializer_list<float> list);
	mat2x2 operator=(std::initializer_list<float> list);

	void translate(float x);
	void scale(float sx);
	void rotate(float angle);
	
	mat2x2 transpose();
	mat2x2 inverse();
	inline float determinant() {
		return mat[0] * mat[3] - mat[1] * mat[2];
	}
	inline float GetValue(int row, int column) {
		return mat[column + row * 2];
	}
	inline static mat2x2 Identity() {
		return mat2x2(1);
	}
	inline static mat2x2 Null() {
		return { };
	}
	
	row operator[](int index);
	const row operator[](int index) const;
	mat2x2& operator=(const mat2x2& other);
	mat2x2& operator+=(const mat2x2& other);
	mat2x2& operator-=(const mat2x2& other);
	mat2x2& operator*=(const mat2x2& other);
	mat2x2& operator*=(const float other);
	mat2x2& operator/=(const float other);
	mat2x2 operator+(const mat2x2& other);
	mat2x2 operator-(const mat2x2& other);
	mat2x2 operator*(const mat2x2& other);
	vec2 operator*(const vec2& other);
	mat2x2 operator*(const float other);
	mat2x2 operator/(const float other);
};
class mat3x3 {
public:
	float mat[9];
	struct row {
	private: 
		float* data = nullptr;
		const float* cdata = nullptr;
		int x = 0;
	public:
		row(float* data, int x) : data(data), x(x) {}
		row(const float* cdata, int x) : cdata(cdata), x(x) {}
		float& operator[](int y) {return data[x + y * 3];}
		float operator[](int y) const {return cdata[x + y * 3];}
	};
	
	mat3x3();
	mat3x3(const float initValue);
	mat3x3(const mat3x3& other);
	mat3x3(const float mat[9]);
	mat3x3(std::initializer_list<float> list);
	mat3x3 operator=(std::initializer_list<float> list);

	void translate(float x, float y);
	void scale(float sx, float sy);
	void rotate(float angle);

	mat3x3 transpose();
	mat3x3 inverse();
	inline float determinant() {
		return mat[0] * (mat[4] * mat[8] - mat[5] * mat[7]) -
			   mat[1] * (mat[3] * mat[8] - mat[5] * mat[6]) +
			   mat[2] * (mat[3] * mat[7] - mat[4] * mat[6]);
	}
	inline float GetValue(int row, int column) {
		return mat[column + row * 3];
	}
	inline static mat3x3 Identity() {
		return mat3x3(1);
	}
	inline static mat3x3 Null() {
		return { };
	}

	row operator[](int index);
	const row operator[](int index) const;
	mat3x3& operator=(const mat3x3& other);
	mat3x3& operator+=(const mat3x3& other);
	mat3x3& operator-=(const mat3x3& other);
	mat3x3& operator*=(const mat3x3& other);
	mat3x3& operator*=(const float other);
	mat3x3& operator/=(const float other);
	mat3x3 operator+(const mat3x3& other);
	mat3x3 operator-(const mat3x3& other);
	mat3x3 operator*(const mat3x3& other);
	vec3 operator*(const vec3& other);
	mat3x3 operator*(const float other);
	mat3x3 operator/(const float other);
};
class mat4x4 {
public:
	float mat[16];
	struct row {
	private: 
		float* data = nullptr;
		const float* cdata = nullptr;
		int x = 0;
	public:
		row(float* data, int x) : data(data), x(x) {}
		row(const float* cdata, int x) : cdata(cdata), x(x) {}
		float& operator[](int y) {return data[x + y * 4];}
		float operator[](int y) const {return cdata[x + y * 4];}
	};
	
	mat4x4();
	mat4x4(const float initValue);
	mat4x4(const mat4x4& other);
	mat4x4(const float mat[16]);
	mat4x4(std::initializer_list<float> list);
	mat4x4 operator=(std::initializer_list<float> list);

	void translate(float x, float y, float z);
	void scale(float sx, float sy, float sz);
	void rotate(float angle, vec3 rotationAxis);
	
	mat4x4 transpose();
	mat4x4 inverse();
	inline float determinant() {
		return  mat[0 + 3 * 4] * mat[1 + 2 * 4] * mat[2 + 1 * 4] * mat[3 + 0 * 4] - mat[0 + 2 * 4] * mat[1 + 3 * 4] * mat[2 + 1 * 4] * mat[3 + 0 * 4] -
				mat[0 + 3 * 4] * mat[1 + 1 * 4] * mat[2 + 2 * 4] * mat[3 + 0 * 4] + mat[0 + 1 * 4] * mat[1 + 3 * 4] * mat[2 + 2 * 4] * mat[3 + 0 * 4] +
				mat[0 + 2 * 4] * mat[1 + 1 * 4] * mat[2 + 3 * 4] * mat[3 + 0 * 4] - mat[0 + 1 * 4] * mat[1 + 2 * 4] * mat[2 + 3 * 4] * mat[3 + 0 * 4] -
				mat[0 + 3 * 4] * mat[1 + 2 * 4] * mat[2 + 0 * 4] * mat[3 + 1 * 4] + mat[0 + 2 * 4] * mat[1 + 3 * 4] * mat[2 + 0 * 4] * mat[3 + 1 * 4] +
				mat[0 + 3 * 4] * mat[1 + 0 * 4] * mat[2 + 2 * 4] * mat[3 + 1 * 4] - mat[0 + 0 * 4] * mat[1 + 3 * 4] * mat[2 + 2 * 4] * mat[3 + 1 * 4] -
				mat[0 + 2 * 4] * mat[1 + 0 * 4] * mat[2 + 3 * 4] * mat[3 + 1 * 4] + mat[0 + 0 * 4] * mat[1 + 2 * 4] * mat[2 + 3 * 4] * mat[3 + 1 * 4] +
				mat[0 + 3 * 4] * mat[1 + 1 * 4] * mat[2 + 0 * 4] * mat[3 + 2 * 4] - mat[0 + 1 * 4] * mat[1 + 3 * 4] * mat[2 + 0 * 4] * mat[3 + 2 * 4] -
				mat[0 + 3 * 4] * mat[1 + 0 * 4] * mat[2 + 1 * 4] * mat[3 + 2 * 4] + mat[0 + 0 * 4] * mat[1 + 3 * 4] * mat[2 + 1 * 4] * mat[3 + 2 * 4] +
				mat[0 + 1 * 4] * mat[1 + 0 * 4] * mat[2 + 3 * 4] * mat[3 + 2 * 4] - mat[0 + 0 * 4] * mat[1 + 1 * 4] * mat[2 + 3 * 4] * mat[3 + 2 * 4] -
				mat[0 + 2 * 4] * mat[1 + 1 * 4] * mat[2 + 0 * 4] * mat[3 + 3 * 4] + mat[0 + 1 * 4] * mat[1 + 2 * 4] * mat[2 + 0 * 4] * mat[3 + 3 * 4] +
				mat[0 + 2 * 4] * mat[1 + 0 * 4] * mat[2 + 1 * 4] * mat[3 + 3 * 4] - mat[0 + 0 * 4] * mat[1 + 2 * 4] * mat[2 + 1 * 4] * mat[3 + 3 * 4] -
				mat[0 + 1 * 4] * mat[1 + 0 * 4] * mat[2 + 2 * 4] * mat[3 + 3 * 4] + mat[0 + 0 * 4] * mat[1 + 1 * 4] * mat[2 + 2 * 4] * mat[3 + 3 * 4];
	}

	inline float GetValue(int row, int column) {
		return mat[column + row * 4];
	}
	inline static mat4x4 Identity() {
		return mat4x4(1);
	}
	inline static mat4x4 Null() {
		return { };
	}

	row operator[](int index);
	const row operator[](int index) const;
	mat4x4& operator=(const mat4x4& other);
	mat4x4& operator+=(const mat4x4& other);
	mat4x4& operator-=(const mat4x4& other);
	mat4x4& operator*=(const mat4x4& other);
	mat4x4& operator*=(const float other);
	mat4x4& operator/=(const float other);
	mat4x4 operator+(const mat4x4& other);
	mat4x4 operator-(const mat4x4& other);
	mat4x4 operator*(const mat4x4& other);
	vec4 operator*(const vec4& other);
	mat4x4 operator*(const float other);
	mat4x4 operator/(const float other);
};


mat4x4 perspectiveProjection(float fov, float aspectRatio, float near, float far);
mat4x4 rotate(const mat4x4& mat, float pitch, float yaw, float roll);
mat4x4 rotate(const mat4x4& mat, float angle, vec3 axis);
vec4 rotate(const vec4& vec, float pitch, float yaw, float roll);
vec4 rotate(const vec4& vec, float angle, vec3 axis);
mat4x4 translate(const mat4x4& mat, const vec3& v);
vec4 translate(const vec4& vec, float x, float y, float z);
mat4x4 pointAt(const vec4& pos, const vec4& target, const vec4& up);
mat4x4 QuickInverse(const mat4x4& m);

inline float radians(float degree) {
	return degree * 3.141592653589793 / 180.0;
}
inline float degrees(float radian) {
	return radian * 180.0 / 3.141592653589793;
}

}

#endif /* TENSOR_MATH_HPP */
