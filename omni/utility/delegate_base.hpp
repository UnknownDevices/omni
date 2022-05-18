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

namespace Omni
{
	template<typename T>
	class DelegateBase;

	template<typename TRet, typename ...TParams>
	class DelegateBase<TRet(TParams...)>
	{
	protected:
		using stub_type = TRet(*)(void*, TParams...);

		struct InvocationElement
		{
			constexpr InvocationElement() = default;
			InvocationElement(void* owner, stub_type stub) : owner(owner), stub(stub)
			{}

			void clone(InvocationElement& target) const
			{
				target.stub = stub;
				target.owner = owner;
			}

			bool operator==(const InvocationElement& other) const
			{
				return other.stub == stub && other.owner == owner;
			}

			bool operator!=(const InvocationElement& other) const
			{
				return other.stub != stub || other.owner != owner;
			}

			void* owner = nullptr;
			stub_type stub = nullptr;
		};

	};

}