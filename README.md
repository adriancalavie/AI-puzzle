# AI-puzzle
This C++ code from 2019 solves the puzzle problem. I wrote it in my first year at University. 

## Description
---
This algorithm uses states(```GameState```) to verify if the current current puzzle is in final form or not. It firstly checks if the game is resolvable or not based on the information on this [site](http://kevingong.com/Math/SixteenPuzzle.html).

If it is solveable, it searches for the best move set(the one with the lowest number of moves that reaches a *final state*) using _*A**_.

## GameState

```c++

struct GameState
{
	vector<vector<int>> matrix;

	int size, cost_R, costSoFar = 0;
	GameState* parent;
	
};
```

A ```GameState``` is defined by the matrix and these fields:
*   costSoFar - retains the total cost of all the moves so far.
*   cost_R - retains the sum of the total cost + the heuristic
*   parent - retains the adress of the last ```GameState``` and it is used to find the precise move set at the end of the search  


## A* implementation
---
[A*](https://en.wikipedia.org/wiki/A*_search_algorithm) is an informed path search algorithm that uses a heuristic to guide it's path.

This [site](https://www.redblobgames.com/pathfinding/a-star/introduction.html) presents and explains 3 path-finding algorithms, BFS, Dijkstra’s and A*. The conclusion to be taken is that:

> _```BFS < Dijkstra’s < A*```_

My implementation uses a priority queue of ```GameState```s that prioritises the ```cost_R``` field of a given ```GameState```. 

```c++
	priority_queue<GameState*, std::vector<GameState*>, comparator> fringeStates;
```

While we have ```GameState```s in our queue, if we do not have a duplicate state(this ensures the algorithm doesn't run into infinite loops and it also means that we can only check a given configuration only once), we enque this state and the priority queue does it's magic, pushing the ```GameState``` with the lowest ```cost_R``` to the top of the heap.
```c++
enqueueStates(action, fringeStates, newGame, final);
```


If we encounter a ```GameState``` that is a final configuration(all the tiles are sorted), we fill up the ```solution``` vector with all the ```GameState```s from this final one up to the initial and return it.

```c++
if (areEqual(newGame, final))
{
	int counter = 0;
	while (newGame->parent != nullptr)
	{
	    solution.push_back(newGame);
	    newGame = newGame->parent;
    	counter++;
	}
	solution.push_back(initial);
	return make_pair(solution, counter);
}
```