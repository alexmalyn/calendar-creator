#include "profilemanager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <filesystem>

#include "rapidcsv.h"
#include "icsfile.hpp"
#include "helpers.hpp"

constexpr int MAX_PROFILES = 10;

ProfileManager::ProfileManager()
{
	m_profiles.resize(MAX_PROFILES);
	for (auto iter = m_profiles.begin() ; iter != m_profiles.end(); ++iter) {
		*iter = new Profile;
	}
}

ProfileManager::~ProfileManager()
{
	for (auto iter = m_profiles.begin() ; iter != m_profiles.end(); ++iter) {
		delete *iter;
	}
}

void ProfileManager::addEvent(uint32_t profileIndex, const std::string& subject, const std::chrono::year_month_day& ymd, const std::string& _frequency)
{
	profileIndex--;
	Profile* profile = getProfile(profileIndex);
	if (!profile) {
		std::cerr << "Profile index invalid" << std::endl;
		return;
	}

	// validate the recurrency string
	std::string frequencyStr = _frequency;
	std::transform(frequencyStr.begin(), frequencyStr.end(), frequencyStr.begin(),
		[](unsigned char c){ return std::toupper(c); });
	icalrecurrencetype_frequency freq = get_recurrencetype_frequency(frequencyStr);
	if ( freq == ICAL_NO_RECURRENCE) {
		std::cerr << "Invalid recurrence specified" << std::endl;
		return;
	}

	profile->addEvent(subject, ymd, freq);
}

void ProfileManager::removeEvent(uint32_t profileIndex, const std::string& subject)
{
	profileIndex--;
	Profile* profile = getProfile(profileIndex);
	if (!profile) {
		std::cerr << "Profile index invalid" << std::endl;
		return;
	}

	profile->removeEvent(subject);
}

void ProfileManager::importProfile(uint32_t profileIndex, const std::string &filePathStr)
{
	profileIndex--;
	Profile* profile = getProfile(profileIndex);
	if (!profile) {
		std::cerr << "Profile index invalid" << std::endl;
		return;
	}

	if (profile->hasData()) {
		std::cout << "The specified profile has data. Are you sure you want to overwrite? [y/n]" << std::endl;
		std::string answer;
		std::cin >> answer;
		std::transform(answer.begin(), answer.end(), answer.begin(),
			[](unsigned char c){ return std::tolower(c); });
		if (answer != "yes" || answer != "y") {
			return;
		}
	}

	ICSFile file(filePathStr);

	if (!file.isValid()) {
		std::cerr << "File invalid" << std::endl;
		return;
	}

	Event e;
	while (file.yieldNextEvent(e) == 0) {
		profile->addEvent(e);
	}



	//TODO allow csv import, but date format required

//	rapidcsv::Document doc(filePathStr);

//	int rowCount = doc.GetRowCount();
//	for (int i = 0; i < rowCount; ++i) {
//		profile->addEvent(doc.GetCell<std::string>("Subject", i), doc.GetCell<std::string>("Start Date", i));
//	}
}

void ProfileManager::exportProfile(uint32_t profileIndex, const std::string &filePathStr)
{
	profileIndex--;
	Profile* profile = getProfile(profileIndex);
	if (!profile) {
		return;
	}

	if (!profile->hasData()) {
		std::cout << "The specified profile has no data." << std::endl;
		return;
	}

	std::string profileStr = profile->icsString();
	std::ofstream ofs;
	ofs.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	try {
		ofs.open(filePathStr, std::ios::out | std::ios::trunc);
		ofs << profileStr;
		ofs.close();
	} catch (std::ifstream::failure e) {
		std::cerr << "Failed to export the specified file" << std::endl;
		return;
	}

}

void ProfileManager::swapProfiles(uint32_t srcProfileIndex, uint32_t dstProfileIndex)
{
	if (srcProfileIndex == dstProfileIndex) {
		return;
	}

	srcProfileIndex--;
	dstProfileIndex--;

	int i = 0;
	std::vector<Profile*>::iterator srcIterator;
	std::vector<Profile*>::iterator dstIterator;
	bool srcIteratorFound = false, dstIteratorFound = false;


	for (auto iter = m_profiles.begin(); iter != m_profiles.end(); ++iter) {
		if (srcProfileIndex == i) {
			srcIterator = iter;
			srcIteratorFound = true;
			i++;
			continue;
		}

		if (dstProfileIndex == i) {
			dstIterator = iter;
			dstIteratorFound = true;
			i++;
			continue;
		}

		i++;

		if (srcIteratorFound && dstIteratorFound) {
			break;
		}
	}

	std::iter_swap(srcIterator, dstIterator);

}

void ProfileManager::viewProfiles(std::vector<std::string>& profiles)
{
	if (profiles.empty()) {
		for (int i = 0; i < m_profiles.size(); ++i) {
			std::cout << '[' << i+1 << "]:\n";
			m_profiles[i]->view();
		}
	} else {
		for (const auto& profileIndexStr: profiles) {
			Profile* profile = nullptr;
			int profileIndex = std::stoi(profileIndexStr);
			profile = getProfile(profileIndex - 1);
			if (!profile) {
				std::cerr << "Profile index invalid: " << profileIndexStr << std::endl;
				continue;
			}
			std::cout << '[' << profileIndex << "]:\n";
			profile->view();
		}
	}
}

Profile *ProfileManager::getProfile(uint32_t profileIndex)
{
	Profile* requestedProfile = nullptr;
	try {
		requestedProfile = m_profiles.at(profileIndex);
	} catch (std::out_of_range const& exc) {
//		std::cerr << "Profile not in range" << std::endl;
	}
	return requestedProfile;
}

/************************************************************************************************/

Profile::Profile()
{

}

void Profile::addEvent(const std::string &subject, const std::chrono::year_month_day& ymd, const icalrecurrencetype_frequency recurrency)
{
	Event e(subject, ymd, recurrency);
	m_calendar.addEvent(e);

	if (!m_calendar.empty()) {
		m_hasData = true;
	}
}

void Profile::addEvent(const Event &ev)
{
	m_calendar.addEvent(ev);

	if (!m_calendar.empty()) {
		m_hasData = true;
	}
}

void Profile::removeEvent(const std::string &subject)
{
	m_calendar.removeEvent(subject);

	if (m_calendar.empty()) {
		m_hasData = false;
	}
}

std::string Profile::icsString()
{
	const EventList& evList = m_calendar.events();
	std::string res;

	res += "BEGIN:VCALENDAR\n";

	for (const auto& event : evList) {
		res += "BEGIN:VEVENT\n";
		if (event.frequency != ICAL_NO_RECURRENCE) {
			res += "RRULE:FREQ=" + std::string(icalrecur_freq_to_string(event.frequency)) + '\n';
		}
		res += "SUMMARY:" + event.summary + '\n';
		std::string ymdStr = ymd_to_YYYYMMDD_string(event.ymd);
		res += "DTSTART;VALUE=DATE:" + ymdStr + '\n';
		res += "DTEND;VALUE=DATE:" + ymdStr + "\n";
		res += "END:VEVENT\n";
	}

	res += "END:VCALENDAR\n";

	return res;
}

void Profile::view()
{
	if (!hasData()) {
		std::cout << "Nothing to view" << std::endl;
		return;
	}

	std::cout << m_calendar;
}
