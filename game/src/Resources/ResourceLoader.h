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
		// Type to crate as the resource (Usually the same as the resource)
		using Type = T;
		// Type of the deleter, used to delete the object when out of scope.
		using DelTy = std::default_delete<T>;
		// Specific loader, by default it's void meaning it will use new() statement.
		using LoadTy = void;

		/**
		* In case of using custom loader and/or deleter, it must implement the lambda defining the functors to use.
		*/
		// static constexpr LoadTy Loader = [](){}
		// static constexpr DelTy Deleter = [](){}
	};

	template <class T>
	concept DefaultConstructed = std::is_same_v<typename Resource<T>::LoadTy, void>;

	template <class T>
	concept DefaultDestroyed = std::is_same_v<typename Resource<T>::LoadTy, std::default_delete<T>>;

	/**
	*
	* Loader can load any object type as long as the type_traits of the given object are valid.
	*
	* By default the type traits will create an object using the new() and delete using std::default_delete.
	*
	* If the type requires specific loader or deleter, it must define it's own type_traits
	*
	*/

	class Loader {
	public:

		template<typename T, typename ... Args>
		static auto Load(Args&& ... args)
		{
			typename Resource<T>::Type* res = MakeObject<T>(std::forward<Args>(args)...);
			return MakeHandle<T>(res);
		}

	private:
		// Object Generators
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

		//Handle Generators

		template<DefaultDestroyed T>
		static Utils::ResourceHandle<typename Resource<T>::Type> MakeHandle(Resource<T>::Type* ptr)
		{
			return std::make_shared<Resource<T>::Type>(ptr);
		}

		template<typename T>
		static Utils::ResourceHandle<typename Resource<T>::Type> MakeHandle(Resource<T>::Type* ptr)
		{
			return { ptr, Resource<T>::Deleter };
		}

	};

}