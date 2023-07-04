#include "app.hpp"
#include <cli/clifilesession.h>
#include <iostream>

#include "date.h"
#include "helpers.hpp"
#include <chrono>

using namespace cli;

App::App()
	: m_cli(std::move(createRootMenu()))
{
	SetColor();
	m_cli.EnterAction( [](auto& out){ out << "calendar-creator\nType help to get started.\n"; });
}

void App::run()
{
	CliFileSession input(m_cli);
	input.Start();
}

std::unique_ptr<Menu> App::createRootMenu()
{
	auto rootMenu = std::make_unique<Menu >( "creator" );

	auto eventMenu = std::make_unique< Menu >( "event" );
	eventMenu->Insert(
				"add", {"profile index", "event summary", "year", "month", "day"},
				[this](std::ostream& out, uint32_t profile, std::string subject, std::string year, std::string month, std::string day)
				{
					m_profileManager.addEvent(profile, subject, strings_to_ymd(year,month,day));
				},
				"Add an event to a calendar profile");
	eventMenu->Insert(
				"add-recurring", {"profile index", "event summary", "year", "month", "day", "frequency"},
				[this](std::ostream& out, uint32_t profile, std::string subject, std::string year, std::string month, std::string day, std::string frequency)
				{
					m_profileManager.addEvent(profile, subject, strings_to_ymd(year,month,day), frequency);
				},
				"Add an event to a calendar profile");
	eventMenu->Insert(
				"remove", {"profile index", "event summary"},
				[this](std::ostream& out, uint32_t profile, std::string subject)
				{
					m_profileManager.removeEvent(profile, subject);
				},
				"Remove an event from a calendar profile");

	auto profileMenu = std::make_unique< Menu >( "profile" );
	profileMenu->Insert(
				"import", {"profile index", "file path"},
				[this](std::ostream& out,  uint32_t profile, std::string filePathStr)
				{
					m_profileManager.importProfile(profile, filePathStr);
				},
				"Import calendar");
	profileMenu->Insert(
				"export", {"profile index", "file path"},
				[this](std::ostream& out, uint32_t profile, std::string filePathStr)
				{
					m_profileManager.exportProfile(profile, filePathStr);
				},
				"Export calendar");
	profileMenu->Insert(
				"swap", {"profile index 1", "profile index 2"},
				[this](std::ostream& out, uint32_t srcProfile, uint32_t dstProfile)
				{
					m_profileManager.swapProfiles(srcProfile, dstProfile);
				},
				"Swap calendar profiles");
	profileMenu->Insert(
				"view", {"profile indices"},
				[this](std::ostream& out, std::vector<std::string> profiles)
				{
					m_profileManager.viewProfiles(profiles);
				},
				"View calendar profiles\nType no indices to view them all");

	rootMenu->Insert(std::move(eventMenu));
	rootMenu->Insert(std::move(profileMenu));

	return rootMenu;
}
