#pragma once 

#include <unordered_map>
#include <typeinfo>
#include <any>
#include <assert.h>
#include <type_traits>

namespace Core {
	/**
	* Game managers is the central storage for all the systems/managers. 
	* It allows to register ANY class as a manager and retrieve it back later.
	*/
	struct GameManagers
	{
		template<typename T, typename ... Args>
		T& RegisterManager(Args&& ... args)
		{
			auto id = GetManagerHash<T>();
			assert(managers.find(id) == managers.end() && "Registering an already existing manager");

			if constexpr (std::is_constructible_v<T, GameManagers&, Args...>) {
				
				std::pair<std::size_t, void*> item{ id, new T(*this, std::forward<Args>(args)...) };
				auto it = managers.emplace(item);
				return *(static_cast<T*>(it.first->second));
			}
			else {
				std::pair<std::size_t, void*> item = { id, new T(std::forward<Args>(args)...) };
				auto it = managers.emplace(item);
				return *(static_cast<T*>(it.first->second));
			}
		}

		template<typename T>
		void UnregisterManager()
		{
			auto id = GetManagerHash<T>();
			auto iter = managers.find(id);
			assert(iter != managers.end() && "Unregistering an unexisting manager");
			delete static_cast<T*>(iter.second);
			managers.erase(id);
		}


		template<typename T>
		T& GetManager()
		{
			auto id = GetManagerHash<T>();
			auto iter = managers.find(id);
			assert(iter != managers.end() && "Accessing non registered manager");
			return *(static_cast<T*>(iter->second));
		}

	private:
		template<typename T>
		std::size_t GetManagerHash() const
		{
			return typeid(T).hash_code();
		}
	private:
		std::unordered_map<std::size_t, void*> managers;
	};
}