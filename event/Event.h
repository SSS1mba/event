#pragma once
#include <vector>
#include <functional>
#include <iostream>

namespace gms {
	template <typename ReturnType, typename ... Args>
	class Event
	{
	public:
		using FunctionType = std::function<ReturnType(Args...)>;


		template <typename F>
		void Subscribe(F&& func)
		{
			static_assert(std::is_constructible_v<FunctionType, F>,
				"Function object must be convertible to std::function<ReturnType(Args...)>");

			functions.emplace_back(std::forward<F>(func));
		}

		void Unsubscribe(const FunctionType& func)
		{
			auto iterator = std::find(functions.begin(), functions.end(), func);

			if (iterator == functions.end())
				throw std::runtime_error("Unsubscribe -> not contain this event");

			functions.erase(iterator);
		}

		template <typename... InvokingArgs >
		void Invoke(InvokingArgs&&... args) const noexcept
		{
			static_assert(sizeof...(InvokingArgs) == sizeof...(Args),
				"Number of arguments must match the event signature");
			static_assert((std::is_convertible_v<InvokingArgs, Args> && ...),
				"Argument types must be convertible to the event signature types");


			if (!functions.empty())
			{
				for (const FunctionType& function : functions)
				{
					function(std::forward<InvokingArgs>(args)...);
				}
			}

		}


		template <typename F>
		void operator += (F&& func)
		{
			static_assert(std::is_constructible_v<FunctionType, F>,
				"Function object must be convertible to std::function<ReturnType(Args...)>");

			Subscribe(std::forward<F>(func));
		}
		void operator -= (const FunctionType& func)
		{
			Unsubscribe(func);
		}



	private:
		std::vector<FunctionType> functions;
	};

}