/*
	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed
*/

#pragma once

#include <omni/core.hpp>
#include <omni/utility/delegate.hpp>

namespace Omni
{
template<typename TRet, typename ...TParams>
class MulticastDelegate<TRet(TParams...)> final : private DelegateBase<TRet(TParams...)>
{
public:
	using Value         = Delegate<TRet(TParams...)>;

	using Args        = TypesPack<TRet, TParams...>;
	using Ret         = TRet;
	using Params      = TypesPack<TParams...>;
	
	using Iterator      = std::vector<Value>::iterator;
	using ConstIterator = std::vector<Value>::const_iterator;

	OMNI_CONSTEXPR MulticastDelegate() noexcept = default;
	~MulticastDelegate() noexcept = default;
 
 	Iterator begin() noexcept { return dels_.begin(); }
	Iterator end() noexcept { return dels_.end(); }

	ConstIterator begin() const noexcept { return dels_.begin(); }
	ConstIterator end() const noexcept { return dels_.end(); }

	ConstIterator cbegin() const noexcept { return dels_.cbegin(); }
	ConstIterator cend() const noexcept { return dels_.cend(); }

	bool empty() const
	{
		return dels_.empty();
	}

	size_t size() const
	{
		return dels_.size();
	}

	MulticastDelegate& operator=(const MulticastDelegate&) = default;

	bool operator==(const MulticastDelegate& other) const
	{
		return dels_ == other.dels_;
	}

	bool operator==(const Value& other) const
	{
		if (size() != 1)
			return false;

		return get(0) == other;
	}

	bool operator!=(const MulticastDelegate& other) const
	{
		return !(*this == other);
	}

	bool operator!=(const Value& other) const
	{
		return !(*this == other);
	}

	MulticastDelegate& operator+=(const MulticastDelegate& target)
	{
		add(target);
		return *this;
	}

	MulticastDelegate& operator+=(const Value& target)
	{
		add(target);
		return *this;
	}

	MulticastDelegate& operator-=(const Value& target)
	{
		remove(target);
		return *this;
	}

	Value& operator[](size_t index)
	{
		return get(index);
	}

	const Value& operator[](size_t index) const
	{
		return get(index);
	}

	void operator()(TParams... params) const
	{
		for (auto& elem : *this)
			elem(params...);
	}

	template<typename THandler>
	void operator()(TParams... params, THandler handler) const
	{
		size_t index = 0;
		for (auto& elem : *this)
		{
			TRet inv_ret = elem(params...);
			if (!handler(index, inv_ret))
				break;

			++index;
		}
	}

	Value& get(size_t index)
	{
		return dels_[index];
	}

	const Value& get(size_t index) const
	{
		return dels_[index];
	}

	void add(const Value& target)
	{
		dels_.push_back(target);
	}

	void add(const MulticastDelegate& target)
	{
		for (auto& elem : target)
			add(elem);
	}

	template <Value::Function TFn>
	void emplace()
	{
		add(Value::template from<TFn>());
	}

	template <class TOwner, Value::template Method<TOwner> TMth>
	void emplace(TOwner* owner)
	{
		add(Value::template from<TOwner, TMth>(owner));
	}

	template <class TOwner, Value::template ConstMethod<TOwner> TMth>
	void emplace(const TOwner* owner)
 	{
		add(Value::template from<TOwner, TMth>(owner));
	}

	template <typename TFunctor>
	void emplace(const TFunctor* functor)
	{
		add(Value::from(functor));
	}

	void remove(const Value& target)
	{
		dels_.erase(target);
	}

	void clear()
	{
		dels_.clear();
	}

private:
	std::vector<Value> dels_;
};

template<typename T>
struct IsMulticastDelegate : std::false_type
{};

template <typename TRet, typename ... TParams>
struct IsMulticastDelegate<MulticastDelegate<TRet(TParams...)>> : std::true_type
{};

template <typename TArgsF, typename ... TArgsT>
struct Instantiate<MulticastDelegate, TypesPack<TArgsF, TArgsT...>>
{
	using Type = MulticastDelegate<TArgsF(TArgsT...)>;
};
}
