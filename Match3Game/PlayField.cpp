#include "pch.h"
#include "PlayField.h"
#include<iostream>
using namespace std;

void PlayField::GeneratePlayField(const int & width, const int & height, const int &numTypes)
{
	//init number generator with device calls
	std::random_device device;
	_randomNumberGenerator.seed(device());
	_columns = width;
	_rows = height;
	_numTypes = numTypes;
	_holes = 0;
	//reserve memory for color sequences
	_colorSequences.reserve(width * height);
	//reserver memory for possible moves
	_possibleMoves.reserve(10);

	_colors.reserve(width * height);

	//init color sequences
	for (int i = 0; i < _columns * _rows; ++i)
	{
		_colorSequences.push_back(false);
		_colors.push_back(-1);
	}

	// Populate board, if there is no possible moves, shuffle it
	do
	{
		Populate();
		DetectPossibleMoves();
	} while (!ContainsPossibleMoves());

}

void PlayField::PrintField(string str)
{
	cout << "--PrintField-- " << str.c_str() <<endl;
	for (int y = 0; y < _rows; y++)
	{
		for (int x = 0; x < _columns; x++)
		{
			int searchPos = y * _columns + x;
			cout << _colors[searchPos] << " ";
		}
		cout << endl;
	}
}

const int PlayField::GetColor(const int & x, const int & y)
{
	int searchPos = y * _columns + x;
	return _colors[searchPos];
}

void PlayField::Populate()
{
	// traverse the board rows to find horizontal color sequences
	for (int y = 0; y < _rows; ++y)
	{
		for (int x = 0; x < _columns; ++x)
		{
			int position = y * _columns + x;

			// generate a random color for this field
			_colors[position] = GetRandomColor();

			// check if a horizontal sequence has been created and remove it
			if (x > 1)
			{
				if (_colors[position] == _colors[y * _columns + x - 1] && _colors[y * _columns + x - 2])
				{
					uint8_t newColor = _colors[position];

					// get a new random color that is different and does not create a sequence
					while (newColor == _colors[position])
					{
						newColor = GetRandomColor();
					}

					_colors[position] = newColor;
				}
			}

			if (y > 1)
			{
				if (_colors[position] == _colors[(y - 1) * _columns + x] && _colors[(y - 2) * _columns + x])
				{
					uint8_t newColor = _colors[position];

					// get a new random color that is different and does not create a sequence
					while (newColor == _colors[position] || newColor == _colors[(y - 1) * _columns + x - 1])
					{
						newColor = GetRandomColor();
					}

					_colors[position] = newColor;
				}
			}
		}
	}
	//PrintField("Populate");
	_holes = 0;
}

void PlayField::DetectColorsSequences()
{
	// traverse the board rows to find horizontal color sequences
	for (int y = 0; y < _rows; ++y)
	{
		for (int x = 0; x < _columns; ++x)
		{
			int position = y * _columns + x;
			int color = _colors[position];

			// look in this row for a sequence

			int sequenceCountX = 1;
			int sequenceCountY = 1;

			// look horizontally - there is no need to look up the last two columns
			if (x < _columns - 2)
			{
				int matchingColor = color;

				while (true)
				{
					int searchPosition = position + sequenceCountX;

					// make sure to look for a sequence in the same row
					if (position / _rows != searchPosition / _rows)
						break;

					// get color of search position
					if (searchPosition < _columns * _rows)
						matchingColor = _colors[searchPosition];

					// stop looking if the colors don't match
					if (matchingColor != color)
						break;
					else
						++sequenceCountX;
				}
			}

			// look vertically - there is no need to look up the last two rows
			if (y < _rows - 2)
			{
				int matchingColor = color;

				while (true)
				{
					int searchPosition = position + sequenceCountY * _columns;

					// get color of search position
					if (searchPosition < _columns * _rows)
						matchingColor = _colors[searchPosition];
					else
						break;
					// stop looking if the colors don't match
					if (matchingColor != color)
						break;
					else
						++sequenceCountY;
				}
			}

			// check if a sequence of at least 3 horizontal matching color has been found
			if (sequenceCountX >= 3)
			{
				for (int sequence = 0; sequence < sequenceCountX; ++sequence)
				{
					int sequencePosition = position + sequence;
					_colorSequences.at(sequencePosition) = true;

					// call handler
					SequenceFound(x + sequence, y);
				}
			}
			// check if a sequence of at least 3 vertical matching color has been found
			if (sequenceCountY >= 3)
			{
				for (int sequence = 0; sequence < sequenceCountY; ++sequence)
				{
					int sequencePosition = position + sequence * _columns;
					_colorSequences.at(sequencePosition) = true;

					// call handler
					SequenceFound(x, y + sequence);
				}
			}
		}
	}
}
void PlayField::SequenceFound(int x, int y)
{
	//Auto Match the sequences
}
void PlayField::DetectPossibleMoves()
{
	// traverse the board rows to find horizontal color sequences
	for (int y = 0; y < _rows; ++y)
	{
		for (int x = 0; x < _columns; ++x)
		{
			int currentIndex = y * _columns + x;
			auto color = _colors[currentIndex];
			//   3
			// 1 x 2
			//   3
			if (x < _columns - 2 && _colors[currentIndex + 2] == color) {
				if (y > 0 && color == _colors[currentIndex + 1 - _columns])
				{
					AddMoves(x + 1, y, x + 1, y - 1);
				}
				if (y < _rows - 1 && color == _colors[currentIndex + 1 + _columns])
				{
					AddMoves(x + 1, y, x + 1, y + 1);
				}
			}
			//   1
			// 3 x 3
			//   2
			if (y < _rows - 2 && _colors[currentIndex + 2 * _columns] == color) {
				if (x > 0 && color == _colors[currentIndex - 1 + _columns])
				{
					AddMoves(x, y + 1, x - 1, y + 1);
				}
				if (x < _columns - 1 && color == _colors[currentIndex + 1 + _columns])
				{
					AddMoves(x, y + 1, x + 1, y + 1);
				}


			}
			//     3
			// 1 2 x 3
			//     3
			if (x < _columns - 2 && _colors[currentIndex + 1] == color) {
				if (y > 0 && color == _colors[currentIndex + 2 - _columns])
				{
					AddMoves(x + 2, y, x + 2, y - 1);
				}
				if (y < _rows - 1 && color == _colors[currentIndex + 2 + _columns])
				{
					AddMoves(x + 2, y, x + 2, y + 1);
				}
				if (x < _columns - 3 && color == _colors[currentIndex + 3])
				{
					AddMoves(x + 2, y, x + 3, y);
				}
			}
			//   3
			// 3 x 2 1
			//   3
			if (x > 1 && _colors[currentIndex - 1] == color) {
				if (y > 0 && color == _colors[currentIndex - 2 - _columns])
				{
					AddMoves(x - 2, y, x - 2, y - 1);
				}
				if (y < _rows - 1 && color == _colors[currentIndex - 2 + _columns])
				{
					AddMoves(x - 2, y, x - 2, y + 1);
				}
				if (x > 2 && color == _colors[currentIndex - 3])
				{
					AddMoves(x - 2, y, x - 3, y);
				}
			}
			//   1
			//   2
			// 3 x 3
			//   3
			if (y < _rows - 2 && _colors[currentIndex + _columns] == color) {
				if (x > 0 && color == _colors[currentIndex - 1 + 2 * _columns])
				{
					AddMoves(x, y + 2, x - 1, y + 2);
				}
				if (x < _columns - 1 && color == _colors[currentIndex + 1 + 2 * _columns])
				{
					AddMoves(x, y + 2, x + 1, y + 2);
				}
				if (y < _rows - 3 && color == _colors[currentIndex + 3 * _columns])
				{
					AddMoves(x, y + 2, x, y + 3);
				}
			}
			//   3
			// 3 x 3
			//   2
			//   1
			if (y > 1 && _colors[currentIndex - _columns] == color) {
				if (x > 0 && color == _colors[currentIndex - 1 - 2 * _columns])
				{
					AddMoves(x, y - 2, x - 1, y - 2);
				}
				if (x < _columns - 1 && color == _colors[currentIndex + 1 - 2 * _columns])
				{
					AddMoves(x, y - 2, x + 1, y - 2);
				}
				if (y > 2 && color == _colors[currentIndex - 3 * _columns])
				{
					AddMoves(x, y - 2, x, y - 3);
				}
			}


		}
	}
}

void  PlayField::AddMoves(int fromx, int fromy, int tarX, int tarY)
{

	MatchMove m;
	m.x = fromx;
	m.y = fromy;
	m.targetX = tarX;
	m.targetY = tarY;
	_possibleMoves.push_back(m);

}

bool PlayField::IsMatched(Cell cell, Cell originCell)
{
	return IsMatched(cell.x, cell.y, originCell.x, originCell.y);
}
//put a color to position x y, is it be matched
bool PlayField::IsMatched(int x, int y, int originX, int originY)
{
	int position = y * _columns + x;
	int color = GetColor(originX, originY);
	//look up
	int upColorSeqences = 1;
	for (int i = 1; i < 3 && y - i >= 0; i++)
	{

		int searchPos = (y - i) * _columns + x;
		int matchingColor = _colors[searchPos];
		if (matchingColor != color)
		{
			break;
		}
		upColorSeqences++;
	}

	if (upColorSeqences >= 3)
	{
		return true;
	}

	//look right
	int rightColorSeqences = 1;
	for (int i = 1; i < 3 && x + i < _columns; i++)
	{
		int searchPos = y * _columns + (x + i);
		int matchingColor = _colors[searchPos];
		if (matchingColor != color)
		{
			break;
		}
		rightColorSeqences++;
	}
	if (rightColorSeqences >= 3)
	{
		return true;
	}
	//look down
	int downColorSeqences = 1;
	for (int i = 1; i < 3 && y + i < _columns; i++)
	{
		int searchPos = (y + i) * _columns + x;
		int matchingColor = _colors[searchPos];
		if (matchingColor != color)
		{
			break;
		}
		downColorSeqences++;
	}
	if (downColorSeqences >= 3)
	{
		return true;
	}

	//look left
	int leftColorSeqences = 1;
	for (int i = 1; i < 3 && x - i >= 0; i++)
	{
		int searchPos = y * _columns + x - i;
		int matchingColor = _colors[searchPos];
		if (matchingColor != color)
		{
			break;
		}
		leftColorSeqences++;
	}
	if (leftColorSeqences >= 3)
	{
		return true;
	}

	return false;
}

bool PlayField::ContainsPossibleMoves()
{
	return _possibleMoves.size() != 0;
}

const bool PlayField::ContainsSequences()
{
	// iterate through color sequences
	for (std::vector<bool>::iterator it = _colorSequences.begin(); it != _colorSequences.end(); ++it)
	{
		// found the beginning of a color sequence
		if (*it == true)
			return true;
	}

	return false;
}

bool PlayField::Step(Cell sourceCell, Cell targetCell)
{
	return Step(sourceCell.x, sourceCell.y, targetCell.x, targetCell.y);
}
bool PlayField::Step(int sourceX, int sourceY, int targetX, int targetY)
{
	cout << "Move x" << sourceX << " y" << sourceY << " to x" << targetX << " y" << targetY << endl;
	// check if source color is valid
	if (sourceX < 0 || sourceX > _columns - 1 || sourceY < 0 || sourceY > _rows - 1)
		return false;

	// check if target color is valid
	if (targetX < 0 || targetX > _columns - 1 || targetY < 0 || targetY > _rows - 1)
		return false;

	int sourcePosition = sourceY * _columns + sourceX;
	int targetPosition = targetY * _columns + targetX;

	// attempt to swap colors - check if the swap leads to a sequence
	uint8_t sourceColor = _colors[sourceY * _columns + sourceX];
	uint8_t targetColor = _colors[targetY * _columns + targetX];
	// swap color
	_colors[sourcePosition] = targetColor;
	_colors[targetPosition] = sourceColor;


	DetectColorsSequences();
	if (ContainsSequences())
	{
		RemoveSequences();
		//create new Color
		while (DropColors()) {}
		ClearPossibleMatch();
		DetectPossibleMoves();
		PrintField("After Drop Color");
		FindAndRemoveSequences();
		return true;
	}

	else
	{
		// reverse swap
		_colors[sourcePosition] = sourceColor;
		_colors[targetPosition] = targetColor;

		return false;
	}

	//---

}

void PlayField::FindAndRemoveSequences()
{
	while (ContainsSequences())
	{
		RemoveSequences();
		while (DropColors())
		{

		}
	}
}


void PlayField::RemoveSequences()
{
	for (int i = 0; i < _rows * _columns; ++i)
	{
		if (_colorSequences.at(i) == true)
		{
			_colors[i] = -1;

			// invoke removal event
		//	ColorRemoved(i % _columns, i / _rows);

			//track how many holes there are after removal
			++_holes;
		}
	}

	//reset valid color sequences
	for (std::vector<bool>::iterator it = _colorSequences.begin(); it != _colorSequences.end(); ++it)
	{
		*it = false;
	}
	PrintField("remove seq");
}

bool PlayField::DropColors()
{
	bool hasHoles = false;
	// iterate through the board from behind and look for holes
	for (int y = _rows - 1; y >= 0; --y)
	{
		for (int x = _columns - 1; x >= 0; --x)
		{
			int position = y * _columns + x;

			// check if there is a hole here
			if (_colors[position] == -1)
			{
				// get position of color above
				int swapPosition = position - _columns;

				// if the position is is within the field, move the color down one cell
				if (swapPosition >= 0)
				{
					_colors[position] = _colors[swapPosition];
					_colors[swapPosition] = -1;

					// check if a hole has been closed
					//if (_colors[position] != -1)
					//{
					//	// invoke color dropped event
					//	ColorDropped(x, y - 1);
					//}
				}
				else
					// the hole is at the top row and it gets closed with a new random color
				{
					_colors[position] = GetRandomColor();

					// invoke color added event
					//ColorAdded(x, y);

					// one hole has bubbled up all the way to the top and has been removed
					--_holes;
					hasHoles = true;
				}

			}
		}
	}
	return hasHoles;
}

const vector<MatchMove>& PlayField::GetPossibleMoves()const
{
	return _possibleMoves;
}
const int & PlayField::GetColumns()
{
	return _columns;
}
const int & PlayField::GetRows()
{
	return _rows;
}
const int & PlayField::GetHoles()
{
	return _holes;
}
void PlayField::SwapColors(const Cell & fromCell, const Cell & toCell)
{
	SwapColors(fromCell.x, fromCell.y, toCell.x, toCell.y);
}
void PlayField::SwapColors(const int & sourceX, const int & sourceY, const int & targetX, const int & targetY)
{
	// check if source color is valid
	if (sourceX < 0 || sourceX > _columns - 1 || sourceY < 0 || sourceY > _rows - 1)
		return;

	// check if target color is valid
	if (targetX < 0 || targetX > _columns - 1 || targetY < 0 || targetY > _rows - 1)
		return;

	int sourcePosition = sourceY * _columns + sourceX;
	int targetPosition = targetY * _columns + targetX;

	// attempt to swap colors - check if the swap leads to a sequence
	uint8_t sourceColor = _colors[sourceY * _columns + sourceX];
	uint8_t targetColor = _colors[targetY * _columns + targetX];

	// swap color
	_colors[sourcePosition] = targetColor;
	_colors[targetPosition] = sourceColor;
	cout << "----Swap Color----" << endl;
	PrintField("after swap color");
}


const int PlayField::GetRandomColor()
{
	int randomColor = uint_dist(_randomNumberGenerator) % _numTypes;
	return randomColor;
}



void PlayField::ClearPossibleMatch()
{
	_possibleMoves.clear();
}
bool PlayField::IsPossibleSequence(const int & x, const int & y, const int & tarX, const int & tarY)
{

	auto color = _colors[y*_columns + x];
	//   3
	// 1 x 2
	//   3
	if (x < _columns - 2 && _colors[x + 2] == color) {
		if (y > 0 && color == _colors[x + 1 - _columns]) return true;
		if (y < _rows - 1 && color == _colors[x + 1 + _columns]) return true;
	}
	//   1
	// 3 x 3
	//   2
	if (y < _rows - 2 && _colors[x + 2 * _columns] == color) {
		if (x > 0 && color == _colors[x - 1 + _columns]) return true;
		if (x < _columns - 1 && color == _colors[x + 1 + _columns]) return true;
	}
	//     3
	// 1 2 x 3
	//     3
	if (x < _columns - 2 && _colors[x + 1] == color) {
		if (y > 0 && color == _colors[x + 2 - _columns]) return true;
		if (y < _rows - 1 && color == _colors[x + 2 + _columns]) return true;
		if (x < _columns - 3 && color == _colors[x + 3]) return true;
	}
	//   3
	// 3 x 2 1
	//   3
	if (x > 1 && _colors[x - 1] == color) {
		if (y > 0 && color == _colors[x - 2 - _columns]) return true;
		if (y < _rows - 1 && color == _colors[x - 2 + _columns]) return true;
		if (x > 2 && color == _colors[x - 3]) return true;
	}
	//   1
	//   2
	// 3 x 3
	//   3
	if (y < _rows - 2 && _colors[x + _columns] == color) {
		if (x > 0 && color == _colors[x - 1 + 2 * _columns]) return true;
		if (x < _columns - 1 && color == _colors[x + 1 + 2 * _columns]) return true;
		if (y < _rows - 3 && color == _colors[x + 3 * _columns]) return true;
	}
	//   3
	// 3 x 3
	//   2
	//   1
	if (y > 1 && _colors[x - _columns] == color) {
		if (x > 0 && color == _colors[x - 1 - 2 * _columns]) return true;
		if (x < _columns - 1 && color == _colors[x + 1 - 2 * _columns]) return true;
		if (y > 2 && color == _colors[x - 3 * _columns]) return true;
	}


	return false;

}

bool PlayField::IsPartOfSequence(const int & cellX, const int & cellY)
{
	// look in all directions and check 

	uint8_t sourceColor = _colors[cellY * _columns + cellX];
	// look right
	if (cellX < (_columns - 2))
	{
		if (sourceColor == _colors[cellY * _columns + cellX + 1] && sourceColor == _colors[cellY * _columns + cellX + 2])
		{
			return true;
		}
	}

	// look left
	if (cellX > 1)
	{
		if (sourceColor == _colors[cellY * _columns + cellX - 1] && sourceColor == _colors[cellY * _columns + cellX - 2])
		{
			return true;
		}
	}

	// look left and right 1 tile
	if (cellX > 0 && cellX < (_columns - 1))
	{
		if (sourceColor == _colors[cellY * _columns + cellX - 1] && sourceColor == _colors[cellY * _columns + cellX + 1])
		{
			return true;
		}
	}

	// look down
	if (cellY < (_rows - 2))
	{
		if (sourceColor == _colors[(cellY + 1) * _columns + cellX] && sourceColor == _colors[(cellY + 2) * _columns + cellX])
		{
			return true;
		}
	}

	// look up
	if (cellY > 1)
	{
		if (sourceColor == _colors[(cellY - 1) * _columns + cellX] && sourceColor == _colors[(cellY - 2) * _columns + cellX])
		{
			return true;
		}
	}

	// look up and down 1 tile
	if (cellY > 0 && cellY < (_rows - 1))
	{
		if (sourceColor == _colors[(cellY - 1) * _columns + cellX] && sourceColor == _colors[(cellY + 1) * _columns + cellX])
		{
			return true;
		}
	}

	return false;


}
