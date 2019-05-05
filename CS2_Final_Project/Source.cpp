#include <iostream>
#include <windows.h>   // WinApi header
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime> // Testing for AI, TODO delete later

using namespace std;   

// TODO add coin flip for who goes first

void PrintBoard(const string board[][7], HANDLE& hConsole);

void StartGame(string  board[6][7], bool &player_won, HANDLE &hConsole);

void DeclareWinner(const HANDLE &hConsole, bool player_won);

bool EndGame(const string board[][7], bool& player_win);

void BOT(string board[][7]);

void PutChip(string board[][7], string chip, bool player = true);

void BOTAI(string board[][7], int &column_pick);

bool isColumnFull(const string board[][7], int column_pick);

const int GREEN = 10; 
const int YELLOW = 14;
const int RED = 12;

// TODO Replace all instances of [O] & [0] with there respective constants

const string PLAYERCHIP = "[O]";
const string BOTCHIP = "[X]";

int main()
{
	srand((unsigned)time(NULL)); // Delete later after BOTAI functionality is added

	HANDLE hConsole;
	
	//int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// you can loop k higher to see more color choices
	// pick the colorattribute k you want

	// TODO change board size to const
	string board[6][7] =
	{
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
		{"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
	};

	bool player_won;

	// While it is not end of game, continue looping.
	// Note: EndGame returns true if it counts 4 chips in a row and !EndGame will change it to false, stopping the loop
	StartGame(board, player_won, hConsole);

	DeclareWinner(hConsole, player_won);

	SetConsoleTextAttribute(hConsole, YELLOW);
	PrintBoard(board, hConsole);

	return 0;
}

void DeclareWinner(const HANDLE &hConsole, bool player_won)
{
	SetConsoleTextAttribute(hConsole, GREEN);
	if (player_won)
		cout << setw(17) << setfill(' ') << "You won!\n\n";
	else
		cout << setw(17) << setfill(' ') << "You lost!\n\n";
}

void StartGame(string  board[6][7], bool &player_won, HANDLE &hConsole)
{
	while (!EndGame(board, player_won)) // TODO create bool function that defines when the game is over. Ex. Once 4 chips are connected
	{
		SetConsoleTextAttribute(hConsole, YELLOW);
		PrintBoard(board, hConsole);

		SetConsoleTextAttribute(hConsole, GREEN);
		PutChip(board, PLAYERCHIP);

		SetConsoleTextAttribute(hConsole, YELLOW);
		PrintBoard(board, hConsole);

		// Added bc if player connects 4 before the bot and then the bot connects 4, Player will lose even though they won first
		if (EndGame(board, player_won))
			break;

		// Bot that will play against player
		SetConsoleTextAttribute(hConsole, GREEN);
		BOT(board);

	}
}

void PrintBoard(const string board[][7], HANDLE& hConsole)
{
	SetConsoleTextAttribute(hConsole, RED);
	cout << " 1 " << " 2 " << " 3 " << " 4 " << " 5 " << " 6 " << " 7 \n";

	SetConsoleTextAttribute(hConsole, YELLOW);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (j == 6)
				cout << board[i][j] << endl;
			else
				cout << board[i][j];
		}
	}
	cout << endl;
}

// TODO May need another character for the chip for the bot. 

void PutChip(string board[][7], string chip, bool isPlayer) // May be reused for the bot
{
	int column_pick = rand() % 7 + 1;

	if (isPlayer)
	{
		cout << "Which column would you like? (1-7): ";
		cin >> column_pick;
		while (column_pick > 7 || column_pick < 1)
		{
			cout << "\nColumn does not exist!\n\n";
			cout << "Which column would you like? (1-7): ";
			cin >> column_pick;
		}
		while (isColumnFull(board, column_pick))
		{
			cout << "\nColumn is full!\n\n";
			cout << "Which column would you like? (1-7): ";
			cin >> column_pick;
		}
	}
	else
	{
		// Want to loop until the column picked is not full
		BOTAI(board, column_pick);
	}

	cout << endl;

	column_pick--; // index starts at 0

	for (int i = 5; i >= 0; i--)
	{
		if (board[i][column_pick] == "[ ]")
		{
			board[i][column_pick] = chip;
			break;
		}
	}

}

void BOTAI(string board[][7], int &column_pick)
{
	while (isColumnFull(board, column_pick))
	{
		column_pick = rand() % 7 + 1;
	}
	cout << "Bot chooses column " << column_pick << endl;
}

void BOT(string board[][7])
{
	PutChip(board, BOTCHIP, false);
}

// TODO return by referrence if the player wins or bot wins
// HACK BUG: registers win even if its not 4 in a row
// TODO imporve the loop as running multiple loops is not efficient
bool EndGame(const string board[][7], bool& player_win)
{
	int row_Ochipcount = 0;
	int row_0chipcount = 0;

	// Counts Horizontal Chips for each player
	// HACK make sure they are next to each other, not just there are 4 in the row/column

	// Why does this work for decremeting, but when incrementing it does not count the others [ ][ ][ ][O][O][O][O]

	for (int row = 5; row >= 0; row--)
	{
		for (int col = 6; col >= 0; col--)
		{
			if (board[row][col] == PLAYERCHIP)
				row_Ochipcount++;

			else if (board[row][col] == BOTCHIP || board[row][col] == "[ ]")
			{
				if (row_Ochipcount >= 4)
				{
					player_win = true;
					return true; // Someone has won, return true
				}
				else 
					row_Ochipcount = 0;
			}

			if (board[row][col] == BOTCHIP)
				row_0chipcount++;

			else if (board[row][col] == PLAYERCHIP || board[row][col] == "[ ]")
			{
				if (row_0chipcount >= 4)
				{
					player_win = false;
					return true; // Someone has won, return true
				}
				else
					row_0chipcount = 0;
			}
		}
	}

	// Counts Vertical Chips for each player

	for (int col = 6; col >= 0; col--)
	{
		for (int row = 7; row >= 0; row--)
		{
			if (board[row][col] == PLAYERCHIP)
				row_Ochipcount++;

			else if (board[row][col] == BOTCHIP || board[row][col] == "[ ]")
			{
				if (row_Ochipcount >= 4)
				{
					player_win = true;
					return true; // Someone has won, return true
				}
				else
					row_Ochipcount = 0;
			}

			if (board[row][col] == BOTCHIP)
				row_0chipcount++;

			else if (board[row][col] == PLAYERCHIP || board[row][col] == "[ ]")
			{
				if (row_0chipcount >= 4)
				{
					player_win = false;
					return true; // Someone has won, return true
				}
				else
					row_0chipcount = 0;
			}
		}
	}

	// Counts Diagonal Chips for each player
	// From left side
	for (int row = 5; row >= 0; row--)
	{
		for (int col = 6; col >= 0; col--)
		{
			if (row > 2 && col >= 3)
			{
				if (board[row][col] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 1][col - 1] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 2][col - 2] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 3][col - 3] == PLAYERCHIP)
					row_Ochipcount++;
				if (row_Ochipcount >= 4)
				{
					player_win = true;
					return true; // Someone has won, return true
				}
				else
				{
					row_Ochipcount = 0;
				}

				if (board[row][col] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 1][col - 1] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 2][col - 2] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 3][col - 3] == BOTCHIP)
					row_0chipcount++;
				if (row_0chipcount >= 4)
				{
					player_win = false;
					return true; // Someone has won, return true
				}
				else
				{
					row_0chipcount = 0;
				}
			}
		}
	}

	// From Right side
	for (int row = 5; row >= 0; row--)
	{
		for (int col = 0; col < 7; col++)
		{
			if (row > 2 && col <= 3)
			{
				if (board[row][col] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 1][col + 1] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 2][col + 2] == PLAYERCHIP)
					row_Ochipcount++;
				if (board[row - 3][col + 3] == PLAYERCHIP)
					row_Ochipcount++;
				if (row_Ochipcount >= 4)
				{
					player_win = true;
					return true; // Someone has won, return true
				}
				else
				{
					row_Ochipcount = 0;
				}

				if (board[row][col] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 1][col + 1] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 2][col + 2] == BOTCHIP)
					row_0chipcount++;
				if (board[row - 3][col + 3] == BOTCHIP)
					row_0chipcount++;
				if (row_0chipcount >= 4)
				{
					player_win = false;
					return true; // Someone has won, return true
				}
				else
				{
					row_0chipcount = 0;
				}
			}
		}
	}

	// Default
	return false;
}

bool isColumnFull(const string board[][7], int column_pick)
{
	int count = 0;
	
	column_pick--;

	for (int i = 0; i < 6; i++)
	{
		if (board[i][column_pick] != "[ ]")
		{
			count++; 
		}
	}

	if (count >= 6) // if the count is 6(row) is greater it should return true and ask for another pick
		return true;
	else
		return false;
}

