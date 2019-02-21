#include "pch.h"
#include <iostream>
#include "PlayField.h"
#include "PlayBot.h"
#include <conio.h>

using namespace std;
int main()
{
    std::cout << "Welcome to play"<<endl; 



	PlayField* field = new PlayField();
	field->GeneratePlayField(4, 4, 4);
	field->PrintField("main");
	PlayBot* bot = PlayBot::Create(field);
	bot->Run();
	


	return 0;

}


