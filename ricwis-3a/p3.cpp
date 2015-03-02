// Declarations and functions for project #4
#include "stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
// Stores the entire Sudoku board
{
   public:
      board(int);
      void clear();
      void initialize(ifstream &fin);
      void print();
      bool isBlank(int, int);
      ValueType getCell(int, int);
	  void setCell(int x,int y,int i);
	  void printConflicts();
	  bool isSolved();
	  void clearCell(int i, int j);
      
   private:

      // The following matrices go from 1 to BoardSize in each
      // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)

      matrix<ValueType> value;

	  void initConflicts();
	  void updateConflicts(int i, int j);//should probs pass the i and j values of the new val to speed up

	  vector<bool> rowConflicts;
	  void updateRowConflicts(int row);
	  vector<bool> columnConflicts;
	  void updateColumnConflicts(int column);
	  vector<bool> squareConflicts;
	  void updateSquareConflicts(int square);
};

board::board(int sqSize)
   : value(BoardSize+1, BoardSize+1)
// Board constructor
{
}

void board::clear()
{
	for (int i = 1; i <= BoardSize; i++)
	{
		for each (ValueType v in value[i])
		{
			v = Blank;
		}
	}

	for each (bool b in rowConflicts)
	{
		b = false;
	}

	for each (bool b in columnConflicts)
	{
		b = false;
	}

	for each (bool b in squareConflicts)
	{
		b = false;
	}
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
   char ch;

   clear();
   rowConflicts.resize(BoardSize);
   columnConflicts.resize(BoardSize);
   squareConflicts.resize(BoardSize);
   for (int i = 1; i <= BoardSize; i++)
   {
	   for (int j = 1; j <= BoardSize; j++)
	   {
		   fin >> ch;

		   // If the read char is not Blank
		   if (ch != '.')
		   {
			   setCell(i, j, ch - '0');   // Convert char to int
		   }
	   }
   }
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

void board::printConflicts()
{
	cout << "\nRow conflicts : ";
	for each (bool b in columnConflicts)
	{
		if (b == true){
			cout << " T";
		}
		else{
			cout << " F";
		}
	}

	cout << "\nColumn conflicts : ";
	for each (bool b in columnConflicts)
	{
		if (b == true){
			cout << " T";
		}
		else{
			cout << " F";
		}
	}

	cout << "\nSquare conflicts : ";
	for each (bool b in squareConflicts)
	{
		if (b == true){
			cout << " T";
		}
		else{
			cout << " F";
		}
	}
}

void conflictCheck(vector<ValueType> valueVect, vector<bool> &conflictVect)
{
	for (int x = 1; x <= 9; x++)
	{
		int count = 0;
		for each (ValueType val in valueVect)//-1 to zero index it
		{
			if (val == x)
			{
				count++;
			}
		}
		if (count > 1)
		{
			conflictVect[x - 1] = true;
		}
	}
}

void board::updateRowConflicts(int row)
{
	conflictCheck(value[row], rowConflicts);
}

void board::updateColumnConflicts(int column)
{
	//build column vector
	vector<ValueType> columnVec;
	for (int i = 1; i <= BoardSize; i++)
	{
		columnVec.push_back(value[i][column]);
	}

	conflictCheck(columnVec, columnConflicts);
}

void board::updateSquareConflicts(int square)
{
	vector<ValueType> squareVec;
	for(int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			if (squareNumber(i, j) == square)
			{
				squareVec.push_back(value[i][j]);
			}
		}
	}

	conflictCheck(squareVec, squareConflicts);
}


void board::updateConflicts(int i, int j)
{
	updateColumnConflicts(j);
	updateRowConflicts(i);
	updateSquareConflicts(squareNumber(i, j));
}

void board::initConflicts()
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			updateConflicts(i,j);
		}
	}
}




ostream &operator<<(ostream &ostr, vector<ValueType> &v)
// Overloaded output operator for vector class.
{
	for (int i = 0; i < v.size(); i++)
	{
		ostr << v[i] << " ";
	}
   ostr << endl;
   return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
	{
		return value[i][j];
	}
	else
	{
		throw rangeError("bad value in getCell");
	}
}

void board::setCell(int i, int j,int val)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize && val >= 1 && val <= 9)
	{
		value[i][j] = val;
		updateConflicts(i,j);
	}
	else
	{
		throw rangeError("bad value in setCell");
	}
}

void board::clearCell(int i, int j)
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
	{
		value[i][j] = 0;
		updateConflicts(i, j);
	}
	else
	{
		throw rangeError("bad value in setCell");
	}
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{
		throw rangeError("bad value in setCell");
	}
	else
	{
		return (value[i][j] == 0);
	}
}

bool board::isSolved()
{
	for each (bool b in rowConflicts)
	{
		if (b == true)
			return false;
	}

	for each (bool b in columnConflicts)
	{
		if (b == true)
			return false;
	}

	for each (bool b in squareConflicts)
	{
		if (b == true)
			return false;
	}

	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j < BoardSize; j++)
		{
			if (isBlank(i, j))
				return false;
		}
	}

	return true;
}

void board::print()
// Prints the current board.
{
   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
         cout << " -";
		 for (int j = 1; j <= BoardSize; j++)
			cout << "---";
			 cout << "-";
		 cout << endl;
      }
      for (int j = 1; j <= BoardSize; j++)
      {
		  if ((j - 1) % SquareSize == 0)
		  {
			  cout << "|";
		  }
		  if (!isBlank(i, j))
		  {
			  cout << " " << getCell(i, j) << " ";
		  }
		  else
		  {
			  cout << "   ";
		  }
      }
      cout << "|";
      cout << endl;
   }

   cout << " -";
   for (int j = 1; j <= BoardSize; j++)
   {
	   cout << "---";
   }
   cout << "-";
   cout << endl;
}

int main()
{
   ifstream fin;
   
   // Read the sample grid from the file.
   string fileName;
   cout << "Enter path to puzzle : ";
   cin >> fileName;

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      board b1(SquareSize);

      while (fin && fin.peek() != 'Z')
      {
			 b1.initialize(fin);
			 b1.print();
			 b1.printConflicts();
			 if (b1.isSolved()){
				 cout << "\nBoard is solved!\n";
			 }
			 else{
				 cout << "\nBoard is not solved :(\n";
			 }
      }
   }
   catch  (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
}

