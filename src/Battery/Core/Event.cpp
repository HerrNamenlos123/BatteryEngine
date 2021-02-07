
#include "Battery/pch.h"
#include "Battery/Core/Event.h"
#include "Battery/Log/Log.h"

namespace Battery {

	Event::Event(enum class EventType type) {
		eventType = type;
	}

	Event::~Event() {

	}

	enum class EventType Event::GetType() {
		return eventType;
	}

	bool Event::WasHandled() {
		return handled;
	}

	void Event::SetHandled(bool h) {
		handled = h;
	}

	void Event::SetAllegroEvent(ALLEGRO_EVENT* event) {
		this->event = event;

		if (event == nullptr) {
			LOG_CORE_WARN("Event: The supplied Allegro Event was nullptr");
		}
	}

	ALLEGRO_EVENT* Event::GetAllegroEvent() {
		return event;
	}

}