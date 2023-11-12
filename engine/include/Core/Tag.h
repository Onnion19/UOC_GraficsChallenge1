#pragma once 

#include <string>
#include <functional>
#include <ostream>
namespace Core {

	template<typename T>
	concept StringLike = std::is_convertible_v<T, std::string_view>;


#if DEBUG
	struct Tag {
		Tag() = default;
		template<StringLike StringTy>
		explicit Tag(StringTy string) noexcept : tag(std::hash<std::string_view>{}(string)), name(string) {}

		Tag(const Tag& t) : tag(t.tag), name(t.name) {}

		bool operator==(const Tag& other) const { return tag == other.tag; }
		bool operator!= (const Tag& other) const { return !(*this == other); }


		friend std::ostream& operator <<(std::ostream& stream, const Tag& t)
		{
			return stream << t.name;
		}

	private:
		std::size_t tag = 0;
		std::string name;
	};
#else
	struct Tag {
		Tag() = default;
		template<StringLike StringTy>
		explicit Tag(StringTy&& string) noexcept : tag(std::hash<std::string_view>{}(string)) {}

		bool operator==(const Tag& other) const { return tag == other.tag; }
		bool operator!= (const Tag& other) const { return !(*this == other); }

		friend std::ostream& operator <<(std::ostream& stream, const Tag& t)
		{
			return stream << t.tag;
		}

	private:
		std::size_t tag;
	};
#endif

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
