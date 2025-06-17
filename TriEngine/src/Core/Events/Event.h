#pragma once

#include "tripch.h"
#include "Core/Base/Core.h"

namespace TriEngine {

	enum class EventType : uint8_t {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped, JoyStickConnected,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryJoystick = BIT(5)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const final { return GetStaticType(); }\
									virtual const char* GetName() const final { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool Handled = false;
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	};

		class EventDispatcher
		{
		public:
			EventDispatcher(Event& event)
				: m_Event(event)
			{
			}

			template<typename T, typename F>
			bool Dispatch(const F& func)
			{
				if (m_Event.GetEventType() == T::GetStaticType())
				{
					m_Event.Handled |= func(static_cast<T&>(m_Event));
					return true;
				}
				return false;
			}
		private:
			Event& m_Event;
		};

		inline std::ostream& operator<<(std::ostream& os, const Event& e)
		{
			return os << e.ToString();
		}
	}