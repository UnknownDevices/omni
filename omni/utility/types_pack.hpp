#pragma once

#include <omni/utility/values_pack.hpp>

template <typename... Types>
class Types_Pack;

template<typename Type>
struct Is_Types_Pack_Instance : std::false_type {};

template<typename ... Types>
struct Is_Types_Pack_Instance<Types_Pack<Types...>> : std::true_type {};

template <typename Type>
concept Types_Pack_Instance = requires()
{
	Is_Types_Pack_Instance<Type>::value;
};

namespace Types_Pack_Details
{
	template <Types_Pack_Instance Instance>
	struct Front_Unpacker;

	template <typename Types_Front, typename... Types_Tail>
	struct Front_Unpacker<Types_Pack<Types_Front, Types_Tail...>>
	{
		using Front = Types_Front;
	};

	template <Types_Pack_Instance Lefts_Pack, Types_Pack_Instance Rights_Pack>
	struct Split_Types_Pack
	{
		constexpr static auto left = Lefts_Pack();
		constexpr static auto right = Rights_Pack();
		constexpr static auto whole = Lefts_Pack() + Rights_Pack();
	};

	template <typename... Lefts, auto = 0, typename... Rights>
	consteval auto split(std::index_sequence<>, Types_Pack<Lefts...>, Types_Pack<Rights...>);

	template <typename... Lefts, auto = 0, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<>, Types_Pack<Lefts...>, Types_Pack<Rights_Front, Rights_Tail...>);

	template <size_t... is, typename... Lefts, auto = 0, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<is...>, Types_Pack<Lefts...>, Types_Pack<Rights_Front, Rights_Tail...>);

	template <typename... Rets, auto = 0, typename... Sources>
	consteval auto get(Values_Pack<size_t>, Types_Pack<Rets...>, Types_Pack<Sources...>);

	template <size_t... indexes, typename... Rets, auto = 0, typename... Sources>
	consteval auto get(Values_Pack<size_t, indexes...>, Types_Pack<Rets...>, Types_Pack<Sources...>);

	template <size_t i, typename... Rets>
	consteval auto erase(Values_Pack<size_t>, Types_Pack<Rets...>);

	template <size_t i, size_t... indexes, typename... Rets>
	consteval auto erase(Values_Pack<size_t, indexes...>, Types_Pack<Rets...>);

	template <typename... Rets>
	consteval auto reverse(Types_Pack<Rets...>, Types_Pack<>);

	template <typename... Rets, auto = 0, typename... Rems>
	consteval auto reverse(Types_Pack<Rets...>, Types_Pack<Rems...>);
}

template <typename... Types>
class Types_Pack
{
public:
	template <size_t index>
	using Unpack = Types_Pack_Details::Front_Unpacker<decltype(pop_front<index>())>::Front;

	consteval Types_Pack() noexcept = default;

	consteval static auto size() noexcept { return sizeof...(Types); }
	consteval static auto is_empty() noexcept { return sizeof...(Types) == 0; }

	template <typename... Concats>
	consteval auto operator+(Types_Pack<Concats...>) const
	{
		return Types_Pack<Types..., Concats...>();
	}

	template <typename... Concats>
	consteval static auto concat(Types_Pack<Concats...>)
	{
		return Types_Pack<Types..., Concats...>();
	}

	template <typename... Concats>
	consteval static auto concat()
	{
		return concat(Types_Pack<Concats...>());
	}

	template <size_t pivot = size() / 2>
	consteval static auto split()
	{
		return Types_Pack_Details::split(std::make_index_sequence<pivot>(), Types_Pack<>(), Types_Pack<Types...>());
	}

	template <size_t... indexes>
	consteval static auto get(Values_Pack<size_t, indexes...> indexes_pack)
	{
		return Types_Pack_Details::get(indexes_pack, Types_Pack<>(), Types_Pack<Types...>());
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
		return Types_Pack_Details::erase<0>(indexes_pack, Types_Pack<Types...>());
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
	consteval static auto insert(Types_Pack<Inserts...> inserts_pack)
	{
		return get_front<index>() + inserts_pack + pop_front<index>();
	}

	template <size_t index, typename... Inserts>
	consteval static auto insert()
	{
		return insert<index>(Types_Pack<Inserts...>());
	}

	template <typename... Pushes>
	consteval static auto push_front(Types_Pack<Pushes...> pushes_pack)
	{
		return pushes_pack + Types_Pack<Types...>();
	}

	template <typename... Pushes>
	consteval static auto push_front()
	{
		return push_front(Types_Pack<Pushes...>());
	}

	template <typename... Pushes>
	consteval static auto push_back(Types_Pack<Pushes...> pushes_pack)
	{
		return Types_Pack<Types...>() + pushes_pack;
	}

	template <typename... Pushes>
	consteval static auto push_back()
	{
		return push_back(Types_Pack<Pushes...>());
	}
	
	consteval static auto reverse()
	{
		return Types_Pack_Details::reverse(Types_Pack<>(), Types_Pack<Types...>());
	}
};

namespace Types_Pack_Details
{
	template <typename... Lefts, auto, typename... Rights>
	consteval auto split(std::index_sequence<>, Types_Pack<Lefts...>, Types_Pack<Rights...>)
	{
		return Split_Types_Pack<Types_Pack<Lefts...>, Types_Pack<Rights...>>();
	}

	template <typename... Lefts, auto, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<>, Types_Pack<Lefts...>, Types_Pack<Rights_Front, Rights_Tail...>)
	{
		return Split_Types_Pack<Types_Pack<Lefts...>, Types_Pack<Rights_Front, Rights_Tail...>>();
	}

	template <size_t... is, typename... Lefts, auto, typename Rights_Front, typename... Rights_Tail>
	consteval auto split(std::index_sequence<is...>, Types_Pack<Lefts...>, Types_Pack<Rights_Front, Rights_Tail...>)
	{
		return split(std::make_index_sequence<sizeof...(is) - 1>(), Types_Pack<Lefts..., Rights_Front>(), Types_Pack<Rights_Tail...>());
	}

	template <typename... Rets, auto, typename... Sources>
	consteval auto get(Values_Pack<size_t>, Types_Pack<Rets...> rets_pack, Types_Pack<Sources...>)
	{
		return rets_pack;
	}

	template <size_t... indexes, typename... Rets, auto, typename... Sources>
	consteval auto get(Values_Pack<size_t, indexes...> indexes_pack, Types_Pack<Rets...> rets_pack, Types_Pack<Sources...> sources_pack)
	{
		return get(indexes_pack.get_tail(), rets_pack + sources_pack.split<indexes_pack.front>().right.split<1>().left, sources_pack);
	}

	template <size_t i, typename... Rets>
	consteval auto erase(Values_Pack<size_t>, Types_Pack<Rets...> rets_pack)
	{
		return rets_pack;
	}

	template <size_t i, size_t... indexes, typename... Rets>
	consteval auto erase(Values_Pack<size_t, indexes...> indexes_pack, Types_Pack<Rets...> rets_pack)
	{
		return erase<i + 1>(indexes_pack.get_tail(), rets_pack.get_front<indexes_pack.front - i>() + rets_pack.pop_front<indexes_pack.front - i + 1>());
	}

	template <typename... Rets>
	consteval auto reverse(Types_Pack<Rets...> rets_pack, Types_Pack<>)
	{
		return rets_pack;
	}

	template <typename... Rets, auto, typename... Rems>
	consteval auto reverse(Types_Pack<Rets...> rets_pack, Types_Pack<Rems...> rems_pack)
	{
		return reverse(rems_pack.get_front() + rets_pack, rems_pack.get_tail());
	}
}
