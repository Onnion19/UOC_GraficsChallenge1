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
		using LoadTy = void;
		using HandleTy = Utils::Handle<Type, DelTy>;
	};

	template <class T>
	concept DefaultConstructed = std::is_same_v<typename Resource<T>::LoadTy, void>;

	template <class T>
	concept DefaultDestroyed = std::is_same_v<typename Resource<T>::LoadTy, std::default_delete<T>>;

	class Loader {
	public:

		template<typename T, typename ... Args>
		static Resource<T>::HandleTy Load(Args&& ... args)
		{
			typename Resource<T>::Type* res = MakeObject<T>(std::forward<Args>(args)...);
			return MakeHandle<T>(res);
		}

	private: 
		template<DefaultConstructed T, typename ... Args>
		static Resource<T>::Type* MakeObject(Args&& ... args)
		{
			return new Resource<T>::Type(std::forward<Args>(args)...);
		}

		template<typename T, typename ... Args>
		static Resource<T>::Type* MakeObject(Args&& ... args)
		{
			return new Resource<T>::Type(Resource<T>::Loader(std::forward<Args>(args)...));
		}

		template<DefaultDestroyed T>
		static Resource<T>::HandleTy MakeHandle(Resource<T>::Type* ptr)
		{
			return std::make_unique<Resource<T>::Type>(ptr);
		}

		template<typename T>
		static Resource<T>::HandleTy MakeHandle(Resource<T>::Type* ptr)
		{
			return { ptr, Resource<T>::Deleter };
		}

	};

}