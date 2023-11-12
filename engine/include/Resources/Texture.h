#pragma once 

#include "ResourceManager.h"
#include <cassert>
#include "raylib.h"

namespace Resources {
	using Texture = ::Texture2D;
	using Image = ::Image;

	template<>
	struct Resource<Texture> {
		using Type = Texture;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadTexture(path.data());
			};

		static constexpr DelTy Deleter = [](Type* f) {
			assert(f && "Trying to delete a null texture. Maybe it's trying to be freed twice?");
			UnloadTexture(*f);
			};
	};

	template<>
	struct Resource<Image> {
		using Type = Image;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadImage(path.data());
			};

		static constexpr DelTy Deleter = [](Type* f) {
			assert(f && "Trying to delete a null image. Maybe it's trying to be freed twice?");
			UnloadImage(*f);
			};
	};

}