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
	class Delegate<TRet(TParams...)> final : private DelegateBase<TRet(TParams...)> {
	public:
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

		template <TRet(*TMethod)(TParams...)>
		static Delegate create()
		{
			return Delegate(nullptr, function_stub<TMethod>);
		}

		template <class T, TRet(T::*Method)(TParams...)>
		static Delegate create(T* owner)
		{
			return Delegate(owner, method_stub<T, Method>);
		}

		template <class T, TRet(T::*Method)(TParams...) const>
		static Delegate create(T const* owner)
		{
			return Delegate(const_cast<T*>(owner), const_method_stub<T, Method>);
		}

		template <typename TFunctor>
		static Delegate create(const TFunctor* functor)
		{
			return Delegate(const_cast<TFunctor*>(functor), 
				const_method_stub<TFunctor, &TFunctor::operator()>);
		}

		TRet operator()(TParams... arg) const
		{
			return (*invocation.stub)(invocation.owner, arg...);
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

		template <TRet(*Function)(TParams...)>
		static TRet function_stub(void*, TParams... params)
		{
			return (Function)(params...);
		}

		template <class TOwner, TRet(TOwner::*Method)(TParams...)>
		static TRet method_stub(void* owner, TParams... params)
		{
			TOwner* p = static_cast<TOwner*>(owner);
			return (p->*Method)(params...);
		}

		template <class TOwner, TRet(TOwner::*Method)(TParams...) const>
		static TRet const_method_stub(void* owner, TParams... params)
		{
			TOwner* const p = static_cast<TOwner*>(owner);
			return (p->*Method)(params...);
		}

		DelegateBase<TRet(TParams...)>::InvocationElement invocation;
	};

	template<typename Type>
	struct IsDelegate : std::false_type
	{};

	template <typename TRet, typename ... TParams>
	struct IsDelegate<Delegate<TRet(TParams...)>> : std::true_type
	{};

	template<typename Type>
	struct IsFunctionPtr : std::false_type
	{};

	template <typename TRet, typename ... TParams>
	struct IsFunctionPtr<TRet(*)(TParams...)> : std::true_type
	{};

	template<typename Type>
	struct IsMethodPtr : std::false_type
	{};

	template <typename TOwner, typename TRet, typename ... TParams>
	struct IsMethodPtr<TRet(TOwner::*)(TParams...)> : std::true_type
	{};

	template<typename Type>
	struct IsConstMethodPtr : std::false_type
	{};

	template <typename TOwner, typename TRet, typename ... TParams>
	struct IsConstMethodPtr<TRet(TOwner::*)(TParams...) const> : std::true_type
	{};
}

template <typename TRet, typename ... TParams>
struct ArgsOf<Omni::Delegate<TRet(TParams...)>>
{
	using Template = Omni::Delegate;
	using Args     = TypesPack<TRet, TParams...>;

	using Ret      = TRet;
	using Params   = TypesPack<TParams...>;
};

template <typename TRet, typename ... TParams>
struct ArgsOf<TRet(*)(TParams...)>
{
	using Args   = TypesPack<TRet, TParams...>;

	using Ret    = TRet;
	using Params = TypesPack<TParams...>;
};

template <typename TOwner, typename TRet, typename ... TParams>
struct ArgsOf<TRet(TOwner::*)(TParams...)>
{
	using Args   = TypesPack<TOwner, TRet, TParams...>;

	using Owner  = TOwner;
	using Ret    = TRet;
	using Params = TypesPack<TParams...>;
};

template <typename TOwner, typename TRet, typename ... TParams>
struct ArgsOf<TRet(TOwner::*)(TParams...) const>
{
	using Args   = TypesPack<TOwner, TRet, TParams...>;

	using Owner  = TOwner;
	using Ret    = TRet;
	using Params = TypesPack<TParams...>;
};
