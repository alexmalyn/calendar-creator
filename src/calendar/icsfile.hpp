#pragma once

#include "event.hpp"
#include "libical/ical.h"

class ICSFile
{
public:
	ICSFile(const std::string& filePathStr);
	~ICSFile();

	bool isValid() { return valid; }

	int yieldNextEvent(Event& event);

private:
	FILE* stream;
	icalparser* parser;
	icalcomponent* rootComponent;
	icalcomponent* currentComponent;

	int _yieldNextEvent(Event& event);

	bool started = false;
	bool valid = false;


};
