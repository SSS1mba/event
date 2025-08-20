#pragma once

#include <vector>
#include <functional>
#include <iostream>


#define GMS_EVENT_VERSION_MAJOR 1


namespace gms {
	template <typename ReturnType, typename ... Args>
	class Event
	{
	public:
		using function_pointer = ReturnType(*)(Args...);


		Event() = default;
		~Event() = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		Event(Event&&) noexcept = default;
		Event& operator=(Event&&) noexcept = default;


		void subscribe(const function_pointer func)
		{
			if (func == nullptr) throw std::invalid_argument("Subscribe(func) -> func == nullptr");

			if (is_subscribed(func)) return;
			functions_.emplace_back(func);
		}
		void unsubscribe(const function_pointer& func)
		{
			if (func == nullptr) throw std::invalid_argument("Subscribe(func) -> func == nullptr");

			auto iterator = std::find(functions_.begin(), functions_.end(), func);
			if (iterator == functions_.end())
				throw std::runtime_error("Unsubscribe -> not contain this event");

			functions_.erase(iterator);
		}

		template <typename... InvokingArgs >
		void invoke(InvokingArgs&&... args) const noexcept
		{
			static_assert(sizeof...(InvokingArgs) == sizeof...(Args),
				"Number of arguments must match the event signature");
			static_assert((std::is_convertible_v<InvokingArgs, Args> && ...),
				"Argument types must be convertible to the event signature types");


			if (!functions_.empty())
			{
				for (const auto function : functions_)
				{
					if (function != nullptr)
						function(std::forward<InvokingArgs>(args)...);
				}
			}

		}

		template <typename... InvokingArgs >
		std::vector<ReturnType> invoke_with_results(InvokingArgs&&... args) const noexcept
		{
			static_assert(sizeof...(InvokingArgs) == sizeof...(Args),
				"Number of arguments must match the event signature");
			static_assert((std::is_convertible_v<InvokingArgs, Args> && ...),
				"Argument types must be convertible to the event signature types");

			std::vector<ReturnType> results;
			results.reserve(functions_.size());
			if (!functions_.empty())
			{
				for (const auto function : functions_)
				{
					if (function != nullptr)
						results.emplace_back(function(std::forward<InvokingArgs>(args)...));
				}
			}

			return results;
		}

		
		inline size_t get_functions_number() const noexcept
		{
			return functions_.size();
		}
		bool is_subscribed(const function_pointer func) const noexcept
		{
			if (func == nullptr) return false;

			return !(functions_.end() == std::find(functions_.begin(), functions_.end(), func));
		}
		void clear() noexcept
		{
			functions_.clear();
		}


		Event& operator += (const function_pointer func)
		{
			subscribe(func);
			return *this;
		}
		Event& operator -= (const function_pointer func)
		{
			unsubscribe(func);
			return *this;
		}


	private:
		std::vector<function_pointer> functions_;
	};

}// namespace gms