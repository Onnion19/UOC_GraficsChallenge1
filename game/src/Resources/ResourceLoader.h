#pragma once

#include <memory>
#include <string_view>
#include <functional>
#include "Utils/Handlers.h"

namespace Resources {

	/*
		Resource Type trait which defines: 
			- Type as resource basic type (Font, Music, string, ...)
			- DelTy as the type of the deleter functor, called when the resource is no longer alive. By default: std::default_delete<T>
			- LoadTy [optional] as the loader type functor called to generate the resource (ie, LoadFont, LoadSound...).
	*/
	template<typename T>
	struct Resource {
		using Type = T;
		using DelTy = std::default_delete<T>;
		using HandleTy = Utils::Handle<Type, DelTy>;
	};

	template <class T>
	concept DefaultResource = std::is_same_v<typename Resource<T>::DelTy, std::default_delete<T>>;

	class Loader {
	public:

		template<DefaultResource T, typename ... Args>
		static Resource<T>::HandleTy Load(Args&& ... args)
		{
			return std::make_unique<typename Resource<T>::Type>(std::forward<Args>(args)...);
		}

		template <typename T, typename ... Args>
		static Resource<T>::HandleTy Load(Args&& ... args)
		{
			typename Resource<T>::Type* res = new Resource<T>::Type(Resource<T>::Loader(std::forward<Args>(args)...));
			return { res, Resource<T>::Deleter };
		}

	};

}