#pragma once

#include <cli/cli.h>
#include "profilemanager.hpp"

class App
{
public:
	App();

	void run();

private:
	ProfileManager m_profileManager;
	cli::Cli m_cli;

	std::unique_ptr<cli::Menu> createRootMenu();
};

