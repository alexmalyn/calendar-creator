#pragma once

#include "calendar.hpp"
#include "libical/ical.h"

#include <vector>
#include <string>

class Profile {
public:
	Profile();

	void addEvent(const std::string& subject, const std::chrono::year_month_day& ymd, const icalrecurrencetype_frequency recurrency);
	void addEvent(const Event& ev);
	void removeEvent(const std::string& subject);

	std::string icsString();

	void view();

	bool hasData() { return m_hasData; }

private:
	Calendar m_calendar;

	bool m_hasData = false;

};

class ProfileManager
{
public:
	ProfileManager();
	~ProfileManager();

	// Events
	void addEvent(uint32_t profileIndex, const std::string& subject, const std::chrono::year_month_day& ymd, const std::string& recurrency = "");
	void removeEvent(uint32_t profileIndex, const std::string& subject);

	// Profiles
	void importProfile(uint32_t profileIndex, const std::string& filePathStr);
	void exportProfile(uint32_t profileIndex, const std::string& filePathStr);
	void swapProfiles(uint32_t srcProfileIndex, uint32_t dstProfileIndex);

	// can view a specific profile or ALL profiles
	void viewProfiles(std::vector<std::string>& profile);

private:
	std::vector<Profile*> m_profiles;

	Profile* getProfile(uint32_t profileIndex);
};

