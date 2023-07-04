#pragma once

#include "event.hpp"
#include <list>
#include <unordered_map>
#include <ostream>

class Calendar
{
public:
	Calendar();

	void addEvent(const Event& event);
	void removeEvent(const std::string& subject);

	bool empty();

	const EventList& events() const { return m_events; }

	friend std::ostream& operator<<(std::ostream& os, const Calendar& dt);

private:
	EventList m_events;
	std::unordered_map<std::string, uint32_t> m_subjectMap;
};

