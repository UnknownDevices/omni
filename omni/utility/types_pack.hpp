#pragma once

#include <omni/core.hpp>
#include <omni/utility/values_pack.hpp>

namespace Omni
{
template <typename... Types>
class TypesPack;

template<typename Type>
struct IsPack : std::false_type
{};

template<typename ... Types>
struct IsPack<TypesPack<Types...>> : std::true_type
{};
}

namespace Omni::PackImpl
{
template <typename Inst>
	requires IsPack<Inst>::value
struct UnpackFront;

template <typename Types_Front, typename... Types_Tail>
struct UnpackFront<TypesPack<Types_Front, Types_Tail...>>
{
	using Type = Types_Front;
};

template <typename LeftsPack, typename RightsPack>
	requires IsPack<LeftsPack>::value&& IsPack<RightsPack>::value
struct SplitPack
{
	OMNI_CONSTEXPR static auto left  = LeftsPack();
	OMNI_CONSTEXPR static auto right = RightsPack();
	OMNI_CONSTEXPR static auto whole = LeftsPack() + RightsPack();
};

template <typename... Lefts, auto = 0, typename... Rights>
OMNI_CONSTEXPR auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights...>);

template <typename... Lefts, auto = 0, typename Rights_Front, typename... Rights_Tail>
OMNI_CONSTEXPR auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights_Front,
	Rights_Tail...>);

template <size_t... is, typename... Lefts, auto = 0, typename Rights_Front,
	typename... Rights_Tail>
OMNI_CONSTEXPR auto split(std::index_sequence<is...>, TypesPack<Lefts...>,
	TypesPack<Rights_Front, Rights_Tail...>);

template <typename... Rets, auto = 0, typename... Sources>
OMNI_CONSTEXPR auto get(Values_Pack<size_t>, TypesPack<Rets...>, TypesPack<Sources...>);

template <size_t... indexes, typename... Rets, auto = 0, typename... Sources>
OMNI_CONSTEXPR auto get(Values_Pack<size_t, indexes...>, TypesPack<Rets...>,
	TypesPack<Sources...>);

template <size_t i, typename... Rets>
OMNI_CONSTEXPR auto erase(Values_Pack<size_t>, TypesPack<Rets...>);

template <size_t i, size_t... indexes, typename... Rets>
OMNI_CONSTEXPR auto erase(Values_Pack<size_t, indexes...>, TypesPack<Rets...>);

template <typename... Rets>
OMNI_CONSTEXPR auto reverse(TypesPack<Rets...>, TypesPack<>);

template <typename... Rets, auto = 0, typename... Rems>
OMNI_CONSTEXPR auto reverse(TypesPack<Rets...>, TypesPack<Rems...>);
}

namespace Omni
{
template <typename... Types>
class TypesPack
{
public:
	OMNI_CONSTEXPR TypesPack() noexcept = default;

	OMNI_CONSTEXPR static auto lenght() noexcept
	{
		return sizeof...(Types);
	}

	OMNI_CONSTEXPR static auto is_empty() noexcept
	{
		return sizeof...(Types) == 0;
	}

	template <typename... Concats>
	OMNI_CONSTEXPR auto operator+(TypesPack<Concats...>) const
	{
		return TypesPack<Types..., Concats...>();
	}

	template <typename... Concats>
	OMNI_CONSTEXPR static auto concat(TypesPack<Concats...>)
	{
		return TypesPack<Types..., Concats...>();
	}

	template <typename... Concats>
	OMNI_CONSTEXPR static auto concat()
	{
		return concat(TypesPack<Concats...>());
	}

	template <size_t pivot = lenght() / 2>
	OMNI_CONSTEXPR static auto split()
	{
		return PackImpl::split(std::make_index_sequence<pivot>(), TypesPack<>(),
			TypesPack<Types...>());
	}

	template <size_t... indexes>
	OMNI_CONSTEXPR static auto get(Values_Pack<size_t, indexes...> indexes_pack)
	{
		return PackImpl::get(indexes_pack, TypesPack<>(), TypesPack<Types...>());
	}

	template <size_t... indexes>
	OMNI_CONSTEXPR static auto get()
	{
		return get(Values_Pack<size_t, indexes...>());
	}

	template <size_t n = 1>
	OMNI_CONSTEXPR static auto get_front()
	{
		return split<n>().left;
	}

	template <size_t n = 1>
	OMNI_CONSTEXPR static auto get_back()
	{
		return split<lenght() - n>().right;
	}

	OMNI_CONSTEXPR static auto get_tail()
	{
		return split<1>().right;
	}

	template <size_t... indexes>
	OMNI_CONSTEXPR static auto erase(Values_Pack<size_t, indexes...> indexes_pack)
	{
		return PackImpl::erase<0>(indexes_pack, TypesPack<Types...>());
	}

	template <size_t... indexes>
	OMNI_CONSTEXPR static auto erase()
	{
		return erase(Values_Pack<size_t, indexes...>());
	}

	template <size_t n = 1>
	OMNI_CONSTEXPR static auto pop_front()
	{
		return split<n>().right;
	}

	template <size_t n = 1>
	OMNI_CONSTEXPR static auto pop_back()
	{
		return split<lenght() - n>().left;
	}

	template <size_t index, typename... Inserts>
	OMNI_CONSTEXPR static auto insert(TypesPack<Inserts...> inserts_pack)
	{
		return get_front<index>() + inserts_pack + pop_front<index>();
	}

	template <size_t index, typename... Inserts>
	OMNI_CONSTEXPR static auto insert()
	{
		return insert<index>(TypesPack<Inserts...>());
	}

	template <typename... Pushes>
	OMNI_CONSTEXPR static auto push_front(TypesPack<Pushes...> pushes_pack)
	{
		return pushes_pack + TypesPack<Types...>();
	}

	template <typename... Pushes>
	OMNI_CONSTEXPR static auto push_front()
	{
		return push_front(TypesPack<Pushes...>());
	}

	template <typename... Pushes>
	OMNI_CONSTEXPR static auto push_back(TypesPack<Pushes...> pushes_pack)
	{
		return TypesPack<Types...>() + pushes_pack;
	}

	template <typename... Pushes>
	OMNI_CONSTEXPR static auto push_back()
	{
		return push_back(TypesPack<Pushes...>());
	}

	OMNI_CONSTEXPR static auto reverse()
	{
		return PackImpl::reverse(TypesPack<>(), TypesPack<Types...>());
	}
};

template <template <typename ...> typename TTemplate, typename TypesPack>
	requires IsPack<TypesPack>::value
struct Instantiate;

template <template <typename ...> typename TTemplate, typename ... TArgs>
struct Instantiate<TTemplate, TypesPack<TArgs...>>
{
	using Type = TTemplate<TArgs...>;
};

template <typename TPack, size_t TIndex>
	requires IsPack<TPack>::value
using Unpack = PackImpl::UnpackFront<decltype(TPack::template pop_front<TIndex>())>::Type;

template <typename TPack, size_t TBegin, size_t TLenght>
	requires IsPack<TPack>::value
using SubPack = decltype(TPack::template pop_front<TBegin>().template get_front<TLenght>());
}

namespace Omni::PackImpl
{
template <typename... Lefts, auto, typename... Rights>
OMNI_CONSTEXPR auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights...>)
{
	return SplitPack<TypesPack<Lefts...>, TypesPack<Rights...>>();
}

template <typename... Lefts, auto, typename Rights_Front, typename... Rights_Tail>
OMNI_CONSTEXPR auto split(std::index_sequence<>, TypesPack<Lefts...>,
	TypesPack<Rights_Front, Rights_Tail...>)
{
	return SplitPack<TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>>();
}

template <size_t... is, typename... Lefts, auto, typename Rights_Front,
	typename... Rights_Tail>
OMNI_CONSTEXPR auto split(std::index_sequence<is...>, TypesPack<Lefts...>,
	TypesPack<Rights_Front, Rights_Tail...>)
{
	return split(std::make_index_sequence<sizeof...(is) - 1>(),
		TypesPack<Lefts..., Rights_Front>(), TypesPack<Rights_Tail...>());
}

template <typename... Rets, auto, typename... Sources>
OMNI_CONSTEXPR auto get(Values_Pack<size_t>, TypesPack<Rets...> rets_pack,
	TypesPack<Sources...>)
{
	return rets_pack;
}

template <size_t... indexes, typename... Rets, auto, typename... Sources>
OMNI_CONSTEXPR auto get(Values_Pack<size_t, indexes...> indexes_pack,
	TypesPack<Rets...> rets_pack, TypesPack<Sources...> sources_pack)
{
	return get(indexes_pack.get_tail(), rets_pack +
		sources_pack.split<indexes_pack.front>().right.split<1>().left, sources_pack);
}

template <size_t i, typename... Rets>
OMNI_CONSTEXPR auto erase(Values_Pack<size_t>, TypesPack<Rets...> rets_pack)
{
	return rets_pack;
}

template <size_t i, size_t... indexes, typename... Rets>
OMNI_CONSTEXPR auto erase(Values_Pack<size_t, indexes...> indexes_pack,
	TypesPack<Rets...> rets_pack)
{
	return erase<i + 1>(indexes_pack.get_tail(),
		rets_pack.get_front<indexes_pack.front - i>() +
		rets_pack.pop_front<indexes_pack.front - i + 1>());
}

template <typename... Rets>
OMNI_CONSTEXPR auto reverse(TypesPack<Rets...> rets_pack, TypesPack<>)
{
	return rets_pack;
}

template <typename... Rets, auto, typename... Rems>
OMNI_CONSTEXPR auto reverse(TypesPack<Rets...> rets_pack, TypesPack<Rems...> rems_pack)
{
	return reverse(rems_pack.get_front() + rets_pack, rems_pack.get_tail());
}
}
