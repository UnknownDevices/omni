#pragma once

namespace Omni
{
    template <typename Value, size_t size>
    class Shuffle_Bag
    {
    public:
        using Array = std::array<Value, size>;

        using Iterator = std::array<Value, size>::iterator;
        using Const_Iterator = std::array<Value, size>::const_iterator;

        constexpr 
        Shuffle_Bag(const Array &values) : m_values(values), m_next_draw(m_values.begin()) {}
        constexpr
        Shuffle_Bag(Array &&values) : m_values(std::move(values)), m_next_draw(m_values.begin()) {}

        consteval [[nodiscard]]  
        static auto get_size() noexcept -> Const_Iterator { return size; }

        constexpr [[nodiscard]] 
        auto cbegin() const noexcept -> Const_Iterator { return m_values.cbegin(); }
        constexpr [[nodiscard]] 
        auto cend() const noexcept -> Const_Iterator { return m_values.cend(); }

        constexpr [[nodiscard]] 
        auto get_next_draw() const noexcept -> Const_Iterator { return m_next_draw; }

        auto draw() -> Value
        {
            if ((m_values.end() - m_next_draw) <= 0)
            {
                m_next_draw = m_values.begin();
                shuffle();
            }

            return *(m_next_draw++);
        }

        auto shuffle() -> void
        {
        }

    private:
        Array m_values;
        Iterator m_next_draw;
    };
}
