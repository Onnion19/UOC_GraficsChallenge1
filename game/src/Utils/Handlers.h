#pragma once 

#include <memory>
namespace Utils {

	/*
		Handle definition will take care to manage and "handle" the lifetime of a resource, from the loading to unloading from memory.
		The initialziation will happen on the constructor, while the unloading will happen when the object goes out of scope or the release function is called.
	*/

	template<typename T, typename deleter = std::default_delete<T>>
	using  Handle = std::unique_ptr<T, deleter>;

	template<typename T>
	using ResourceHandle = std::shared_ptr<T>;
}