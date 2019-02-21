#pragma once
#include "PlayField.h"
class PlayBot
{
public:
	static PlayBot* Create(PlayField* field);
public:
	void Init(PlayField * field);

	void Run();
	void MoveStep();
private:
	PlayBot();
	~PlayBot();
	PlayField* _playField;
};

