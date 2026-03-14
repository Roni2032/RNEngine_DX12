#pragma once

namespace RNEngine {
	struct Color : public XMFLOAT4 {
		const static Color White;
		const static Color Red;
		const static Color Blue;
		const static Color Black;
		const static Color Green;

		Color();
		Color(float value);
		Color(float x, float y, float z, float w);
		Color(const Color& other);
		Color(const array<float, 4>& other);

		Color(const XMVECTOR& other);
		operator XMVECTOR() const;
		Color& operator=(const XMVECTOR& other);
		Color& operator=(const Color& other);
		Color operator+(const Color& other);
		Color operator-(const Color& other);
		Color operator*(const Color& other);
		Color operator*(const float& other);
		Color operator/(const Color& other);
		Color operator/(const float& other);
		bool operator==(const Color& other);
		bool operator!=(const Color& other);

		void Set(float x, float y, float z, float w);
		void Set(float value);

		void RandomVec(float min, float max);
	};
}


