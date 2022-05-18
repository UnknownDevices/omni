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
		constexpr MulticastDelegate() = default;

		~MulticastDelegate()
		{
			for (auto& element : invocationList) delete element;
			invocationList.clear();
		}

		bool is_null() const
		{
			return invocationList.size() < 1;
		}
		bool operator ==(void* ptr) const
		{
			return (ptr == nullptr) && this->is_null();
		} //operator ==
		bool operator !=(void* ptr) const
		{
			return (ptr != nullptr) || (!this->is_null());
		} //operator !=

		size_t size() const
		{
			return invocationList.size();
		}

		MulticastDelegate& operator =(const MulticastDelegate&) = delete;
		MulticastDelegate(const MulticastDelegate&) = delete;

		bool operator ==(const MulticastDelegate& another) const
		{
			if (invocationList.size() != another.invocationList.size()) return false;
			auto anotherIt = another.invocationList.begin();
			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
				if (**it != **anotherIt) return false;
			return true;
		} //==
		bool operator !=(const MulticastDelegate& another) const
		{
			return !(*this == another);
		}

		bool operator ==(const Delegate<TRet(TParams...)>& another) const
		{
			if (is_null() && another.is_null()) return true;
			if (another.is_null() || (size() != 1)) return false;
			return (another.invocation == **invocationList.begin());
		} //==
		bool operator !=(const Delegate<TRet(TParams...)>& another) const
		{
			return !(*this == another);
		}

		MulticastDelegate& operator +=(const MulticastDelegate& another)
		{
			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
				this->invocationList.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(item->owner, item->stub));
			return *this;
		} //operator +=

		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		MulticastDelegate& operator +=(const LAMBDA& lambda)
		{
			Delegate<TRet(TParams...)> d = Delegate<TRet(TParams...)>::template create<LAMBDA>(lambda);
			return *this += d;
		} //operator +=

		MulticastDelegate& operator +=(const Delegate<TRet(TParams...)>& another)
		{
			if (another.is_null()) return *this;
			this->invocationList.push_back(new typename DelegateBase<TRet(TParams...)>::InvocationElement(another.invocation.owner, another.invocation.stub));
			return *this;
		} //operator +=

		// will work even if RET is void, return values are ignored:
		// (for handling return values, see operator(..., handler))
		void operator()(TParams... params) const
		{
			for (auto& item : invocationList)
				(*(item->stub))(item->owner, params...);
		} //operator()

		template<typename HANDLER>
		void operator()(TParams... params, HANDLER) const
		{
			size_t index = 0;
			for (auto& item : invocationList)
			{
				TRet value = (*(item->stub))(item->owner, params...);
				handler(index, &value);
				++index;
			} //loop
		} //operator()

		void operator()(TParams... params, Delegate<void(size_t, TRet*)> handler) const
		{
			operator()<decltype(handler)>(params..., handler);
		} //operator()
		void operator()(TParams... params, std::function<void(size_t, TRet*)> handler) const
		{
			operator()<decltype(handler)>(params..., handler);
		} //operator()

	private:
		std::list<typename DelegateBase<TRet(TParams...)>::InvocationElement*> invocationList;
	}; //class MulticastDelegate
} /* namespace SA */