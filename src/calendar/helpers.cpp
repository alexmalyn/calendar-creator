#include "helpers.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

std::chrono::year_month_day strings_to_ymd(const std::string& year, const std::string& month, const std::string& day)
{
	return numbers_to_ymd( std::stoi(year),
						   static_cast<unsigned int>(std::stoi(month)),
						   static_cast<unsigned int>(std::stoi(day)) );
}

std::chrono::year_month_day numbers_to_ymd(int year, unsigned int month, unsigned int day)
{
	std::chrono::year y{std::chrono::year{year}};
	std::chrono::month m{std::chrono::month{month}};
	std::chrono::day d{std::chrono::day{day}};
	std::chrono::year_month_day ymd{ y / m / d};
	return ymd;
}

std::string ymd_to_YYYYMMDD_string(const std::chrono::year_month_day &ymd)
{
	std::ostringstream oss;

	oss << std::setw(4) << std::setfill('0') << static_cast<int>(ymd.year());
	oss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(ymd.month());
	oss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(ymd.day());

	return oss.str();
}

icalrecurrencetype_frequency get_recurrencetype_frequency(const std::string &str)
{
	static const std::set<std::string> frequencyStrSet{"HOURLY", "DAILY", "WEEKLY", "MONTHLY", "YEARLY"};

	std::set<std::string>::iterator it = frequencyStrSet.find(str);
	if (it == frequencyStrSet.end()) {
		return ICAL_NO_RECURRENCE;
	}
	return icalrecur_string_to_freq(str.c_str());
}
