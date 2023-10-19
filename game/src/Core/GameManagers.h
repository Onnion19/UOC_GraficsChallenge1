#pragma once 

#include <unordered_map>
#include <typeinfo>
#include <any>
#include <assert.h>
#include <type_traits>

namespace Core {
	struct GameManagers
	{
		template<typename T, typename ... Args>
		T& RegisterManager(Args&& ... args)
		{
			auto id = GetManagerHash<T>();
			assert(managers.find(id) == managers.end() && "Registering an already existing manager");

			std::pair<std::size_t, std::any> item;

			if constexpr (std::is_constructible_v<T, GameManagers&, Args...>)
				item = { id, std::make_any<T>(*this, std::forward<Args>(args)...) };
			else
					item = { id, std::make_any<T>(std::forward<Args>(args)...) };


			auto it = managers.emplace(item);
			return std::any_cast<T&>(it.first->second);
		}

		template<typename T>
		void UnregisterManager()
		{
			auto id = GetManagerHash<T>();
			managers.erase(id);
		}


		template<typename T>
		T& GetManager()
		{
			auto id = GetManagerHash<T>();
			auto iter = managers.find(id);
			assert(iter != managers.end() && "Accessing non registered manager");
			return std::any_cast<T&>(iter->second);
		}

	private:
		template<typename T>
		std::size_t GetManagerHash() const
		{
			return typeid(T).hash_code();
		}
	private:
		std::unordered_map<std::size_t, std::any> managers;
	};
}