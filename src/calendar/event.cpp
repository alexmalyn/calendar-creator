#include "event.hpp"

Event::Event()
	: ymd{}
{

}

Event::Event(const std::string& _subject, const std::chrono::year_month_day& _ymd, const icalrecurrencetype_frequency _frequency)
	: summary(_subject)
	, ymd(_ymd)
	, frequency(_frequency)
{
}

std::ostream& operator<<(std::ostream& os, const Event& ev)
{
	os << "Event: " << ev.summary;
	os << " Start Date: " << static_cast<int>(ev.ymd.year())<< '-' << static_cast<unsigned int>(ev.ymd.month()) << '-' << static_cast<unsigned int>(ev.ymd.day());
	if (ev.frequency != ICAL_NO_RECURRENCE) {
		os << " Frequency: " << icalrecur_freq_to_string(ev.frequency);
	}
	return os;
}
