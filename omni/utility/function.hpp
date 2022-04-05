#pragma once

namespace Omni
{
	template<typename Ret, typename... Args>
	class Function
	{
	public:
		constexpr Function(Ret(*context_fn_ptr)(void*, Args...), void* context) noexcept
			: m_context_fn_ptr(context_fn_ptr), m_context(context)
		{
		}

		~Function() noexcept = default;

		template<auto invokable>
		constexpr static auto make() -> Function<Ret, Args...>
		{
			return Function<Ret, Args...>(&pv_invoke_literal<invokable>, nullptr);
		}

		constexpr static auto make(Ret(*fn_ptr)(Args...)) noexcept -> Function<Ret, Args...>
		{
			return Function<Ret, Args...>(&pv_invoke_context<Ret(*)(Args...)>, fn_ptr);
		}

		template<typename Functor>  
		constexpr static auto make(Functor& functor) -> Function<Ret, Args...>
		{
			return Function<Ret, Args...>(&pv_invoke_context<Functor>, &functor);
		}

		auto operator()(Args ... args) const -> Ret
		{
			return m_context_fn_ptr(m_context, args...);
		}

	private:
		template<auto invokable>
		constexpr static auto pv_invoke_literal(void*, Args ... args) -> Ret
		{
			return invokable(args...);
		}

		template<typename Invokable>
		static auto pv_invoke_context(void* context, Args ... args) -> Ret
		{
			return (*(Invokable*)context)(args...);
		}
		
		template<>
		static auto pv_invoke_context<Ret(*)(Args...)>(void* context, Args ... args) -> Ret
		{
			return ((Ret(*)(Args...))context)(args...);
		}

		Ret(* m_context_fn_ptr)(void*, Args...);
		void* m_context;
	};
}