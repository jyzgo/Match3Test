#include "pch.h"
#include <iostream>
#include "PlayBot.h"
#include <conio.h>

using namespace std;

PlayBot* PlayBot::Create(PlayField * field)
{
	PlayBot* bot = new PlayBot();
	bot->Init(field);
	return bot;
}

PlayBot::PlayBot()
{
}


PlayBot::~PlayBot()
{
}

void PlayBot::Run()
{
	std::cout << "Press anykey to run, Press ESC to exit!" << std::endl;
	char ch; //or 'int ch;' (it doesn't really matter)
	bool exit = false;
	while (!exit) {
		ch = _getch();

		if (ch == 27) {
			exit = true;
		}
		else {
			MoveStep();
		}
	}
}

void PlayBot::MoveStep()
{
	std::vector<MatchMove> possibleMoves = _playField->GetPossibleMoves();
	MatchMove first = possibleMoves[0];
	_playField->Step(first.x, first.y, first.targetX, first.targetY);
	_playField->PrintField("Bot move step Finish");
}

void PlayBot::Init(PlayField* field)
{
	_playField = field;
}
