#pragma once

#include <type_traits>
#include <cmath>

namespace Utils
{
	template<typename T>
	concept VectorType = std::is_arithmetic_v<T>;

	template<VectorType T>
	struct Vector2 {
		using rep = T;

		T x, y;

		constexpr Vector2() noexcept : x(0), y(0) {}
		constexpr Vector2(T x1, T y2) noexcept : x(x1), y(y2) {}

		// Allow copying int vectors with floating point vectors... this might be dangerous but in the users we trust
		template<typename Vec>
		constexpr Vector2(const Vec& other) noexcept : x(static_cast<rep>(other.x)), y(static_cast<rep>(other.y)) {}


		constexpr Vector2<T> operator* (T i) const { return { x * i, y * i }; }
		void operator *= (T i) const { return { x *= i, y *= i }; }
		constexpr Vector2<T> operator+ (const Vector2<T>& i) const { return { x + i.x, y + i.y }; }
		void operator+= (const Vector2<T>& i) { x += i.x; y += i.y; }
		constexpr Vector2<T> operator- (const Vector2<T>& i) const { return { x - i.x, y - i.y }; }
		void operator-= (const Vector2<T>& i) { x -= i.x; y -= i.y; }
	};


	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;

	constexpr Vector2f UP{ 0,1 };
	constexpr Vector2f DOWN{ 0,-1 };
	constexpr Vector2f RIGHT{ 1,0 };
	constexpr Vector2f LEFT{ -1,0 };


	template<VectorType T>
	constexpr float VectorLengthSquared(const Vector2<T>& vec) {
		return vec.x * vec.x + vec.y * vec.y;
	}

	template<VectorType T>
	constexpr float VectorLength(const Vector2<T>& vec) {
		// this is expensive, consider using Fast inverse square root.
		return std::sqrtf(VectorLengthSquared<T>(vec));
	}


	constexpr Vector2f NormalizeVector(const Vector2f& vec)
	{
		auto length = VectorLength(vec);
		return { vec.x / length , vec.y / length };
	}
}