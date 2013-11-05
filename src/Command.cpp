#include "Command.h"
#include <iostream>
#include "Script.h"
#include "GameScene.h"
#include "Novel.h"

using namespace VNPSP;

Command::Command(Script *script, char *remainingLine):
	script(script) { }
Command::~Command() { }

// 
// -- Unknown Command
// 
UnknownCommand::UnknownCommand(Script *script, char *line):
	Command(script, line),
	line(line)
{
	
}

UnknownCommand::~UnknownCommand() {}

void UnknownCommand::exec(bool skipping)
{
	this->script->scene->pushLine(std::string("<< ") + line + std::string(" >>"), GameScene::LineTypeSystem);
}

// 
// -- Text
// 
TextCommand::TextCommand(Script *script, char *remainingLine):
	Command(script, remainingLine),
	text(remainingLine)
{
	
}

TextCommand::~TextCommand() { }

void TextCommand::exec(bool skipping)
{
	this->script->scene->pushLine(text);
}

// 
// -- BgLoad
// 
BgLoadCommand::BgLoadCommand(Script *script, char *remainingLine):
	Command(script, remainingLine)
{
	const char *tmp = strtok(remainingLine, " ");
	filename = (tmp ? tmp : remainingLine);
}

BgLoadCommand::~BgLoadCommand() { }

void BgLoadCommand::exec(bool skipping)
{
	
}
