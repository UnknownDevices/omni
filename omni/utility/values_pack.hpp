#pragma once

template<typename Value, Value ... vals>
class Values_Pack;

template<typename Value, Value vals_front, Value ... vals_tail>
class Values_Pack<Value, vals_front, vals_tail...>
{
public:
	consteval
	Values_Pack() noexcept = default;

	constexpr static auto array = std::array<Value, sizeof...(vals_tail) + 1>({vals_front, vals_tail...});
	constexpr static auto front = vals_front;
	
	template<Value ... concats> consteval
	auto operator+(Values_Pack<Value, concats...>) const
	{
			return Values_Pack<Value, vals_front, vals_tail..., concats...>();
	}

	/*template<size_t ... indexes, auto = 0, Value ... filters>
	static auto filter(Values_Pack<size_t, indexes...>, Values_Pack<Value, filters...>)
	{
		return 
	}*/

	consteval
	static auto get_front()
	{
		return Values_Pack<Value, vals_front>();
	}

	consteval
	static auto get_tail()
	{
		return Values_Pack<Value, vals_tail...>();
	}
};

template<typename Value>
class Values_Pack<Value>
{
public:
	consteval
	Values_Pack() noexcept = default;

	constexpr static auto array = std::array<Value, 0>();

	template<Value ... concats> consteval
	auto operator+(Values_Pack<Value, concats...>) const
	{
			return Values_Pack<Value, concats...>();
	}
	
	consteval
	static auto get_front()
	{
		return Values_Pack<Value>();
	}

	consteval
	static auto get_tail()
	{
		return Values_Pack<Value>();
	}
};

template<typename Integer_Sequence>
struct Values_Pack_Inst_From_Integer_Sequence_Inst; 

template<typename Integer, Integer ... integers>
struct Values_Pack_Inst_From_Integer_Sequence_Inst<std::integer_sequence<Integer, integers...>>
{
	using Pack_Inst = Values_Pack<Integer, integers...>;
	using Sequence_Inst = std::integer_sequence<Integer, integers...>;
};

template<typename Value, size_t size, std::array<Value, size> arr, auto = 0, Value ... rets> consteval
auto make_values_pack_from_array(Values_Pack<size_t>, Values_Pack<Value, rets...>)
{
	return Values_Pack<Value, rets...>();
}

template<typename Value, size_t size, std::array<Value, size> arr, auto = 0, size_t ... indexes, auto = 0, Value ... rets> consteval
auto make_values_pack_from_array(Values_Pack<size_t, indexes...> indexes_pack, Values_Pack<Value, rets...>)
{
	return make_values_pack_from_array<Value, size, arr>(indexes_pack.get_tail(), Values_Pack<Value, rets..., arr.at(indexes_pack.front)>());
}

template<typename Value, size_t size, std::array<Value, size> arr> consteval
auto make_values_pack_from_array()
{
	return make_values_pack_from_array<Value, size, arr>(
		Values_Pack_Inst_From_Integer_Sequence_Inst<decltype(std::make_index_sequence<size>())>::Pack_Inst(), Values_Pack<Value>());
}