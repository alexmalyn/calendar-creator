#include "calendar.hpp"

#include <iostream>

Calendar::Calendar()
{

}

void Calendar::addEvent(const Event &event)
{
	m_events.push_back(event);
	m_subjectMap[event.summary] = m_events.size() - 1;
}

void Calendar::removeEvent(const std::string &summary)
{
	auto iter = m_subjectMap.find(summary);

	if (iter == m_subjectMap.end()) {
		std::cerr << "Subject does not exist in this profile" << std::endl;
		return;
	}

	auto eventsIter = m_events.begin();
	int eventIdx =  (*iter).second;
	while (eventIdx) {
		eventsIter++;
		eventIdx--;
	}
	m_events.erase(eventsIter);
	m_subjectMap.erase(iter);
}

std::ostream& operator<<(std::ostream& os, const Calendar& cal)
{
	for (const auto& event : cal.events()) {
		os << event << '\n';
	}
	return os;
}

bool Calendar::empty()
{
	return m_events.empty();
}
