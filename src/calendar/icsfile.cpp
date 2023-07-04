#include "icsfile.hpp"

#include <sstream>
#include <fstream>
#include "libical/ical.h"

#include <iostream>
#include "helpers.hpp"

char* read_stream(char *s, size_t size, void *d);

ICSFile::ICSFile(const std::string& filePathStr)
{
	parser = icalparser_new();

	stream = fopen(filePathStr.c_str(), "r");
	if (stream == NULL) {
		valid = false;
		return;
	}
	valid = true;

	icalparser_set_gen_data(parser, stream);

	rootComponent = icalparser_parse(parser, read_stream);
}

ICSFile::~ICSFile()
{
	icalparser_free(parser);
}

char* read_stream(char *s, size_t size, void *d)
{
	  return fgets(s, (int)size, (FILE*)d);
}

int ICSFile::yieldNextEvent(Event &event)
{
	if (!valid) {
		return 1;
	}

	currentComponent = nullptr;

	if (!started) {
		started = true;
		currentComponent = icalcomponent_get_first_component(rootComponent, ICAL_VEVENT_COMPONENT);
	} else {
		currentComponent = icalcomponent_get_next_component(rootComponent, ICAL_VEVENT_COMPONENT);
	}
	return _yieldNextEvent(event);
}

int ICSFile::_yieldNextEvent(Event &event)
{
	if (!currentComponent) {
		return 1;
	}

	event.summary = icalcomponent_get_summary(currentComponent);
	icaltimetype tt(icalcomponent_get_dtstart(currentComponent));
	event.ymd = numbers_to_ymd(tt.year, tt.month, tt.day);

	//importing FREQ RRULE
	icalproperty* prop;
	for (prop = icalcomponent_get_first_property(currentComponent, ICAL_RRULE_PROPERTY);
		prop != 0;
		prop = icalcomponent_get_next_property(currentComponent, ICAL_RRULE_PROPERTY))
	{
		struct icalrecurrencetype rt = icalproperty_get_rrule(prop);
		event.frequency = rt.freq;
	}

	return 0;
}
