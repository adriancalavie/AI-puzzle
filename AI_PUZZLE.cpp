#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cmath>
#include <Windows.h>
#define EMPTY 0

using namespace std;



struct GameState
{
	vector<vector<int>> matrix;

	int size, cost_R, costSoFar = 0;
	GameState* parent;
	void allocate()
	{
		matrix.resize(size);
		for (int i = 0; i < size; i++)
			matrix.at(i).resize(size);
	}

	void copyS(GameState* pGame)
	{
		this->matrix = pGame->matrix;
		this->size = pGame->size;
		this->parent = pGame->parent;
		this->costSoFar = pGame->costSoFar;
		this->cost_R = pGame->cost_R;
	}
};
struct comparator
{
	bool operator () (GameState* a, GameState* b)
	{
		return (*a).cost_R > (*b).cost_R;
	}
};
int manhattan(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}
pair<int, int> getFinalPos(vector<vector<int>> matrix, int element)
{
	for (int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix.size(); j++)
			if (element == matrix.at(i).at(j))
				return make_pair(i, j);
}
int manhattanSum(vector<vector<int>> current, vector<vector<int>> final)
{
	int sum = 0;
	for (int i = 0; i < current.size(); i++)
		for (int j = 0; j < current.size(); j++)
		{
			pair<int, int> finalPos = getFinalPos(final, current.at(i).at(j));
			sum += manhattan(i, j, finalPos.first, finalPos.second);
		}
	return sum;
}

void displayState(GameState* myGame)
{
	std::cout << endl;
	for (int i = 0; i < (*myGame).size; i++)
	{
		std::cout << '\t' << '|';
		for (int j = 0; j < (*myGame).size; j++)
			if ((*myGame).matrix.at(i).at(j) == EMPTY)
				std::cout << (char)254 << '\t';
			else
				std::cout << (*myGame).matrix.at(i).at(j) << '\t';
		std::cout << '|' << endl;
	}
}
pair<int, int> get_Pos(GameState* myGame)
{
	for (int i = 0; i < (*myGame).size; i++)
	{
		for (int j = 0; j < (*myGame).size; j++)
			if ((*myGame).matrix.at(i).at(j) == EMPTY)
			{
				pair<int, int> par = make_pair(i, j);
				return par;
			}
	}
}
GameState* goUp(GameState* initGame, GameState* final)
{
	GameState* myGame = new GameState();
	myGame->copyS(initGame);
	myGame->parent = initGame;

	pair<int, int> empty_positions = get_Pos(myGame);

	if (empty_positions.first == 0)
	{
		std::cout << "\nOut of bound move\n";
		return myGame;
	}

	swap(
		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second),

		(*myGame).matrix.at(empty_positions.first - 1).at(empty_positions.second)
	);

	myGame->costSoFar = initGame->costSoFar + 1;
	myGame->cost_R = myGame->costSoFar + manhattanSum((*myGame).matrix, (*final).matrix);
	return myGame;

}
GameState* goDown(GameState* initGame, GameState* final)
{
	GameState* myGame = new GameState();
	myGame->copyS(initGame);
	myGame->parent = initGame;

	pair<int, int> empty_positions = get_Pos(myGame);

	if (empty_positions.first == (*myGame).size - 1)
	{
		std::cout << "\nOut of bound move\n";
		return myGame;
	}

	swap(
		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second),

		(*myGame).matrix.at(empty_positions.first + 1).at(empty_positions.second)
	);

	myGame->costSoFar = initGame->costSoFar + 1;
	myGame->cost_R = myGame->costSoFar + manhattanSum((*myGame).matrix, (*final).matrix);
	return myGame;

}
GameState* goLeft(GameState* initGame, GameState* final)
{
	GameState* myGame = new GameState();
	myGame->copyS(initGame);
	myGame->parent = initGame;

	pair<int, int> empty_positions = get_Pos(myGame);

	if (empty_positions.second == 0)
	{
		std::cout << "\nOut of bound move\n";
		return myGame;
	}

	swap(
		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second),

		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second - 1)
	);

	myGame->costSoFar = initGame->costSoFar + 1;
	myGame->cost_R = myGame->costSoFar + manhattanSum((*myGame).matrix, (*final).matrix);

	return myGame;

}
GameState* goRight(GameState* initGame, GameState* final)
{
	GameState* myGame = new GameState();
	myGame->copyS(initGame);
	myGame->parent = initGame;

	pair<int, int> empty_positions = get_Pos(myGame);

	if (empty_positions.second == (*myGame).size - 1)
	{
		std::cout << "\nOut of bound move\n";
		return myGame;
	}

	swap(
		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second),

		(*myGame).matrix.at(empty_positions.first).at(empty_positions.second + 1)
	);

	myGame->costSoFar = initGame->costSoFar + 1;
	myGame->cost_R = myGame->costSoFar + manhattanSum((*myGame).matrix, (*final).matrix);

	return myGame;
}
bool gameResolvable(GameState* myGame)
{
	vector<int> tileRow;

	for (int i = 0; i < (*myGame).size; i++)
		for (int j = 0; j < (*myGame).size; j++)
			tileRow.push_back((*myGame).matrix.at(i).at(j));

	int inversionCounter = 0;

	for (int i = 0; i < tileRow.size() - 1; i++)
		for (int j = i + 1; j < tileRow.size(); j++)
		{
			if (tileRow.at(i) != EMPTY && tileRow.at(j) != EMPTY)
			{
				if (tileRow.at(i) > tileRow.at(j))
					inversionCounter++;
			}
		}

	pair<int, int> empty_positions = get_Pos(myGame);

	if ((*myGame).size % 2 != 0 && inversionCounter % 2 == 0)
		return true;

	if ((*myGame).size % 2 == 0)
	{
		if (empty_positions.first % 2 == 0 && inversionCounter % 2 != 0)
			return true;

		if (empty_positions.first % 2 != 0 && inversionCounter % 2 == 0)
			return true;
	}

	return false;
}
GameState* getFinal(int size)
{
	GameState* final = new GameState();

	(*final).size = size;
	(*final).allocate();

	int number = 1;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			if (!(i == size - 1 && j == size - 1))
			{
				(*final).matrix.at(i).at(j) = number++;

			}
			else
			{
				(*final).matrix.at(i).at(j) = EMPTY;
			}
		}
	return final;
}
bool areEqual(GameState* myGame1, GameState* myGame2)
{
	if ((*myGame1).size != (*myGame2).size)
		return false;

	for (int i = 0; i < (*myGame1).size; i++)
		for (int j = 0; j < (*myGame1).size; j++)
			if ((*myGame1).matrix.at(i).at(j) != (*myGame2).matrix.at(i).at(j))
				return false;
	return true;
}
bool duplicate(vector<GameState*> closed, GameState* myGame)
{
	for (int i = 0; i < closed.size(); i++)
		if (areEqual(myGame, closed.at(i))) return true;
	return false;
}
int checkEdge(int i, int j, int size)
{

	if (i == 0 && j == 0)
		return 1; //codificare colt stanga sus

	if (i == 0 && j == size - 1)
		return 2; //codificare colt dreapta sus

	if (i == size - 1 && j == 0)
		return 3; //codificare colt stanga jos

	if (i == size - 1 && j == size - 1)
		return 4;//codificare colt dreapta jos

	if (j == 0)
		return 5; //codificare margine stanga

	if (j == size - 1)
		return 6; //codificare margine dreapta

	if (i == 0)
		return 7; //codificare margine sus

	if (i == size - 1)
		return 8; //codificare margine jos

	return 0; //codificare centru
}
void enqueueStates(int action, priority_queue<GameState*, std::vector<GameState*>, comparator>& statesQ, GameState* myGame, GameState* final)
{
	switch (action)
	{
	case 0:
		statesQ.push(goLeft(myGame, final));
		statesQ.push(goUp(myGame, final));
		statesQ.push(goRight(myGame, final));
		statesQ.push(goDown(myGame, final));
		break; //centru - 4 miscari: stanga, sus, dreapta, jos 
	case 1:
		statesQ.push(goRight(myGame, final));
		statesQ.push(goDown(myGame, final));
		break; //colt stanga sus - 2 miscari: dreapta, jos 
	case 2:
		statesQ.push(goDown(myGame, final));
		statesQ.push(goLeft(myGame, final));
		break; //colt dreapta sus - 2 miscari: jos, stanga
	case 3:
		statesQ.push(goUp(myGame, final));
		statesQ.push(goRight(myGame, final));
		break; //colt stanga jos - 2 miscari: sus, dreapta
	case 4:
		statesQ.push(goLeft(myGame, final));
		statesQ.push(goUp(myGame, final));
		break; //colt dreapta jos - 2 miscari: stanga, sus
	case 5:
		statesQ.push(goUp(myGame, final));
		statesQ.push(goRight(myGame, final));
		statesQ.push(goDown(myGame, final));
		break; //margine stanga - 3 miscari: sus, dreapta, jos
	case 6:
		statesQ.push(goLeft(myGame, final));
		statesQ.push(goUp(myGame, final));
		statesQ.push(goDown(myGame, final));
		break; // margine dreapta - 3 miscari: stanga, sus, jos
	case 7:
		statesQ.push(goLeft(myGame, final));
		statesQ.push(goRight(myGame, final));
		statesQ.push(goDown(myGame, final));
		break; // margine sus - 3 miscari: stanga, dreapta, jos
	case 8:
		statesQ.push(goLeft(myGame, final));
		statesQ.push(goUp(myGame, final));
		statesQ.push(goRight(myGame, final));
		break; // margine jos - 3 miscari: stanga, sus, dreapta
	default: "\nnot good"; break;
	};
}
pair< vector<GameState*>, int> searchAI(GameState* initial, GameState* final)
{
	vector<GameState*> solution;
	if (areEqual(initial, final))
		return make_pair(solution, 0);

	priority_queue<GameState*, std::vector<GameState*>, comparator> fringeStates;

	vector<GameState*> closedStates;

	closedStates.push_back(initial);

	pair<int, int> empty_positions = get_Pos(initial);
	int firstAction = checkEdge(empty_positions.first, empty_positions.second, (*initial).size);
	enqueueStates(firstAction, fringeStates, initial, final);
	initial->cost_R = manhattanSum(initial->matrix, final->matrix);
	long long iteration = 0;
	while (!fringeStates.empty())
	{
		GameState* newGame = new GameState();
		newGame->copyS(fringeStates.top());
		fringeStates.pop();
		cout << endl << newGame->cost_R;

		if (!duplicate(closedStates, newGame))
		{
			std::cout << "\nIteration #" << iteration << ": \n";
			displayState(newGame);
			//Sleep(1000);
			//system("cls");
			iteration++;
			closedStates.push_back(newGame);

			if (areEqual(newGame, final))
			{
				int countor = 0;

				while (newGame->parent != nullptr)
				{
					solution.push_back(newGame);
					newGame = newGame->parent;
					countor++;
				}
				solution.push_back(initial);
				return make_pair(solution, countor);
			}

			empty_positions = get_Pos(newGame);
			int action = checkEdge(empty_positions.first, empty_positions.second, (*newGame).size);
			enqueueStates(action, fringeStates, newGame, final);
		}
	}

}
char movedTo(GameState* gameBefore, GameState* gameAfter)
{
	pair<int, int> position1 = get_Pos(gameBefore), position2 = get_Pos(gameAfter);

	if (position1.first == position2.first)
	{
		if (position1.second == position2.second - 1)
			return 'l';
		if (position1.second == position2.second + 1)
			return 'r';
	}
	if (position1.second == position2.second)
	{
		if (position1.first == position2.first + 1)
			return 'd';
		if (position1.first == position2.first - 1)
			return 'u';
	}
	return 'x';
}
void printSolution(pair < vector<GameState*>, int > solution, int code, string filename)
{
	ofstream out(filename);

	if (code)
	{
		out << "Game is solvable, solution is composed of " << solution.second << " moves:\n";
		for (int k = solution.first.size() - 1; k >= 0; k--)
		{
			if (k == solution.first.size() - 1)
				out << "\nInitial game set: ";
			out << endl;
			for (int i = 0; i < (*solution.first.at(k)).size; i++)
			{
				out << '\t' << '|';
				for (int j = 0; j < (*solution.first.at(k)).size; j++)
					if ((*solution.first.at(k)).matrix.at(i).at(j) == EMPTY)
						out << '#' << '\t';
					else
						out << (*solution.first.at(k)).matrix.at(i).at(j) << '\t';
				out << '|' << endl;
			}
			if (k > 0)
			{
				out << "\nMove " << solution.first.size() - k;
				switch (movedTo(solution.first.at(k - 1), solution.first.at(k)))
				{
				case 'u': out << "(up"; break;
				case'd':out << "(down"; break;
				case'r':out << "(right"; break;
				case'l':out << "(left"; break;
				default: std::cout << "\n\tmovedTo error\n"; break;
				};
				out << "):";
			}

		}
		out << "\n^ Finished ^";
	}
	else
		out << "Game is not solvable.";

	out.close();

}
GameState* readInitial(string filename)
{
	ifstream fin(filename);
	vector<int> myVector;
	int max;
	fin >> max;
	myVector.push_back(max);
	while (!fin.eof())
	{
		int number;
		fin >> number;
		myVector.push_back(number);
		if (number > max)
			max = number;
	}
	GameState* initial = new GameState();
	initial->size = sqrt(max + 1);
	initial->allocate();
	int k = 0;
	for (int i = 0; i < sqrt(max + 1); i++)
		for (int j = 0; j < sqrt(max + 1); j++)
		{
			initial->matrix.at(i).at(j) = myVector[k++];
		}
	return initial;
}
int main()
{

	GameState* initial = readInitial("Text.txt");
	pair < vector<GameState*>, int > solution;
	int code;
	if (gameResolvable(initial))
		code = 1;
	else
	{
		code = 0;
		printSolution(solution, code, "rezultat.txt");
	}
	if (code) {
		GameState* final = getFinal((*initial).size);
		solution = searchAI(initial, final);
		printSolution(solution, code, "rezultat.txt");
		delete final;
	}

	delete initial;

	return 0;
}
