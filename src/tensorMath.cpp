/********************************
 * Project: Cidr                *
 * File: tensorMath.cpp         *
 * Date: 13.9.2020              *
 ********************************/

#include <iostream>
#include "tensorMath.hpp"

/* tem::vec2 ==================================================== */

tem::vec2::vec2() : x{0}, y{0} { }
tem::vec2::vec2(float x, float y) : x{x}, y{y} { }

void tem::vec2::setLength(float value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
}
void tem::vec2::addLength(float value) {
	setLength(length() + value);
}
void tem::vec2::subLength(float value) {
	setLength(length() - value);
}
tem::vec2& tem::vec2::operator=(const tem::vec2& other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}
tem::vec2& tem::vec2::operator+=(const tem::vec2& other) {
	this->x += other.x;
	this->y += other.y;
	return *this;
}
tem::vec2& tem::vec2::operator-=(const tem::vec2& other) {
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}
float tem::vec2::operator^=(const vec2& other) {
	float crossValue = this->x * other.y - other.x * this->y;
	return crossValue;
}
tem::vec2& tem::vec2::operator/=(const float other) {
	this->x /= other;
	this->y /= other;
	return *this;
}
tem::vec2 tem::vec2::operator+(const tem::vec2& other) const {
	return tem::vec2 { this->x + other.x, this->y + other.y };
}
tem::vec2 tem::vec2::operator-(const tem::vec2& other) const {
	return tem::vec2 { this->x - other.x, this->y - other.y };
}
float tem::vec2::operator*(const tem::vec2& other) const {
	return this->x * other.x + this->y * other.y;
}
tem::vec2 tem::vec2::operator*(const float other) const {
	return tem::vec2 { this->x * other, this->y * other };
}
float tem::vec2::operator^(const tem::vec2& other) const {
	return this->x * other.x + this->y * other.y;
}
tem::vec2 tem::vec2::operator/(const float other) const {
	return tem::vec2 { this->x / other, this->y / other };
}


/* tem::vec3 =================================================================== */

tem::vec3::vec3() : x{0}, y{0}, z{0} { }
tem::vec3::vec3(float x, float y, float z) : x{x}, y{y}, z{z} { }

void tem::vec3::setLength(float value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
	z = ratio * z;
}
void tem::vec3::addLength(float value) {
	setLength(length() + value);
}
void tem::vec3::subLength(float value) {
	setLength(length() - value);
}
tem::vec3& tem::vec3::operator=(const tem::vec3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}
tem::vec3& tem::vec3::operator+=(const tem::vec3& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}
tem::vec3& tem::vec3::operator-=(const tem::vec3& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}
tem::vec3& tem::vec3::operator*=(const float other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	return *this;
}
tem::vec3& tem::vec3::operator^=(const tem::vec3& other) {
	*this = *this ^ other;
	return *this;
}
tem::vec3& tem::vec3::operator/=(const float other) {
	this->x /= other;
	this->y /= other;
	this->z /= other;
	return *this;
}
tem::vec3 tem::vec3::operator+(const tem::vec3& other) const {
	return tem::vec3 { this->x + other.x, this->y + other.y, this->z + other.z };
}
tem::vec3 tem::vec3::operator-(const tem::vec3& other) const {
	return tem::vec3 { this->x - other.x, this->y - other.y, this->z - other.z };
}
tem::vec3 tem::vec3::operator*(const float other) const {
	return tem::vec3 { this->x * other, this->y * other, this->z * other };
}
float tem::vec3::operator*(const tem::vec3& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}
tem::vec3 tem::vec3::operator^(const tem::vec3& other) const {
	return { this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x };
}
tem::vec3 tem::vec3::operator/(const float other) const {
	return tem::vec3 { this->x / other, this->y / other, this->z / other };
}


/* tem::vec4 =================================================================== */

tem::vec4::vec4() : x{0}, y{0}, z{0}, w{1} { }
tem::vec4::vec4(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} { }

void tem::vec4::setLength(float value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
	z = ratio * z;
	w = ratio * w;
}
void tem::vec4::addLength(float value) {
	setLength(length() + value);
}
void tem::vec4::subLength(float value) {
	setLength(length() - value);
}
tem::vec4& tem::vec4::operator=(const tem::vec4& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
	return *this;
}
tem::vec4& tem::vec4::operator+=(const tem::vec4& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}
tem::vec4& tem::vec4::operator-=(const tem::vec4& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}
tem::vec4& tem::vec4::operator*=(const float other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	return *this;
}
tem::vec4& tem::vec4::operator^=(const tem::vec4& other) {
	*this = *this ^ other;
	return *this;
}
tem::vec4& tem::vec4::operator/=(const float other) {
	this->x /= other;
	this->y /= other;
	this->z /= other;
	return *this;
}
tem::vec4 tem::vec4::operator+(const tem::vec4& other) const {
	return tem::vec4 { this->x + other.x, this->y + other.y, this->z + other.z };
}
tem::vec4 tem::vec4::operator-(const tem::vec4& other) const {
	return tem::vec4 { this->x - other.x, this->y - other.y, this->z - other.z };
}
float tem::vec4::operator*(const tem::vec4& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}
tem::vec4 tem::vec4::operator*(const float other) const {
	return tem::vec4 { this->x * other, this->y * other, this->z * other };
}
tem::vec4 tem::vec4::operator^(const tem::vec4& other) const {
	return tem::vec4 { this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x, 1 };
}
tem::vec4 tem::vec4::operator/(const float other) const {
	return tem::vec4 { this->x / other, this->y / other, this->z / other };
}





/* tem::ivec2 ==================================================== */

tem::ivec2::ivec2() : x{0}, y{0} { }
tem::ivec2::ivec2(int x, int y) : x{x}, y{y} { }

void tem::ivec2::setLength(int value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
}
void tem::ivec2::addLength(int value) {
	setLength(length() + value);
}
void tem::ivec2::subLength(int value) {
	setLength(length() - value);
}
tem::ivec2& tem::ivec2::operator=(const tem::ivec2& other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}
tem::ivec2& tem::ivec2::operator+=(const tem::ivec2& other) {
	this->x += other.x;
	this->y += other.y;
	return *this;
}
tem::ivec2& tem::ivec2::operator-=(const tem::ivec2& other) {
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}
int tem::ivec2::operator^=(const ivec2& other) {
	int crossValue = this->x * other.y - other.x * this->y;
	return crossValue;
}
tem::ivec2& tem::ivec2::operator/=(const int other) {
	this->x /= other;
	this->y /= other;
	return *this;
}
tem::ivec2 tem::ivec2::operator+(const tem::ivec2& other) {
	return tem::ivec2 { this->x + other.x, this->y + other.y };
}
tem::ivec2 tem::ivec2::operator-(const tem::ivec2& other) {
	return tem::ivec2 { this->x - other.x, this->y - other.y };
}
int tem::ivec2::operator*(const tem::ivec2& other) {
	return this->x * other.x + this->y * other.y;
}
tem::ivec2 tem::ivec2::operator*(const int other) {
	return tem::ivec2 { this->x * other, this->y * other };
}
int tem::ivec2::operator^(const tem::ivec2& other) {
	return this->x * other.x + this->y * other.y;
}
tem::ivec2 tem::ivec2::operator/(const int other) {
	return tem::ivec2 { this->x / other, this->y / other };
}


/* tem::ivec3 =================================================================== */

tem::ivec3::ivec3() : x{0}, y{0}, z{0} { }
tem::ivec3::ivec3(int x, int y, int z) : x{x}, y{y}, z{z} { }

void tem::ivec3::setLength(int value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
	z = ratio * z;
}
void tem::ivec3::addLength(int value) {
	setLength(length() + value);
}
void tem::ivec3::subLength(int value) {
	setLength(length() - value);
}
tem::ivec3& tem::ivec3::operator=(const tem::ivec3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}
tem::ivec3& tem::ivec3::operator+=(const tem::ivec3& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}
tem::ivec3& tem::ivec3::operator-=(const tem::ivec3& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}
tem::ivec3& tem::ivec3::operator*=(const int other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	return *this;
}
tem::ivec3& tem::ivec3::operator^=(const tem::ivec3& other) {
	*this = *this ^ other;
	return *this;
}
tem::ivec3& tem::ivec3::operator/=(const int other) {
	this->x /= other;
	this->y /= other;
	this->z /= other;
	return *this;
}
tem::ivec3 tem::ivec3::operator+(const tem::ivec3& other) {
	return tem::ivec3 { this->x + other.x, this->y + other.y, this->z + other.z };
}
tem::ivec3 tem::ivec3::operator-(const tem::ivec3& other) {
	return tem::ivec3 { this->x - other.x, this->y - other.y, this->z - other.z };
}
tem::ivec3 tem::ivec3::operator*(const int other) {
	return tem::ivec3 { this->x * other, this->y * other, this->z * other };
}
int tem::ivec3::operator*(const tem::ivec3& other) {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}
tem::ivec3 tem::ivec3::operator^(const tem::ivec3& other) {
	return { this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x };
}
tem::ivec3 tem::ivec3::operator/(const int other) {
	return tem::ivec3 { this->x / other, this->y / other, this->z / other };
}


/* tem::ivec4 =================================================================== */

tem::ivec4::ivec4() : x{0}, y{0}, z{0}, w{0} { }
tem::ivec4::ivec4(int x, int y, int z, int w) : x{x}, y{y}, z{z}, w{w} { }

void tem::ivec4::setLength(int value) {
	float ratio = value / length();
	x = ratio * x;
	y = ratio * y;
	z = ratio * z;
	w = ratio * w;
}
void tem::ivec4::addLength(int value) {
	setLength(length() + value);
}
void tem::ivec4::subLength(int value) {
	setLength(length() - value);
}
tem::ivec4& tem::ivec4::operator=(const tem::ivec4& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
	return *this;
}
tem::ivec4& tem::ivec4::operator+=(const tem::ivec4& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
	return *this;
}
tem::ivec4& tem::ivec4::operator-=(const tem::ivec4& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
	return *this;
}
tem::ivec4& tem::ivec4::operator*=(const int other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	this->w *= other;
	return *this;
}
tem::ivec4& tem::ivec4::operator/=(const int other) {
	this->x /= other;
	this->y /= other;
	this->z /= other;
	this->w /= other;
	return *this;
}
tem::ivec4 tem::ivec4::operator+(const tem::ivec4& other) {
	return tem::ivec4 { this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w };
}
tem::ivec4 tem::ivec4::operator-(const tem::ivec4& other) {
	return tem::ivec4 { this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w };
}
int tem::ivec4::operator*(const tem::ivec4& other) {
	return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
}
tem::ivec4 tem::ivec4::operator*(const int other) {
	return tem::ivec4 { this->x * other, this->y * other, this->z * other, this->w * other };
}
tem::ivec4 tem::ivec4::operator/(const int other) {
	return tem::ivec4 { this->x / other, this->y / other, this->z / other, this->w / other };
}





/* tem::mat2x2 =================================================================== */

tem::mat2x2::mat2x2() : mat{ } {
}
tem::mat2x2::mat2x2(const float initValue) : mat{ }{
	for(int i = 0; i < 2; i++)
		this->mat[i+2*i] = initValue;
}
tem::mat2x2::mat2x2(const float mat[4]) {
	for(int i = 0; i < 4; i++)
		this->mat[i] = mat[i];
}
tem::mat2x2::mat2x2(const tem::mat2x2& other) {
	for(int i = 0; i < 4; i++)
		this->mat[i] = other.mat[i];
}
tem::mat2x2::mat2x2(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
}
tem::mat2x2 tem::mat2x2::operator=(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
	return *this;
}

tem::mat2x2 tem::mat2x2::transpose() {
	mat2x2 mat;
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			mat.mat[i + j * 2] = this->mat[j + i * 2];
		}
	}
	
	return mat;
}
tem::mat2x2 tem::mat2x2::inverse() {
	mat2x2 mat = mat2x2::Null();
	
	float det = this->determinant();
	if(det == 0) return mat;
	
	float invdet = 1.f / det;
	
	mat.mat[0] =  this->mat[3] * invdet;
	mat.mat[1] = -this->mat[1] * invdet;
	mat.mat[2] = -this->mat[2] * invdet;
	mat.mat[3] =  this->mat[0] * invdet;
	
	return mat;
}
tem::mat2x2::row tem::mat2x2::operator[](int index) {
	return (tem::mat2x2::row){this->mat, index};
}
const tem::mat2x2::row tem::mat2x2::operator[](int index) const {
	return (tem::mat2x2::row){this->mat, index};
}
tem::mat2x2& tem::mat2x2::operator=(const tem::mat2x2& other) {
	for(int i = 0; i < 4; i++)
		this->mat[i] = other.mat[i];
	return *this;
}
tem::mat2x2& tem::mat2x2::operator+=(const tem::mat2x2& other) {
	for(int i = 0; i < 4; i++) {
		this->mat[i] += other.mat[i];
	}
	return *this;
}
tem::mat2x2& tem::mat2x2::operator-=(const tem::mat2x2& other) {
	for(int i = 0; i < 4; i++) {
		this->mat[i] -= other.mat[i];
	}
	return *this;
}
tem::mat2x2& tem::mat2x2::operator*=(const tem::mat2x2& other) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			this->mat[i + j * 2] =
				this->mat[j * 2] * other.mat[j + i] +
				this->mat[j * 2 + 1] * other.mat[j + 2 + i];
		}
	}
	
	return *this;
}
tem::mat2x2& tem::mat2x2::operator*=(const float other) {
	for(int i = 0; i < 4; i++) {
		this->mat[i] *= other;
	}
	
	return *this;
}
tem::mat2x2& tem::mat2x2::operator/=(const float other) {
	for(int i = 0; i < 4; i++) {
		this->mat[i] /= other;
	}
	
	return *this;
}
tem::mat2x2 tem::mat2x2::operator+(const tem::mat2x2& other) {
	tem::mat2x2 temp;
	for(int i = 0; i < 4; i++) {
		temp.mat[i] = this->mat[i] + other.mat[i];
	}
	return temp;
}
tem::mat2x2 tem::mat2x2::operator-(const tem::mat2x2& other) {
	tem::mat2x2 temp;
	for(int i = 0; i < 4; i++) {
		temp.mat[i] = this->mat[i] - other.mat[i];
	}
	return temp;
}
tem::mat2x2 tem::mat2x2::operator*(const tem::mat2x2& other) {
	tem::mat2x2 temp;
	
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			temp.mat[i + j * 2] =
				this->mat[j * 2] * other.mat[i] +
				this->mat[j * 2 + 1] * other.mat[2 + i];
		}
	}
	
	return temp;
}
tem::vec2 tem::mat2x2::operator*(const tem::vec2& other) {
	return tem::vec2 {
		other.x * this->mat[0] + other.y * this->mat[1],
		other.x * this->mat[2] + other.y * this->mat[3]
	};
}
tem::mat2x2 tem::mat2x2::operator*(const float other) {
	tem::mat2x2 temp = tem::mat2x2(*this);
	
	for(int i = 0; i < 4; i++) {
		temp.mat[i] *= 2;
	}
	
	return temp;
}
tem::mat2x2 tem::mat2x2::operator/(const float other) {
	tem::mat2x2 temp = tem::mat2x2(*this);
	
	for(int i = 0; i < 4; i++) {
		temp.mat[i] /= 2;
	}
	
	return temp;
}


/* mat3x3 =================================================================== */

tem::mat3x3::mat3x3() : mat{ } {
}
tem::mat3x3::mat3x3(const float initValue) : mat{ }{
	for(int i = 0; i < 3; i++)
		this->mat[i+3*i] = initValue;
}
tem::mat3x3::mat3x3(const float mat[9]) {
	for(int i = 0; i < 9; i++)
		this->mat[i] = mat[i];
}
tem::mat3x3::mat3x3(const tem::mat3x3& other) {
	for(int i = 0; i < 9; i++)
		this->mat[i] = other.mat[i];
}
tem::mat3x3::mat3x3(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
}
tem::mat3x3 tem::mat3x3::operator=(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
	return *this;
}

void tem::mat3x3::translate(float x, float y) {
	this->mat[2] = x;
	this->mat[5] = y;
}
void tem::mat3x3::scale(float sx, float sy) {
	this->mat[0] = sx;
	this->mat[4] = sy;
}
void tem::mat3x3::rotate(float angle) {
	this->mat[0] = cosf(angle);
	this->mat[1] = -sinf(angle);
	this->mat[3] = sinf(angle);
	this->mat[4] = cosf(angle);
}
tem::mat3x3 tem::mat3x3::transpose() {
	mat3x3 mat;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mat.mat[i + j * 3] = this->mat[j + i * 3];
		}
	}
	
	return mat;
}
tem::mat3x3 tem::mat3x3::inverse() {
	mat3x3 mat = mat3x3::Null();
	
	float det = this->determinant();
	if(det == 0) return mat;
	
	float invdet = 1.f / det;
	
	mat.mat[0 + 0 * 3] = (this->mat[1 + 1 * 3] * this->mat[2 + 2 * 3] - this->mat[2 + 1 * 3] * this->mat[1 + 2 * 3]) * invdet;
	mat.mat[0 + 1 * 3] = (this->mat[0 + 2 * 3] * this->mat[2 + 1 * 3] - this->mat[0 + 1 * 3] * this->mat[2 + 2 * 3]) * invdet;
	mat.mat[0 + 2 * 3] = (this->mat[0 + 1 * 3] * this->mat[1 + 2 * 3] - this->mat[0 + 2 * 3] * this->mat[1 + 1 * 3]) * invdet;
	mat.mat[1 + 0 * 3] = (this->mat[1 + 2 * 3] * this->mat[2 + 0 * 3] - this->mat[1 + 0 * 3] * this->mat[2 + 2 * 3]) * invdet;
	mat.mat[1 + 1 * 3] = (this->mat[0 + 0 * 3] * this->mat[2 + 2 * 3] - this->mat[0 + 2 * 3] * this->mat[2 + 0 * 3]) * invdet;
	mat.mat[1 + 2 * 3] = (this->mat[1 + 0 * 3] * this->mat[0 + 2 * 3] - this->mat[0 + 0 * 3] * this->mat[1 + 2 * 3]) * invdet;
	mat.mat[2 + 0 * 3] = (this->mat[1 + 0 * 3] * this->mat[2 + 1 * 3] - this->mat[2 + 0 * 3] * this->mat[1 + 1 * 3]) * invdet;
	mat.mat[2 + 1 * 3] = (this->mat[2 + 0 * 3] * this->mat[0 + 1 * 3] - this->mat[0 + 0 * 3] * this->mat[2 + 1 * 3]) * invdet;
	mat.mat[2 + 2 * 3] = (this->mat[0 + 0 * 3] * this->mat[1 + 1 * 3] - this->mat[1 + 0 * 3] * this->mat[0 + 1 * 3]) * invdet;
	
	
	return mat;
}
tem::mat3x3::row tem::mat3x3::operator[](int index) {
	return (tem::mat3x3::row){this->mat, index};
}
const tem::mat3x3::row tem::mat3x3::operator[](int index) const {
	return (tem::mat3x3::row){this->mat, index};
}
tem::mat3x3& tem::mat3x3::operator=(const tem::mat3x3& other) {
	for(int i = 0; i < 9; i++)
		this->mat[i] = other.mat[i];
	return *this;
}
tem::mat3x3& tem::mat3x3::operator+=(const tem::mat3x3& other) {
	for(int i = 0; i < 9; i++) {
		this->mat[i] += other.mat[i];
	}
	return *this;
}
tem::mat3x3& tem::mat3x3::operator-=(const tem::mat3x3& other) {
	for(int i = 0; i < 9; i++) {
		this->mat[i] -= other.mat[i];
	}
	return *this;
}
tem::mat3x3& tem::mat3x3::operator*=(const tem::mat3x3& other) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			this->mat[i + j * 3] =
				this->mat[j * 3] * other.mat[i] +
				this->mat[j * 3 + 1] * other.mat[3 + i] +
				this->mat[j * 3 + 2] * other.mat[6 + i];
		}
	}
	return *this;
}
tem::mat3x3& tem::mat3x3::operator*=(const float other) {
	for(int i = 0; i < 9; i++) {
		this->mat[i] *= other;
	}
	
	return *this;
}
tem::mat3x3& tem::mat3x3::operator/=(const float other) {
	for(int i = 0; i < 9; i++) {
		this->mat[i] /= other;
	}
	
	return *this;
}
tem::mat3x3 tem::mat3x3::operator+(const tem::mat3x3& other) {
	tem::mat3x3 temp;
	for(int i = 0; i < 9; i++) {
		temp.mat[i] = this->mat[i] + other.mat[i];
	}
	return temp;
}
tem::mat3x3 tem::mat3x3::operator-(const tem::mat3x3& other) {
	tem::mat3x3 temp;
	for(int i = 0; i < 9; i++) {
		temp.mat[i] = this->mat[i] - other.mat[i];
	}
	return temp;
}
tem::mat3x3 tem::mat3x3::operator*(const tem::mat3x3& other) {
	tem::mat3x3 temp;
	
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			temp.mat[i + j * 3] =
				this->mat[j * 3] * other.mat[i] +
				this->mat[j * 3 + 1] * other.mat[3 + i] +
				this->mat[j * 3 + 2] * other.mat[6 + i];
		}
	}
	
	return temp;
}
tem::vec3 tem::mat3x3::operator*(const tem::vec3& other) {
	return tem::vec3 {
		other.x * this->mat[0] + other.y * this->mat[1] + other.z * this->mat[2],
		other.x * this->mat[3] + other.y * this->mat[4] + other.z * this->mat[5],
		other.x * this->mat[6] + other.y * this->mat[7] + other.z * this->mat[8]
	};
}
tem::mat3x3 tem::mat3x3::operator*(const float other) {
	tem::mat3x3 temp = tem::mat3x3(*this);
	
	for(int i = 0; i < 4; i++) {
		temp.mat[i] *= other;
	}
	
	return temp;
}
tem::mat3x3 tem::mat3x3::operator/(const float other) {
	tem::mat3x3 temp = tem::mat3x3(*this);
	
	for(int i = 0; i < 4; i++) {
		temp.mat[i] /= other;
	}
	
	return temp;
}


/* tem::mat4x4 =================================================================== */

tem::mat4x4::mat4x4() : mat{ }{
}
tem::mat4x4::mat4x4(const float initValue) : mat{ } {
	for(int i = 0; i < 4; i++) {
		this->mat[i+4*i] = initValue;
	}
}
tem::mat4x4::mat4x4(const float mat[16]) {
	for(int i = 0; i < 16; i++)
		this->mat[i] = mat[i];
}
tem::mat4x4::mat4x4(const tem::mat4x4& other) {
	for(int i = 0; i < 16; i++)
		this->mat[i] = other.mat[i];
}
tem::mat4x4::mat4x4(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
}
tem::mat4x4 tem::mat4x4::operator=(std::initializer_list<float> list) {
	int counter{0};
	for (auto element : list) {
		mat[counter] = element;
		++counter;
	}
	return *this;
}

void tem::mat4x4::translate(float x, float y, float z) {
	this->mat[3] = x;
	this->mat[7] = y;
	this->mat[11] = z;
}
void tem::mat4x4::scale(float sx, float sy, float sz) {
	this->mat[0] = sx;
	this->mat[5] = sy;
	this->mat[10] = sz;
}
void tem::mat4x4::rotate(float angle, vec3 rotationAxis) {
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	this->mat[0] = cosAngle + rotationAxis.x * rotationAxis.x * (1 - cosAngle);
	this->mat[1] = rotationAxis.x * rotationAxis.y * (1 - cosAngle) - rotationAxis.z * sinAngle;
	this->mat[2] = rotationAxis.x * rotationAxis.z * (1 - cosAngle) + rotationAxis.y * sinAngle;
	this->mat[4] = rotationAxis.y * rotationAxis.x * (1 - cosAngle) + rotationAxis.z * sinAngle;
	this->mat[5] = cosAngle + rotationAxis.y * rotationAxis.y * (1 - cosAngle);
	this->mat[6] = rotationAxis.y * rotationAxis.z * (1 - cosAngle) - rotationAxis.x * sinAngle;
	this->mat[8] = rotationAxis.z * rotationAxis.x * (1 - cosAngle) - rotationAxis.y * sinAngle;
	this->mat[9] = rotationAxis.z * rotationAxis.y * (1 - cosAngle) + rotationAxis.x * sinAngle;
	this->mat[10] = cosAngle + rotationAxis.z * rotationAxis.z * (1 - cosAngle);
}
tem::mat4x4 tem::mat4x4::transpose() {
	mat4x4 mat;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat.mat[i + j * 4] = this->mat[j + i * 4];
		}
	}
	
	return mat;
}
tem::mat4x4 tem::mat4x4::inverse() {
	mat4x4 mat = mat4x4::Null();
	
	float det = this->determinant();
	if(det == 0) return mat;
	
	float invdet = 1.f / det;
	
	mat.mat[0] = (this->mat[5]  * this->mat[10] * this->mat[15] -
			 this->mat[5]  * this->mat[11] * this->mat[14] -
			 this->mat[9]  * this->mat[6]  * this->mat[15] +
			 this->mat[9]  * this->mat[7]  * this->mat[14] +
			 this->mat[13] * this->mat[6]  * this->mat[11] -
			 this->mat[13] * this->mat[7]  * this->mat[10]) * invdet;

	mat.mat[4] = (-this->mat[4]  * this->mat[10] * this->mat[15] +
			  this->mat[4]  * this->mat[11] * this->mat[14] +
			  this->mat[8]  * this->mat[6]  * this->mat[15] -
			  this->mat[8]  * this->mat[7]  * this->mat[14] -
			  this->mat[12] * this->mat[6]  * this->mat[11] +
			  this->mat[12] * this->mat[7]  * this->mat[10]) * invdet;

	mat.mat[8] = (this->mat[4]  * this->mat[9] * this->mat[15] -
			 this->mat[4]  * this->mat[11] * this->mat[13] -
			 this->mat[8]  * this->mat[5] * this->mat[15] +
			 this->mat[8]  * this->mat[7] * this->mat[13] +
			 this->mat[12] * this->mat[5] * this->mat[11] -
			 this->mat[12] * this->mat[7] * this->mat[9]) * invdet;

	mat.mat[12] = (-this->mat[4]  * this->mat[9] * this->mat[14] +
			   this->mat[4]  * this->mat[10] * this->mat[13] +
			   this->mat[8]  * this->mat[5] * this->mat[14] -
			   this->mat[8]  * this->mat[6] * this->mat[13] -
			   this->mat[12] * this->mat[5] * this->mat[10] +
			   this->mat[12] * this->mat[6] * this->mat[9]) * invdet;

	mat.mat[1] = (-this->mat[1]  * this->mat[10] * this->mat[15] +
			  this->mat[1]  * this->mat[11] * this->mat[14] +
			  this->mat[9]  * this->mat[2] * this->mat[15] -
			  this->mat[9]  * this->mat[3] * this->mat[14] -
			  this->mat[13] * this->mat[2] * this->mat[11] +
			  this->mat[13] * this->mat[3] * this->mat[10]) * invdet;

	mat.mat[5] = (this->mat[0]  * this->mat[10] * this->mat[15] -
			 this->mat[0]  * this->mat[11] * this->mat[14] -
			 this->mat[8]  * this->mat[2] * this->mat[15] +
			 this->mat[8]  * this->mat[3] * this->mat[14] +
			 this->mat[12] * this->mat[2] * this->mat[11] -
			 this->mat[12] * this->mat[3] * this->mat[10]) * invdet;

	mat.mat[9] = (-this->mat[0]  * this->mat[9] * this->mat[15] +
			  this->mat[0]  * this->mat[11] * this->mat[13] +
			  this->mat[8]  * this->mat[1] * this->mat[15] -
			  this->mat[8]  * this->mat[3] * this->mat[13] -
			  this->mat[12] * this->mat[1] * this->mat[11] +
			  this->mat[12] * this->mat[3] * this->mat[9]) * invdet;

	mat.mat[13] = (this->mat[0]  * this->mat[9] * this->mat[14] -
			  this->mat[0]  * this->mat[10] * this->mat[13] -
			  this->mat[8]  * this->mat[1] * this->mat[14] +
			  this->mat[8]  * this->mat[2] * this->mat[13] +
			  this->mat[12] * this->mat[1] * this->mat[10] -
			  this->mat[12] * this->mat[2] * this->mat[9]) * invdet;

	mat.mat[2] = (this->mat[1]  * this->mat[6] * this->mat[15] -
			 this->mat[1]  * this->mat[7] * this->mat[14] -
			 this->mat[5]  * this->mat[2] * this->mat[15] +
			 this->mat[5]  * this->mat[3] * this->mat[14] +
			 this->mat[13] * this->mat[2] * this->mat[7] -
			 this->mat[13] * this->mat[3] * this->mat[6]) * invdet;

	mat.mat[6] = (-this->mat[0]  * this->mat[6] * this->mat[15] +
			  this->mat[0]  * this->mat[7] * this->mat[14] +
			  this->mat[4]  * this->mat[2] * this->mat[15] -
			  this->mat[4]  * this->mat[3] * this->mat[14] -
			  this->mat[12] * this->mat[2] * this->mat[7] +
			  this->mat[12] * this->mat[3] * this->mat[6]) * invdet;

	mat.mat[10] = (this->mat[0]  * this->mat[5] * this->mat[15] -
			  this->mat[0]  * this->mat[7] * this->mat[13] -
			  this->mat[4]  * this->mat[1] * this->mat[15] +
			  this->mat[4]  * this->mat[3] * this->mat[13] +
			  this->mat[12] * this->mat[1] * this->mat[7] -
			  this->mat[12] * this->mat[3] * this->mat[5]) * invdet;

	mat.mat[14] = (-this->mat[0]  * this->mat[5] * this->mat[14] +
			   this->mat[0]  * this->mat[6] * this->mat[13] +
			   this->mat[4]  * this->mat[1] * this->mat[14] -
			   this->mat[4]  * this->mat[2] * this->mat[13] -
			   this->mat[12] * this->mat[1] * this->mat[6] +
			   this->mat[12] * this->mat[2] * this->mat[5]) * invdet;

	mat.mat[3] = (-this->mat[1] * this->mat[6] * this->mat[11] +
			  this->mat[1] * this->mat[7] * this->mat[10] +
			  this->mat[5] * this->mat[2] * this->mat[11] -
			  this->mat[5] * this->mat[3] * this->mat[10] -
			  this->mat[9] * this->mat[2] * this->mat[7] +
			  this->mat[9] * this->mat[3] * this->mat[6]) * invdet;

	mat.mat[7] = (this->mat[0] * this->mat[6] * this->mat[11] -
			 this->mat[0] * this->mat[7] * this->mat[10] -
			 this->mat[4] * this->mat[2] * this->mat[11] +
			 this->mat[4] * this->mat[3] * this->mat[10] +
			 this->mat[8] * this->mat[2] * this->mat[7] -
			 this->mat[8] * this->mat[3] * this->mat[6]) * invdet;

	mat.mat[11] = (-this->mat[0] * this->mat[5] * this->mat[11] +
			   this->mat[0] * this->mat[7] * this->mat[9] +
			   this->mat[4] * this->mat[1] * this->mat[11] -
			   this->mat[4] * this->mat[3] * this->mat[9] -
			   this->mat[8] * this->mat[1] * this->mat[7] +
			   this->mat[8] * this->mat[3] * this->mat[5]) * invdet;

	mat.mat[15] = (this->mat[0] * this->mat[5] * this->mat[10] -
			  this->mat[0] * this->mat[6] * this->mat[9] -
			  this->mat[4] * this->mat[1] * this->mat[10] +
			  this->mat[4] * this->mat[2] * this->mat[9] +
			  this->mat[8] * this->mat[1] * this->mat[6] -
			  this->mat[8] * this->mat[2] * this->mat[5]) * invdet;
	
	return mat;
}
tem::mat4x4::row tem::mat4x4::operator[](int index) {
	return (tem::mat4x4::row){this->mat, index};
}
const tem::mat4x4::row tem::mat4x4::operator[](int index) const {
	return (tem::mat4x4::row){this->mat, index};
}
tem::mat4x4& tem::mat4x4::operator=(const tem::mat4x4& other) {
	for(int i = 0; i < 16; i++)
		this->mat[i] = other.mat[i];
	return *this;
}
tem::mat4x4& tem::mat4x4::operator+=(const tem::mat4x4& other) {
	for(int i = 0; i < 16; i++) {
		this->mat[i] += other.mat[i];
	}
	return *this;
}
tem::mat4x4& tem::mat4x4::operator-=(const tem::mat4x4& other) {
	for(int i = 0; i < 16; i++) {
		this->mat[i] -= other.mat[i];
	}
	return *this;
}
tem::mat4x4& tem::mat4x4::operator*=(const tem::mat4x4& other) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			this->mat[i + j * 4] =
				this->mat[j * 4] * other.mat[i] +
				this->mat[j * 4 + 1] * other.mat[4 + i] +
				this->mat[j * 4 + 2] * other.mat[8 + i] +
				this->mat[j * 4 + 3] * other.mat[12 + i];
		}
	}
	return *this;
}
tem::mat4x4& tem::mat4x4::operator*=(const float other) {
	for(int i = 0; i < 16; i++) {
		this->mat[i] *= other;
	}
	
	return *this;
}
tem::mat4x4& tem::mat4x4::operator/=(const float other) {
	for(int i = 0; i < 16; i++) {
		this->mat[i] /= other;
	}
	
	return *this;
}
tem::mat4x4 tem::mat4x4::operator+(const tem::mat4x4& other) {
	tem::mat4x4 temp;
	for(int i = 0; i < 16; i++) {
		temp.mat[i] = this->mat[i] + other.mat[i];
	}
	return temp;
}
tem::mat4x4 tem::mat4x4::operator-(const tem::mat4x4& other) {
	tem::mat4x4 temp;
	for(int i = 0; i < 16; i++) {
		temp.mat[i] = this->mat[i] - other.mat[i];
	}
	return temp;
}
tem::mat4x4 tem::mat4x4::operator*(const tem::mat4x4& other) {
	tem::mat4x4 temp;
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp.mat[i + j * 4] =
				this->mat[j * 4 + 0] * other.mat[ 0 + i] +
				this->mat[j * 4 + 1] * other.mat[ 4 + i] +
				this->mat[j * 4 + 2] * other.mat[ 8 + i] +
				this->mat[j * 4 + 3] * other.mat[12 + i];
		}
	}
	
	return temp;
}
tem::vec4 tem::mat4x4::operator*(const tem::vec4& other) {
	return tem::vec4 {
		other.x * this->mat[0] + other.y * this->mat[1] + other.z * this->mat[2] + other.w * this->mat[3],
		other.x * this->mat[4] + other.y * this->mat[5] + other.z * this->mat[6] + other.w * this->mat[7],
		other.x * this->mat[8] + other.y * this->mat[9] + other.z * this->mat[10] + other.w * this->mat[11],
		other.x * this->mat[12] + other.y * this->mat[13] + other.z * this->mat[14] + other.w * this->mat[15],
	};
}
tem::mat4x4 tem::mat4x4::operator*(const float other) {
	tem::mat4x4 temp = tem::mat4x4(*this);
	
	for(int i = 0; i < 16; i++) {
		temp.mat[i] *= other;
	}
	
	return temp;
}
tem::mat4x4 tem::mat4x4::operator/(const float other) {
	tem::mat4x4 temp = tem::mat4x4(*this);
	
	for(int i = 0; i < 16; i++) {
		temp.mat[i] /= other;
	}
	
	return temp;
}

tem::mat4x4 tem::perspectiveProjection(float fov, float aspectRatio, float near, float far) {
	float fovRad = 1.f / tan(fov * 0.5 / 180 * M_PI);
	tem::mat4x4 m = tem::mat4x4::Identity();
	
	m[0][0] = aspectRatio * fovRad;
	m[1][1] = fovRad;
	m[2][2] = far / (far - near);
	m[3][2] = (-far * near) / (far - near);
	m[2][3] = 1;
	m[3][3] = 0;
	
	return m;
}

tem::mat4x4 tem::rotate(const tem::mat4x4& mat, float pitch, float yaw, float roll) {
	const float cg = cos(pitch);
	const float sg = sin(pitch);
	const float cb = cos(yaw);
	const float sb = sin(yaw);
	const float ca = cos(roll);
	const float sa = sin(roll);
	
	
	tem::mat4x4 p {
		1,  0,   0, 0, 
		0, cg, -sg, 0, 
		0, sg,  cg, 0,
		0,  0,   0, 1, 
	};
	tem::mat4x4 y { 
		cb, 0, sb, 0, 
		0, 1, 0, 0, 
		-sb, 0, cb, 0, 
		0, 0, 0, 1 
	};
	tem::mat4x4 r {
		ca, -sa, 0, 0,
		sa, ca, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	
	tem::mat4x4 m = p * y * r;
	// tem::mat4x4 m {
	// 	ca*cb, ca*sb*sg - sa*cg, ca*sb*cg + sa*sg, 0,
	// 	sa*cb, sa*sb*sg + ca*cg, sa*sb*cg - ca*sg, 0,
	// 	-sb,   cb*sg,            cb*cg,            0,
	// 	0,     0,                0,                1
	// };
	
	return m * mat;
}
tem::mat4x4 tem::rotate(const tem::mat4x4& mat, float angle, tem::vec3 axis) {
	axis.normalize();
	const float ca = cos(angle);
	const float sa = sin(angle);
	
	const float &rx = axis.x, ry = axis.y, rz = axis.z;
	
	mat4x4 m {
		ca+rx*rx*(1-ca), rx*ry*(1-ca)-rz*sa, rx*rz*(1-ca)+ry*sa, 0,
		ry*rx*(1-ca)+rz*sa, ca+ry*ry*(1-ca), ry*rz*(1-ca)-rx*sa, 0,
		rz*rx*(1-ca)-ry*sa, rz*ry*(1-ca)+rx*sa, ca+rz*rz*(1-ca), 0,
		0, 0, 0, 1
	};
	
	// tem::mat4x4 m {
	// 	ca*cb, ca*sb*sg - sa*cg, ca*sb*cg + sa*sg, 0,
	// 	sa*cb, sa*sb*sg + ca*cg, sa*sb*cg - ca*sg, 0,
	// 	-sb,   cb*sg,            cb*cg,            0,
	// 	0,     0,                0,                1
	// };
	
	return m * mat;
}
tem::vec4 tem::rotate(const tem::vec4& vec, float pitch, float yaw, float roll) {
	return rotate(tem::mat4x4{1}, pitch, yaw, roll) * vec;
}
tem::vec4 tem::rotate(const tem::vec4& vec, float angle, tem::vec3 axis) {
	return rotate(tem::mat4x4{1}, angle, axis) * vec;
}

tem::mat4x4 tem::translate(const tem::mat4x4& mat, const vec3& v) {
	mat4x4 m {
		1, 0, 0, v.x,
		0, 1, 0, v.y,
		0, 0, 1, v.z,
		0, 0, 0, 1,
	};
	
	return m * mat;
}
tem::vec4 tem::translate(const tem::vec4& vec, float x, float y, float z) {
	return {
		vec.x + x,
		vec.y + y,
		vec.z + z,
		vec.w
	};
}
tem::mat4x4 tem::pointAt(const tem::vec4& pos, const tem::vec4& target, const tem::vec4& up) {
	vec4 newForward = target - pos;
	newForward.normalize();	
	
	vec4 a = newForward * (up * newForward);
	vec4 newUp = up - a;
	newUp.normalize();
	
	vec4 newRight = newUp ^ newForward;
	
	mat4x4 matrix {
		newRight.x, 	newUp.x,	newForward.x,	pos.x,
		newRight.y, 	newUp.y,	newForward.y,	pos.y,
		newRight.z, 	newUp.z,	newForward.z,	pos.z,
		0,				0,				0,				1
	};
	return matrix;
}

tem::mat4x4 tem::QuickInverse(const tem::mat4x4& m) {
	mat4x4 matrix;
	matrix[0][0] =   m[0][0];  matrix[0][1] = m[1][0];  matrix[0][2] = m[2][0];  matrix[0][3] = 0.0f;
	matrix[1][0] =   m[0][1];  matrix[1][1] = m[1][1];  matrix[1][2] = m[2][1];  matrix[1][3] = 0.0f;
	matrix[2][0] =   m[0][2];  matrix[2][1] = m[1][2];  matrix[2][2] = m[2][2];  matrix[2][3] = 0.0f;
	matrix[3][0] = -(m[3][0] * matrix[0][0] + m[3][1] * matrix[1][0] + m[3][2] * matrix[2][0]);
	matrix[3][1] = -(m[3][0] * matrix[0][1] + m[3][1] * matrix[1][1] + m[3][2] * matrix[2][1]);
	matrix[3][2] = -(m[3][0] * matrix[0][2] + m[3][1] * matrix[1][2] + m[3][2] * matrix[2][2]);
	matrix[3][3] = 1.0f;
	return matrix;
}
