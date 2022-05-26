 /*
	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.OmniKryukov.org
	http://www.codeproject.com/Members/OmniKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed
*/

#pragma once

#include <omni/core.hpp>
#include <omni/utility/delegate_base.hpp>
#include <omni/utility/types_pack.hpp>

namespace Omni
{
template <typename T>
class Delegate;

template <typename T>
class MulticastDelegate;

template<typename TRet, typename ... TParams>
class Delegate<TRet(TParams...)> final : private DelegateBase<TRet(TParams...)>
{
public:
	using Base        = DelegateBase<TRet(TParams...)>;  

	using Args        = TypesPack<TRet, TParams...>;
	using Ret         = TRet;
	using Params      = TypesPack<TParams...>;

	using Function    = TRet(*)(TParams...);
	template <typename TOwner>
	using Method      = TRet(TOwner::*)(TParams...);
	template <typename TOwner>
	using ConstMethod = TRet(TOwner::*)(TParams...) const;

	friend class MulticastDelegate<TRet(TParams...)>;

	OMNI_CONSTEXPR Delegate() noexcept
	{
		invocation_.owner = nullptr;
		invocation_.stub = &null_stub;
	}

	OMNI_CONSTEXPR Delegate(const Delegate& other)
	{ 
		other.invocation_.clone(invocation_);
	}

	bool is_null() const
	{
		return invocation_.stub == nullptr;
	}

	Delegate& operator=(const Delegate& other)
	{
		other.invocation_.clone(invocation_);
		return *this;
	}

	template <typename TFunctor>
	Delegate& operator=(const TFunctor& functor)
	{
		assign((void*)(&functor), const_method_stub<TFunctor, &TFunctor::operator()>);
		return *this;
	}

	bool operator==(const Delegate& other) const
	{
		return invocation_ == other.invocation_;
	}

	bool operator==(const MulticastDelegate<TRet(TParams...)>& other) const
	{
		if (other.size() != 1)
			return false;

		return *this == other[0];
	}

	bool operator!=(const Delegate& other) const
	{
		return invocation_ != other.invocation_;
	}

	bool operator!=(const MulticastDelegate<TRet(TParams...)>& other) const
	{
		return other != (*this);
	}

	template <Function TFn>
	static Delegate from()
	{
		return Delegate(nullptr, function_stub<TFn>);
	}

	template <class TOwner, Method<TOwner> TMth>
	static Delegate from(TOwner* owner)
	{
		return Delegate(owner, method_stub<TOwner, TMth>);
	}

	template <class TOwner, ConstMethod<TOwner> TMth>
	static Delegate from(const TOwner* owner)
	{
		return Delegate(const_cast<TOwner*>(owner), const_method_stub<TOwner, TMth>);
	}

	template <typename TFunctor>
	static Delegate from(const TFunctor* functor)
	{
		return Delegate(const_cast<TFunctor*>(functor),
			const_method_stub<TFunctor, &TFunctor::operator()>);
	}

	TRet operator()(TParams... params) const
	{
		return (*invocation_.stub)(invocation_.owner, params...);
	}

private:
	Delegate(void* owner, Base::stub_type stub) : invocation_(owner, stub)
	{}

	void assign(void* owner, Base::stub_type stub)
	{
		invocation_.owner = owner;
		invocation_.stub = stub;
	}

	template <Function TFn>
	static TRet function_stub(void*, TParams... params)
	{
		return (TFn)(params...);
	}

	template <class TOwner, Method<TOwner> TMth>
	static TRet method_stub(void* owner, TParams... params)
	{
		TOwner* p = static_cast<TOwner*>(owner);
		return (p->*TMth)(params...);
	}

	template <class TOwner, ConstMethod<TOwner> TMth>
	static TRet const_method_stub(void* owner, TParams... params)
	{
		const TOwner* p = static_cast<TOwner*>(owner);
		return (p->*TMth)(params...);
	}

	static TRet null_stub(void*, TParams...)
	{
		return TRet();
	}

	Base::InvocationElement invocation_;
};

template<typename T>
struct IsDelegate : std::false_type
{};

template <typename TRet, typename ... TParams>
struct IsDelegate<Delegate<TRet(TParams...)>> : std::true_type
{};

template <typename TArgsF, typename ... TArgsT>
struct Instantiate<Delegate, TypesPack<TArgsF, TArgsT...>>
{
	using Type = Delegate<TArgsF(TArgsT...)>;
};
}
