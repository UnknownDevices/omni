#pragma once

#include <omni/utility/values_pack.hpp>

template <typename... Types>
class TypesPack;

template<typename Type>
struct IsTypePackInst : std::false_type
{};

template<typename ... Types>
struct IsTypePackInst<TypesPack<Types...>> : std::true_type
{};

template <typename Type>
concept TypesPackInst = requires()
{
	IsTypePackInst<Type>::value;
};

namespace TypesPackDetails
{
	template <TypesPackInst Instance>
	struct Front_Unpacker;

	template <typename Types_Front, typename... Types_Tail>
	struct Front_Unpacker<TypesPack<Types_Front, Types_Tail...>>
	{
		using Front = Types_Front;
	};

	template <TypesPackInst Lefts_Pack, TypesPackInst Rights_Pack>
	struct Split_Types_Pack
	{
		constexpr static auto left = Lefts_Pack();
		constexpr static auto right = Rights_Pack();
		constexpr static auto whole = Lefts_Pack() + Rights_Pack();
	};

	template <typename... Lefts, auto = 0, typename... Rights>
	consteval auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights...>);

	template <typename... Lefts, auto = 0, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>);

	template <size_t... is, typename... Lefts, auto = 0, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<is...>, TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>);

	template <typename... Rets, auto = 0, typename... Sources>
	consteval auto get(Values_Pack<size_t>, TypesPack<Rets...>, TypesPack<Sources...>);

	template <size_t... indexes, typename... Rets, auto = 0, typename... Sources>
	consteval auto get(Values_Pack<size_t, indexes...>, TypesPack<Rets...>, TypesPack<Sources...>);

	template <size_t i, typename... Rets>
	consteval auto erase(Values_Pack<size_t>, TypesPack<Rets...>);

	template <size_t i, size_t... indexes, typename... Rets>
	consteval auto erase(Values_Pack<size_t, indexes...>, TypesPack<Rets...>);

	template <typename... Rets>
	consteval auto reverse(TypesPack<Rets...>, TypesPack<>);

	template <typename... Rets, auto = 0, typename... Rems>
	consteval auto reverse(TypesPack<Rets...>, TypesPack<Rems...>);
}

template <typename... Types>
class TypesPack
{
public:
	template <size_t index>
	using Unpack = TypesPackDetails::Front_Unpacker<decltype(pop_front<index>())>::Front;

	consteval TypesPack() noexcept = default;

	consteval static auto size() noexcept
	{
		return sizeof...(Types);
	}
	
	consteval static auto is_empty() noexcept
	{
		return sizeof...(Types) == 0;
	}

	template <typename... Concats>
	consteval auto operator+(TypesPack<Concats...>) const
	{
		return TypesPack<Types..., Concats...>();
	}

	template <typename... Concats>
	consteval static auto concat(TypesPack<Concats...>)
	{
		return TypesPack<Types..., Concats...>();
	}

	template <typename... Concats>
	consteval static auto concat()
	{
		return concat(TypesPack<Concats...>());
	}

	template <size_t pivot = size() / 2>
	consteval static auto split()
	{
		return TypesPackDetails::split(std::make_index_sequence<pivot>(), TypesPack<>(), TypesPack<Types...>());
	}

	template <size_t... indexes>
	consteval static auto get(Values_Pack<size_t, indexes...> indexes_pack)
	{
		return TypesPackDetails::get(indexes_pack, TypesPack<>(), TypesPack<Types...>());
	}

	template <size_t... indexes>
	consteval static auto get()
	{
		return get(Values_Pack<size_t, indexes...>());
	}

	template <size_t n = 1>
	consteval static auto get_front()
	{
		return split<n>().left;
	}

	template <size_t n = 1>
	consteval static auto get_back()
	{
		return split<size() - n>().right;
	}

	consteval static auto get_tail()
	{
		return split<1>().right;
	}

	template <size_t... indexes>
	consteval static auto erase(Values_Pack<size_t, indexes...> indexes_pack)
	{
		return TypesPackDetails::erase<0>(indexes_pack, TypesPack<Types...>());
	}

	template <size_t... indexes>
	consteval static auto erase()
	{
		return erase(Values_Pack<size_t, indexes...>());
	}

	template <size_t n = 1>
	consteval static auto pop_front()
	{
		return split<n>().right;
	}

	template <size_t n = 1>
	consteval static auto pop_back()
	{
		return split<size() - n>().left;
	}

	template <size_t index, typename... Inserts>
	consteval static auto insert(TypesPack<Inserts...> inserts_pack)
	{
		return get_front<index>() + inserts_pack + pop_front<index>();
	}

	template <size_t index, typename... Inserts>
	consteval static auto insert()
	{
		return insert<index>(TypesPack<Inserts...>());
	}

	template <typename... Pushes>
	consteval static auto push_front(TypesPack<Pushes...> pushes_pack)
	{
		return pushes_pack + TypesPack<Types...>();
	}

	template <typename... Pushes>
	consteval static auto push_front()
	{
		return push_front(TypesPack<Pushes...>());
	}

	template <typename... Pushes>
	consteval static auto push_back(TypesPack<Pushes...> pushes_pack)
	{
		return TypesPack<Types...>() + pushes_pack;
	}

	template <typename... Pushes>
	consteval static auto push_back()
	{
		return push_back(TypesPack<Pushes...>());
	}

	consteval static auto reverse()
	{
		return TypesPackDetails::reverse(TypesPack<>(), TypesPack<Types...>());
	}
};

namespace TypesPackDetails
{
	template <typename... Lefts, auto, typename... Rights>
	consteval auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights...>)
	{
		return Split_Types_Pack<TypesPack<Lefts...>, TypesPack<Rights...>>();
	}

	template <typename... Lefts, auto, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<>, TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>)
	{
		return Split_Types_Pack<TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>>();
	}

	template <size_t... is, typename... Lefts, auto, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<is...>, TypesPack<Lefts...>, TypesPack<Rights_Front, Rights_Tail...>)
	{
		return split(std::make_index_sequence<sizeof...(is) - 1>(), TypesPack<Lefts..., Rights_Front>(), TypesPack<Rights_Tail...>());
	}

	template <typename... Rets, auto, typename... Sources>
	consteval auto get(Values_Pack<size_t>, TypesPack<Rets...> rets_pack, TypesPack<Sources...>)
	{
		return rets_pack;
	}

	template <size_t... indexes, typename... Rets, auto, typename... Sources>
	consteval auto get(Values_Pack<size_t, indexes...> indexes_pack, TypesPack<Rets...> rets_pack, TypesPack<Sources...> sources_pack)
	{
		return get(indexes_pack.get_tail(), rets_pack + sources_pack.split<indexes_pack.front>().right.split<1>().left, sources_pack);
	}

	template <size_t i, typename... Rets>
	consteval auto erase(Values_Pack<size_t>, TypesPack<Rets...> rets_pack)
	{
		return rets_pack;
	}

	template <size_t i, size_t... indexes, typename... Rets>
	consteval auto erase(Values_Pack<size_t, indexes...> indexes_pack, TypesPack<Rets...> rets_pack)
	{
		return erase<i + 1>(indexes_pack.get_tail(), rets_pack.get_front<indexes_pack.front - i>() + rets_pack.pop_front<indexes_pack.front - i + 1>());
	}

	template <typename... Rets>
	consteval auto reverse(TypesPack<Rets...> rets_pack, TypesPack<>)
	{
		return rets_pack;
	}

	template <typename... Rets, auto, typename... Rems>
	consteval auto reverse(TypesPack<Rets...> rets_pack, TypesPack<Rems...> rems_pack)
	{
		return reverse(rems_pack.get_front() + rets_pack, rems_pack.get_tail());
	}
}

template <typename T>
struct ArgsOf;
