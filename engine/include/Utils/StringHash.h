#pragma once

#include <functional>
#include <string_view>


struct StringHash
{
private:
	inline static const std::hash<std::string_view>hashFactory{};
	std::size_t id;
public:
	explicit StringHash(std::string_view name) noexcept : id(hashFactory(name)) {}
	explicit StringHash(const char* name) noexcept : id(hashFactory(name)) {}

	size_t GetId()const { return id; }
	inline operator size_t() const noexcept { return id; }
};

namespace std
{
	template<>
	struct std::hash<StringHash> {
		size_t operator()(const StringHash& resourceID) const noexcept {
			// implicit size_t cast
			return resourceID;
		}
	};
}