#pragma once 

#include <string_view>
#include <assert.h>
#include "raylib.h"

namespace Resources {
	using Font = ::Font;

	/*
		Definition of resource Font specifying the types and loaders / deleters
	*/
	template<>
	struct Resource<Font> {
		using Type = Font;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(const std::string_view &);

		// Loader function called to generate a new resource of type Font.
		static constexpr LoadTy Loader = [](const std::string_view& path) -> Type {
			return LoadFont(path.data());
			};

		// Deleter function called when the resource is no longer alive.
		static constexpr DelTy Deleter = [](Font* f) -> void {
			assert(f && "Trying to delete a null font. Maybe it's trying to be freed twice?");
			UnloadFont(*f);
		};
	};
}