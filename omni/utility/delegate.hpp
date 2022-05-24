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
	using Args        = TypesPack<TRet, TParams...>;

	using Ret         = TRet;
	using Params      = TypesPack<TParams...>;

	using Function    = TRet(*)(TParams...);
	template <typename TOwner>
	using Method      = TRet(TOwner::*)(TParams...);
	template <typename TOwner>
	using ConstMethod = TRet(TOwner::*)(TParams...) const;

	friend class MulticastDelegate<TRet(TParams...)>;

	Delegate() = default;

	Delegate(const Delegate& other)
	{
		other.invocation.clone(invocation);
	}

	template <typename TFunctor>
	Delegate(const TFunctor& functor)
	{
		assign((void*)(&functor), const_method_stub<TFunctor, &TFunctor::operator()>);
	}

	bool is_null() const
	{
		return invocation.stub == nullptr;
	}

	Delegate& operator=(const Delegate& other)
	{
		other.invocation.clone(invocation);
		return *this;
	}

	template <typename TFunctor>
	Delegate& operator=(const TFunctor& functor)
	{
		assign((void*)(&functor), const_method_stub<TFunctor, &TFunctor::operator()>);
		return *this;
	}

	bool operator==(void* ptr) const
	{
		return (ptr == nullptr) && this->is_null();
	}

	bool operator==(const Delegate& other) const
	{
		return invocation == other.invocation;
	}

	bool operator==(const MulticastDelegate<TRet(TParams...)>& other) const
	{
		return other == (*this);
	}

	bool operator!=(void* ptr) const
	{
		return (ptr != nullptr) || (!this->is_null());
	}

	bool operator!=(const Delegate& other) const
	{
		return invocation != other.invocation;
	}

	bool operator!=(const MulticastDelegate<TRet(TParams...)>& other) const
	{
		return other != (*this);
	}

	template <TRet(*TFn)(TParams...)>
	static Delegate from()
	{
		return Delegate(nullptr, function_stub<TFn>);
	}

	template <class TOwner, TRet(TOwner::* TMth)(TParams...)>
	static Delegate from(TOwner* owner)
	{
		return Delegate(owner, method_stub<TOwner, TMth>);
	}

	template <class TOwner, TRet(TOwner::* TMth)(TParams...) const>
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
		return (*invocation.stub)(invocation.owner, params...);
	}

private:
	Delegate(void* owner, DelegateBase<TRet(TParams...)>::stub_type stub)
	{
		invocation.owner = owner;
		invocation.stub = stub;
	}

	void assign(void* owner, DelegateBase<TRet(TParams...)>::stub_type stub)
	{
		this->invocation.owner = owner;
		this->invocation.stub = stub;
	}

	template <TRet(*TFn)(TParams...)>
	static TRet function_stub(void*, TParams... params)
	{
		return (TFn)(params...);
	}

	template <class TOwner, TRet(TOwner::* TMth)(TParams...)>
	static TRet method_stub(void* owner, TParams... params)
	{
		TOwner* p = static_cast<TOwner*>(owner);
		return (p->*TMth)(params...);
	}

	template <class TOwner, TRet(TOwner::* TMth)(TParams...) const>
	static TRet const_method_stub(void* owner, TParams... params)
	{
		const TOwner* p = static_cast<TOwner*>(owner);
		return (p->*TMth)(params...);
	}

	DelegateBase<TRet(TParams...)>::InvocationElement invocation;
};

template<typename Type>
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
