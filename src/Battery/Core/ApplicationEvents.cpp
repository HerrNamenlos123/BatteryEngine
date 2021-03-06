#pragma once

#include "Battery/pch.h"
#include "Battery/Core/ApplicationEvents.h"

namespace Battery {

	std::vector<Event*> CreateEvent(ALLEGRO_EVENT* event) {

		if (event == nullptr)
			return { nullptr };

		switch (event->type) {

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			return { new WindowCloseEvent(event) };

		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			return { new WindowResizeEvent(event) };

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			return { new MouseButtonPressedEvent(event) };

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			return { new MouseButtonReleasedEvent(event) };

		case ALLEGRO_EVENT_KEY_DOWN:
			return { new KeyPressedEvent(event) };

		case ALLEGRO_EVENT_KEY_UP:
			return { new KeyReleasedEvent(event) };

		case ALLEGRO_EVENT_KEY_CHAR:
			return { new TextInputEvent(event) };

		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			std::vector<Event*> e;
			if (event->mouse.dx != 0 || event->mouse.dy != 0) {
				e.push_back(new MouseMovedEvent(event));
			}
			else if (event->mouse.dz != 0 || event->mouse.dw != 0) {
				e.push_back(new MouseScrolledEvent(event));
			}
			return e;
		}

		default:
			return { new PureAllegroEvent(event) };
		}
	}

}
