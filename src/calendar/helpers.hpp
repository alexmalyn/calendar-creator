#pragma once

#include <chrono>
#include <string>
#include "libical/ical.h"

std::chrono::year_month_day strings_to_ymd(const std::string& year, const std::string& month, const std::string& day);

std::chrono::year_month_day numbers_to_ymd(int year, unsigned int month, unsigned int day);

std::string ymd_to_YYYYMMDD_string(const std::chrono::year_month_day& ymd);

icalrecurrencetype_frequency get_recurrencetype_frequency(const std::string& str);
