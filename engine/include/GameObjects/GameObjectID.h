#pragma once 

#include <functional>
// Introduces the hashing system for game objects. 
/*
	The hashing should not be creation order identifier as it should be able to serialize.
	An option could be the windows UUID (Requires investigation between performance vs value).

	As a first iteration it will be just an incremental number that can't be serialized
*/

namespace GameObject
{
	// This probably will need to be an UUID
	// https://learn.microsoft.com/en-us/windows/win32/api/guiddef/ns-guiddef-guid
	struct ID {

		ID(const ID&) = delete;
		ID(ID&&) = delete;
		ID& operator=(const ID&) = delete;
		ID& operator=(const ID&&) = delete;

		bool operator==(const ID& i)const { return id == i.id; }
		bool operator <(const ID& i)const { return id < i.id; }
		bool operator >(const ID& i)const { return id > i.id; }

		operator unsigned int() const { return id; }

	private:
		ID(unsigned int i) : id(i) {}
		const unsigned int id;
		friend class IdFactory;
	};


	// This probably will require to be updated to be UUID generator: 
	// https://learn.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-uuidcreatesequential
	class IdFactory {
		inline static unsigned int seed = 0;

	public:
		static ID MakeGameObjectID() noexcept { return ++seed; }
		static ID MakeInvalidID() noexcept { return 0; }
	};

	class IdentifiedObject {
	public:
		IdentifiedObject() noexcept : id(IdFactory::MakeGameObjectID()) {}
		[[nodiscard]] const ID& GetId() const { return id; }
	private:
		ID id;
	};

}

namespace std {
	template<>
	struct std::hash <GameObject::ID> {
		size_t operator()(const GameObject::ID& id) const noexcept {
			// implicit size_t cast
			return static_cast<std::size_t>(id.operator unsigned int());
		}
	};
}