#include "Script.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "GameScene.h"
#include "Novel.h"
#include "Command.h"

using namespace VNPSP;

Script::Script(Novel *novel, std::string filename):
	novel(novel),
	pos(0)
{
	this->load(filename);
}

Script::~Script()
{
	for(std::deque<Command*>::iterator it = commands.begin(); it != commands.end(); it++)
		delete *it;
}

void Script::next(bool skipping)
{
	bool cont = false;
	do
	{
		cont = false;
		
		if(pos >= commands.size())
		{
			std::cout << "-- Reached the end of the script!" << std::endl;
		}
		else
		{
			std::cout << "-- Executing Command " << pos << " of " << commands.size() << "..." << std::endl;
			Command *cmd = commands.at(pos);
			cont = cmd->exec(skipping);
			++pos;
		}
	} while(cont);
}

void Script::load(std::string filename)
{
	std::string path = novel->pathForResource("script", filename);
	std::ifstream file(path.c_str());
	
	std::string line;
	while(std::getline(file, line))
	{
		char *cLine = strdup(line.c_str());
		
		// Skip empty lines and comments
		if(strlen(cLine) == 0 || cLine[0] == '#')
			continue;
		
		// Figure out how long the command is, and what it is
		char *pbrk = strpbrk(cLine, " \t\r");
		size_t cmdLength = (pbrk != NULL ? pbrk - cLine : strlen(cLine));
		char *remaining = cLine + (pbrk != NULL ? cmdLength + 1 : 0);
		
		if(strncmp("text", cLine, cmdLength) == 0)
			commands.push_back(new TextCommand(this, remaining));
		else if(strncmp("bgload", cLine, cmdLength) == 0)
			commands.push_back(new BgLoadCommand(this, remaining));
		else
			commands.push_back(new UnknownCommand(this, cLine));
		
		free(cLine);
	}
}
