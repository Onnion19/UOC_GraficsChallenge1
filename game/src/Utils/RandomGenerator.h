#pragma once

#include <random>
#include <type_traits>
#include <cassert>
namespace Utils {
	struct RandomGenerator {
		template<typename T>
		static T GenerateRandom(T min, T max)
		{
			static_assert(std::is_arithmetic_v<T>, "Can't generate a non arithmetic type");
			if constexpr (std::is_floating_point_v<T>) {
				std::uniform_real_distribution distribution(min, max);
				return distribution(randomGenerator);

			}
			
			if constexpr (std::is_integral_v<T>)
			{
				std::uniform_int_distribution distribution(min, max);
				return distribution(randomGenerator);
			}

			assert(false);
			return {};
		}
	private:
		static inline std::mt19937 randomGenerator{ std::random_device{}()};
	};
}