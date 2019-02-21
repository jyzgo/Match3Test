#pragma once

#include<vector>
#include<random>
using namespace std;

struct MatchMove
{
	int x;
	int y;
	int targetX;
	int targetY;		
};

struct Cell {
	int x;
	int y;
};

class PlayField
{
public:
	void GeneratePlayField(const int& width,const int& height,const int&numTypes);
	void PrintField(string str);
	//Retrives the color of a cell
	const int GetColor(const int& column, const int& row);

	void SwapColors(const Cell& fromCell, const Cell& toCell);
	void SwapColors(const int& sourceX, const int& sourceY, const int& targetX, const int& targetY);
	//
	void Populate();

	void DetectColorsSequences();

	void SequenceFound(int x, int y);

	void DetectPossibleMoves();

	void AddMoves(int fromx, int fromy, int tarX, int tarY);

	bool IsMatched(Cell cell, Cell originCell);

	bool IsMatched(int x, int y,int originX, int originY);

	bool ContainsPossibleMoves();

	const bool ContainsSequences();

	bool DropColors();

	bool Step(Cell fromCell, Cell toCell);
	bool Step(int fromX, int fromY, int toX, int toY);

	void FindAndRemoveSequences();

	void RemoveSequences();

	const std::vector<MatchMove >& GetPossibleMoves() const;

	// returns the amount of columns
	const int& GetColumns();
	// returns the amount of rows
	const int& GetRows();
	// returns amount of holes
	const int& GetHoles();
	const int GetRandomColor();
private:
	int _columns;
	int _rows;
	int _numTypes;

	int _holes;
	
	
	//hold color info of each cell;
	vector<int> _colors;

	// the Mersenne Twister Engine for random number generation
	typedef std::mt19937 MersenneNumberGenerator;
	MersenneNumberGenerator _randomNumberGenerator;
	// get a uniform distribution for random number generation
	std::uniform_int_distribution<uint32_t> uint_dist;


	void ClearPossibleMatch();

	bool IsPossibleSequence(const int & cellX, const int & cellY,const int & tarX, const int & tarY );

	bool IsPartOfSequence(const int& cellX, const int& cellY);

	// a bit array that describes valid color sequences on the board
	std::vector<bool> _colorSequences;

	//possible moves on the board
	std::vector<MatchMove> _possibleMoves;
};

