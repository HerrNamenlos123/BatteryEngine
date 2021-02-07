#pragma once

#include "Battery/pch.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Event.h"
#include "Battery/AllegroDeps.h"

#undef CreateEvent

namespace Battery {

	/// <summary>
	/// Returns a Battery::Event* object, which must be deleted manually.
	/// </summary>
	std::vector<Event*> CreateEvent(ALLEGRO_EVENT* event);

	class WindowResizeEvent : public Battery::Event {
	public:
		WindowResizeEvent(ALLEGRO_EVENT* event) : Event(EventType::WindowResize) {
			width = event->display.width;
			height = event->display.height;
			windowPosition = glm::ivec2(event->display.x, event->display.y);
			SetAllegroEvent(event);
		}

		int width = 0;
		int height = 0;
		glm::ivec2 windowPosition;

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_INFO_STRING(" width=" << width << " height=" << height
				<< " windowPosition: (" << windowPosition.x << ", " << windowPosition.y << ")"
		);
	};

	class WindowCloseEvent : public Battery::Event {
	public:
		WindowCloseEvent(ALLEGRO_EVENT* event) : Event(EventType::WindowClose) {
			SetAllegroEvent(event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_INFO_STRING("");
	};
	
	class MouseButtonPressedEvent : public Battery::Event {
	public:
		MouseButtonPressedEvent(ALLEGRO_EVENT* event)
		: Event(EventType::MouseButtonPressed) {
			x = event->mouse.x; y = event->mouse.y; z = event->mouse.z; 
			w = event->mouse.w; button = event->mouse.button; 
			pressure = event->mouse.pressure; display = event->mouse.display;
			SetAllegroEvent(event);
		}

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonPressed);
		EVENT_INFO_STRING(" x=" << x << " y=" << y << " z=" << z << " w=" << w
			<< " button=" << button << " pressure=" << pressure << " display=" << display
		);
	};

	class MouseButtonReleasedEvent : public Battery::Event {
	public:
		MouseButtonReleasedEvent(ALLEGRO_EVENT* event)
			: Event(EventType::MouseButtonReleased) {
			x = event->mouse.x; y = event->mouse.y; z = event->mouse.z;
			w = event->mouse.w; button = event->mouse.button;
			pressure = event->mouse.pressure; display = event->mouse.display;
			SetAllegroEvent(event);
		}

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonReleased);
		EVENT_INFO_STRING(" x=" << x << " y=" << y << " z=" << z << " w=" << w
			<< " button=" << button << " pressure=" << pressure << " display=" << display
		);
	};

	class MouseMovedEvent : public Battery::Event {
	public:
		MouseMovedEvent(ALLEGRO_EVENT* event)
			: Event(EventType::MouseMoved) {
			x = event->mouse.x; y = event->mouse.y; dx = event->mouse.dx; dy = event->mouse.dy;
			pressure = event->mouse.pressure; display = event->mouse.display;
			SetAllegroEvent(event);
		}

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_INFO_STRING(" x=" << x << " y=" << y << " dx=" << dx << " dy=" << dy
							<< " pressure=" << pressure << " display=" << display
		);
	};

	class MouseScrolledEvent : public Battery::Event {
	public:
		MouseScrolledEvent(ALLEGRO_EVENT* event)
			: Event(EventType::MouseScrolled) {
			x = event->mouse.w; y = event->mouse.z; dx = event->mouse.dw; dy = event->mouse.dz;
			pressure = event->mouse.pressure; display = event->mouse.display;
			SetAllegroEvent(event);
		}

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_INFO_STRING(" x=" << x << " y=" << y << " dx=" << dx << " dy=" << dy
			<< " pressure=" << pressure << " display=" << display
		);
	};

	class PureAllegroEvent : public Battery::Event {
	public:
		PureAllegroEvent(ALLEGRO_EVENT* event) : Event(EventType::None) {
			SetAllegroEvent(event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(No specific event);
		EVENT_INFO_STRING("");
	};

}
