
// La clase AI representa a la inteligencia artificial que controla a 1 de los jugadores.

#ifndef AI_HPP
#define AI_HPP

#include <iostream>
#include <vector>
#include "Player.hpp"

class AI
{
private:
	int currentIndex;
	std::vector<int>* coords;
public:
	AI();
	~AI();
	int getCurrentMoveIndex(Player*, Player*, Checkerboard*); // Da el indice actual
	std::vector<int>* AI_Move(Player*, Player*, Checkerboard*);
};

#endif

