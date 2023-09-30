#pragma once 

#include "Resources/ResourceLoader.h"

namespace Resources {
	template<>
	struct Resource<Font> {
		using Type = Font;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadFont(path.data());
			};

		static constexpr DelTy Deleter = [](Font* f) {
			assert(f && "Trying to delete a null font. Maybe it's trying to be freed twice?");
			UnloadFont(*f);
			};
	};
}