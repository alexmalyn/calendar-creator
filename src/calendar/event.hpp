#pragma once

#include <string>
#include <list>
#include <ostream>

#include "libical/ical.h"

class Event
{
public:
	Event();
	Event(const std::string& subject, const std::chrono::year_month_day& ymd, const icalrecurrencetype_frequency frequency);

	std::string summary;
	std::chrono::year_month_day ymd;
	icalrecurrencetype_frequency frequency = ICAL_NO_RECURRENCE;

	friend std::ostream& operator<<(std::ostream& os, const Event& ev);
};

using EventList = std::list<Event>;

