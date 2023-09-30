#pragma once

#include <memory>
#include <string_view>
#include "raylib.h"
#include "Utils/Handlers.h"
#include <assert.h>
namespace Resources {


	template<typename T>
	struct Resource {
		using Type = T;
		using DelTy = std::default_delete<T>;
	};

	template<>
	struct Resource<Music> {
		using Type = Music;
		using DelTy = void(*)(Type*);

		using LoadTy = Type(*)(std::string_view);

		static constexpr LoadTy Loader = [](std::string_view path) -> Type {
			return LoadMusicStream(path.data());
			};

		static constexpr DelTy deleter = [](Type* f) {
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

		static constexpr DelTy deleter = [](Type* f) {
			assert(f && "Trying to delete a null Music. Maybe it's trying to be freed twice?");
			UnloadSound(*f);
			};
	};

	template <class T>
	concept DefaultResource = std::is_same_v<typename Resource<T>::DelTy, std::default_delete<T>>;

	class Loader {
	public:
		template<DefaultResource T>
		static Utils::Handle< typename Resource<T>::Type>Load(std::string_view _)
		{
			// Not sure how to load unkown resources, but will just create one by default;

			return std::make_unique<typename Resource<T>::Type>();
		}

		template <typename T>
		static Utils::Handle< typename Resource<T>::Type, typename Resource<T>::DelTy>Load(std::string_view path)
		{
			Resource<T>::Type* res = new Resource<T>::Type(Resource<T>::Loader(path));
			return { res, Resource<T>::Deleter };
		}
	};

}