#pragma once

#include "Battery/Core/Config.h"

#ifdef BATTERY_DEBUG
#define EVENT_CLASS_TYPE_BASE(type) virtual std::string GetTypeString() { return #type; }
#define EVENT_CLASS_TYPE(type) std::string GetTypeString() override { return #type; }
#define EVENT_INFO_STRING_BASE(_string) virtual std::string GetInfoString() { \
	std::stringstream ss;	\
	ss << GetTypeString() << ": " << _string;	\
	return ss.str(); \
}
#define EVENT_INFO_STRING(_string) std::string GetInfoString() override { \
	std::stringstream ss;	\
	ss << GetTypeString() << ": " << _string;	\
	return ss.str(); \
}
#else
#define EVENT_CLASS_TYPE_BASE(type)
#define EVENT_CLASS_TYPE(type)
#define EVENT_INFO_STRING_BASE(string)
#define EVENT_INFO_STRING(string)
#endif

namespace Battery {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event {
	public:
		Event(enum class EventType type);
		virtual ~Event();
		
		enum class EventType GetType();
		bool WasHandled();
		void SetHandled(bool h = true);
		void SetAllegroEvent(ALLEGRO_EVENT* event);
		ALLEGRO_EVENT* GetAllegroEvent();

		// These are for debugging only
		EVENT_CLASS_TYPE_BASE(None);
		EVENT_INFO_STRING_BASE("No specific event data");

	protected:

		template<typename... T>
		std::string p_Str(T... msg) {
			return std::to_string(msg...);
		}

	private:
		enum class EventType eventType = EventType::None;
		bool handled = false;
		ALLEGRO_EVENT* event;
	};

}
