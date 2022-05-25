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
#include <omni/utility/delegate.hpp>

namespace Omni
{
template<typename TRet, typename ...TParams>
class MulticastDelegate<TRet(TParams...)> final : private DelegateBase<TRet(TParams...)>
{
public:
	using Value = Delegate<TRet(TParams...)>;

	constexpr MulticastDelegate() = default;
	MulticastDelegate(const MulticastDelegate&) = delete;

	~MulticastDelegate()
	{
		for (auto& elem : invocations)
			delete elem;

		invocations.clear();
	}

	bool is_null() const
	{
		return invocations.size() < 1;
	}

	bool operator==(void* ptr) const
	{
		return (ptr == nullptr) && this->is_null();
	}

	bool operator!=(void* ptr) const
	{
		return (ptr != nullptr) || (!this->is_null());
	}

	size_t size() const
	{
		return invocations.size();
	}

	MulticastDelegate& operator=(const MulticastDelegate&) = delete;

	bool operator==(const MulticastDelegate& other) const
	{
		if (invocations.size() != other.invocations.size()) return false;
		auto otherIt = other.invocations.begin();
		for (auto it = invocations.begin(); it != invocations.end(); ++it)
			if (**it != **otherIt) return false;
		return true;
	}

	bool operator!=(const MulticastDelegate& other) const
	{
		return !(*this == other);
	}

	bool operator==(const Delegate<TRet(TParams...)>& other) const
	{
		if (is_null() && other.is_null()) return true;
		if (other.is_null() || (size() != 1)) return false;
		return (other.invocation == **invocations.begin());
	}

	bool operator!=(const Delegate<TRet(TParams...)>& other) const
	{
		return !(*this == other);
	}

	MulticastDelegate& operator+=(const MulticastDelegate& other)
	{
		for (auto& elem : other.invocations)
			this->invocations.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(elem->owner, elem->stub));
		return *this;
	}

	MulticastDelegate& operator+=(const Delegate<TRet(TParams...)>& other)
	{
		if (other.is_null()) return *this;
		this->invocations.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(other.invocation.owner, other.invocation.stub));
		return *this;
	}

	void operator()(TParams... params) const
	{
		for (auto& elem : invocations)
			(*(elem->stub))(elem->owner, params...);
	}

	template<typename THandler>
	void operator()(TParams... params, THandler handler) const
	{
		size_t index = 0;
		for (auto& elem : invocations)
		{
			TRet inv_ret = (*(elem->stub))(elem->owner, params...);
			if (!handler(index, inv_ret))
				break;

			++index;
		}
	}

	void add(const MulticastDelegate& other)
	{
		for (auto& elem : other.invocations)
			this->invocations.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(elem->owner, elem->stub));
	}

	void add(const Delegate<TRet(TParams...)>& other)
	{
		if (other.is_null())
			return;
			
		this->invocations.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(other.invocation.owner, other.invocation.stub));
	}

	template <Value::Function TFn>
	void emplace()
	{
		add(Value(nullptr, Value::template function_stub<TFn>));
	}

	template <class TOwner, Value::template Method<TOwner> TMth>
	void emplace(TOwner* owner)
	{
		add(Value(owner, Value::template method_stub<TOwner, TMth>));
	}

	template <class TOwner, Value::template ConstMethod<TOwner> TMth>
	void emplace(const TOwner* owner)
	{
		add(Value(const_cast<TOwner*>(owner),
			Value:: template const_method_stub<TOwner, TMth>));
	}

	template <typename TFunctor>
	void emplace(const TFunctor* functor)
	{
		add(Value(const_cast<TFunctor*>(functor),
			Value::template onst_method_stub<TFunctor, &TFunctor::operator()>));
	}

private:
	std::vector<typename DelegateBase<TRet(TParams...)>::InvocationElement*> invocations;
};
}
