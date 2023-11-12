#pragma once 

#include <string>
#include <functional>

namespace Core {

	template<typename T>
	concept StringLike = std::is_convertible_v<T, std::string_view>;

	struct Tag {
		Tag() = default;
		template<StringLike StringTy>
		explicit Tag(StringTy&& string) noexcept : tag(std::hash<std::string_view>{}(string)) {}

		bool operator==(const Tag& other) const { return tag == other.tag; }
		bool operator!= (const Tag& other) const { return !(*this == other); }

	private:
		std::size_t tag;
	};

	class TaggedObject {
	public:
		TaggedObject() = default;
		TaggedObject(const Tag& t) noexcept : _tag(t) {}
		template<StringLike StringTy>
		explicit TaggedObject(StringTy&& string) noexcept : _tag(string) {}

		const Tag& GetTag()const { return _tag; }

		template<StringLike StringTy>
		void SetTag(StringTy&& string) {
			_tag = Tag{ std::forward<StringTy>(string) };
		}

		void SetTag(const Tag& t)noexcept { _tag = t; }

	private:
		Tag _tag;
	};

}
