#pragma once

#include "Battery/pch.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Event.h"
#include "Battery/AllegroDeps.h"
#include "Battery/StringUtils.h"

#undef CreateEvent

namespace Battery {

	class WindowCloseEvent : public Battery::Event {
	public:
		WindowCloseEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowClose, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_INFO_STRING("");
	};

	class WindowResizeEvent : public Battery::Event {
	public:
		WindowResizeEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowResize, event);
			width = event->display.width;
			height = event->display.height;
			windowPosition = glm::ivec2(event->display.x, event->display.y);
		}

		int width = 0;
		int height = 0;
		glm::ivec2 windowPosition;

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_INFO_STRING(" width=%d height=%d windowPosition: (%d, %d)", 
			width, height, windowPosition.x, windowPosition.y
		);
	};

	class WindowFocusEvent : public Battery::Event {
	public:
		WindowFocusEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowFocus, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_INFO_STRING("");
	};

	class WindowLostFocusEvent : public Battery::Event {
	public:
		WindowLostFocusEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowLostFocus, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_INFO_STRING("");
	};

	class WindowMouseEnteredEvent : public Battery::Event {
	public:
		WindowMouseEnteredEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowMouseEntered, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowMouseEntered);
		EVENT_INFO_STRING("");
	};

	class WindowMouseLeftEvent : public Battery::Event {
	public:
		WindowMouseLeftEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::WindowMouseLeft, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowMouseLeft);
		EVENT_INFO_STRING("");
	};

	class KeyPressedEvent : public Battery::Event {
	public:
		KeyPressedEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::KeyPressed, event);
			display = event->keyboard.display;
			keycode = event->keyboard.keycode;
			repeat = event->keyboard.repeat;
			modifiers = event->keyboard.modifiers;
		}

		int keycode;
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(KeyPressed);
		EVENT_INFO_STRING("KeyCode: %d Modifiers: %d Allegro display: %p Repeat: %s", 
			keycode, modifiers, display, repeat ? "true" : "false");
	};

	class KeyReleasedEvent : public Battery::Event {
	public:
		KeyReleasedEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::KeyReleased, event);
			display = event->keyboard.display;
			keycode = event->keyboard.keycode;
			repeat = event->keyboard.repeat;
			modifiers = event->keyboard.modifiers;
		}

		int keycode;
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(KeyReleased);
		EVENT_INFO_STRING("KeyCode: %d Modifiers: %d Allegro display: %p Repeat: %s",
			keycode, modifiers, display, repeat ? "true" : "false");
	};

	class TextInputEvent : public Battery::Event {
	public:
		TextInputEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::TextInput, event);
			display = event->keyboard.display;
			codepoint = event->keyboard.unichar;
			repeat = event->keyboard.repeat;
			modifiers = event->keyboard.modifiers;
			StringUtils::ConvertCodepointToUTF8(codepoint, character);
		}

		int32_t codepoint;
		char character[5];
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(TextInput);
		EVENT_INFO_STRING("Unicode character: '%s' Unicode codepoint: U+%x Modifiers: %d Allegro display: %p Repeat: %s",
			character, codepoint, modifiers, display, repeat ? "true" : "false");
	};
	
	class MouseButtonPressedEvent : public Battery::Event {
	public:
		MouseButtonPressedEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::MouseButtonPressed, event);
			x = event->mouse.x; y = event->mouse.y; z = event->mouse.z; 
			w = event->mouse.w; button = event->mouse.button; 
			pressure = event->mouse.pressure; display = event->mouse.display;
		}

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonPressed);
		EVENT_INFO_STRING(" x=%d y=%d z=%d w=%d button=%d pressure=%f display=%p",
			x, y, z, w, button, pressure, display
		);
	};

	class MouseButtonReleasedEvent : public Battery::Event {
	public:
		MouseButtonReleasedEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::MouseButtonReleased, event);
			x = event->mouse.x; y = event->mouse.y; z = event->mouse.z;
			w = event->mouse.w; button = event->mouse.button;
			pressure = event->mouse.pressure; display = event->mouse.display;
		}

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonReleased);
		EVENT_INFO_STRING(" x=%d y=%d z=%d w=%d button=%d pressure=%f display=%p",
			x, y, z, w, button, pressure, display
		);
	};

	class MouseMovedEvent : public Battery::Event {
	public:
		MouseMovedEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::MouseMoved, event);
			x = event->mouse.x; y = event->mouse.y; dx = event->mouse.dx; dy = event->mouse.dy;
			pressure = event->mouse.pressure; display = event->mouse.display;
		}

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_INFO_STRING(" x=%d y=%d dx=%d dy=%d pressure=%f display=%p",
			x, y, dx, dy, pressure, display
		);
	};

	class MouseScrolledEvent : public Battery::Event {
	public:
		MouseScrolledEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::MouseScrolled, event);
			x = event->mouse.w; y = event->mouse.z; dx = event->mouse.dw; dy = event->mouse.dz;
			pressure = event->mouse.pressure; display = event->mouse.display;
		}

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_INFO_STRING(" x=%d y=%d dx=%d dy=%d pressure=%f display=%p",
			x, y, dx, dy, pressure, display
		);
	};

	class PureAllegroEvent : public Battery::Event {
	public:
		PureAllegroEvent() {}

		void Load(ALLEGRO_EVENT* event) {
			Battery::Event::Load(EventType::PureAllegro, event);
		}

		// These are for debugging only
		EVENT_CLASS_TYPE(Event not handled by Battery);
		EVENT_INFO_STRING("");
	};

	class ParentEventContainer {
	public:
		ParentEventContainer() {}

		void Load(ALLEGRO_EVENT* allegroEvent) {

			primaryEventType = EventType::None;
			secondaryEventType = EventType::None;
			primaryEvent = nullptr;
			secondaryEvent = nullptr;

			if (allegroEvent == nullptr)
				return;

			switch (allegroEvent->type) {

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				windowCloseEvent.Load(allegroEvent); 
				primaryEvent = &windowCloseEvent;
				primaryEventType = EventType::WindowClose;
				return;

			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				windowResizeEvent.Load(allegroEvent);
				primaryEvent = &windowResizeEvent;
				primaryEventType = EventType::WindowResize;
				return;

				case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
				windowFocusEvent.Load(allegroEvent);
				primaryEvent = &windowFocusEvent;
				primaryEventType = EventType::WindowFocus;
				return;

			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
				windowLostFocusEvent.Load(allegroEvent);
				primaryEvent = &windowLostFocusEvent;
				primaryEventType = EventType::WindowLostFocus;
				return;

			case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
				windowMouseEnteredEvent.Load(allegroEvent);
				primaryEvent = &windowMouseEnteredEvent;
				primaryEventType = EventType::WindowMouseEntered;
				return;

			case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
				windowMouseLeftEvent.Load(allegroEvent);
				primaryEvent = &windowMouseLeftEvent;
				primaryEventType = EventType::WindowMouseLeft;
				return;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				mouseButtonPressedEvent.Load(allegroEvent);
				primaryEvent = &mouseButtonPressedEvent;
				primaryEventType = EventType::MouseButtonPressed;
				return;

			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				mouseButtonReleasedEvent.Load(allegroEvent);
				primaryEvent = &mouseButtonReleasedEvent;
				primaryEventType = EventType::MouseButtonReleased;
				return;

			case ALLEGRO_EVENT_KEY_DOWN:
				keyPressedEvent.Load(allegroEvent);
				primaryEvent = &keyPressedEvent;
				primaryEventType = EventType::KeyPressed;
				return;

			case ALLEGRO_EVENT_KEY_UP:
				keyReleasedEvent.Load(allegroEvent);
				primaryEvent = &keyReleasedEvent;
				primaryEventType = EventType::KeyReleased;
				return;

			case ALLEGRO_EVENT_KEY_CHAR:
				textInputEvent.Load(allegroEvent);
				primaryEvent = &textInputEvent;
				primaryEventType = EventType::TextInput;
				return;

			case ALLEGRO_EVENT_MOUSE_AXES:

				if (allegroEvent->mouse.dx != 0 || allegroEvent->mouse.dy != 0) {
					mouseMovedEvent.Load(allegroEvent);
					primaryEvent = &mouseMovedEvent;
					primaryEventType = EventType::MouseMoved;
				}

				if (allegroEvent->mouse.dz != 0 || allegroEvent->mouse.dw != 0) {
					mouseScrolledEvent.Load(allegroEvent);
					if (primaryEventType == EventType::MouseMoved) {
						secondaryEvent = &mouseScrolledEvent;
						secondaryEventType = EventType::MouseScrolled;
					}
					else {
						primaryEvent = &mouseScrolledEvent;
						primaryEventType = EventType::MouseScrolled;
					}
				}
				return;

			default:
				pureAllegroEvent.Load(allegroEvent);
				primaryEvent = &pureAllegroEvent;
				primaryEventType = EventType::PureAllegro;
				return;
			}
		}

		WindowCloseEvent			windowCloseEvent;
		WindowResizeEvent			windowResizeEvent;
		WindowFocusEvent			windowFocusEvent;
		WindowLostFocusEvent		windowLostFocusEvent;
		WindowMouseEnteredEvent		windowMouseEnteredEvent;
		WindowMouseLeftEvent		windowMouseLeftEvent;
		MouseButtonPressedEvent		mouseButtonPressedEvent;
		MouseButtonReleasedEvent	mouseButtonReleasedEvent;
		KeyPressedEvent				keyPressedEvent;
		KeyReleasedEvent			keyReleasedEvent;
		TextInputEvent				textInputEvent;
		MouseMovedEvent				mouseMovedEvent;
		MouseScrolledEvent			mouseScrolledEvent;
		PureAllegroEvent			pureAllegroEvent;

		enum class EventType primaryEventType = EventType::None;
		enum class EventType secondaryEventType = EventType::None;
		Battery::Event* primaryEvent = nullptr;
		Battery::Event* secondaryEvent = nullptr;
	};

}
