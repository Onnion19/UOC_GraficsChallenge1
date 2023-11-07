#pragma once 
#include <cassert>
#include "raylib.h"


namespace Resources {
	template<>
	struct Resource<Music> {
		using Type = Music;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadMusicStream(path.data());
			};

		static constexpr DelTy Deleter = [](Type* f) {
			assert(f && "Trying to delete a null Music. Maybe it's trying to be freed twice?");
			UnloadMusicStream(*f);
			};
	};

	template<>
	struct Resource<Sound> {
		using Type = Sound;
		using DelTy = void(*)(Type*);
		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadSound(path.data());
			};

		static constexpr DelTy Deleter = [](Type* f) {
			assert(f && "Trying to delete a null Music. Maybe it's trying to be freed twice?");
			UnloadSound(*f);
			};
	};

}