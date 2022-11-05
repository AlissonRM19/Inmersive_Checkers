
#include "AI.hpp"

using std::vector;
using std::cout;
using std::endl;

// Constructor Predeternimado
AI::AI() 
{
	
}

AI::~AI() 
{
	delete coords;
}

// Devuelve el indice actual de la ficha a mover mediante la clase Moveable
int AI::getCurrentMoveIndex(Player* active, Player* enemy, Checkerboard* checkerboard)
{
	vector<Checkerpiece*> activeCheckers = active->getCheckersVector();
	vector<Checkerpiece*> enemyCheckers = enemy->getCheckersVector();

    // Hace iteraciones para encontrar una ficha con posibilidad de salto
	for(unsigned int i = 0; i != activeCheckers.size(); ++i)
		if(Moveable::hasJump(activeCheckers.at(i), enemyCheckers, checkerboard))
		{
			cout << "P2 has found a jump @ index " << i << endl;
			currentIndex = i;
			return i;
		}
	// Hace iteraciones para encontrar una ficha con posibilidad de movimiento
	for(unsigned int i = 0; i != activeCheckers.size(); ++i)
		if(Moveable::hasMove(activeCheckers.at(i), enemyCheckers, checkerboard))
		{
			cout << "P2 has found a move @ index " << i << endl;
			currentIndex = i;
			return i;
		}
	return -1; // Implica el fion del juego (no hay fichas para mover)
}

/*
    Se usa para determinarun posible movimiento, ya sea un salto o un movimiento
    tambien se encarga de modificar variables como las posiciones de la ficha a mover
    o la eliminacion de una ficha saltada.
*/
vector<int>* AI::AI_Move(Player* active, Player* enemy, Checkerboard* checkerboard) 
{
	vector<Checkerpiece*> activeCheckers = active->getCheckersVector();
	vector<Checkerpiece*> enemyCheckers = enemy->getCheckersVector();
	for(unsigned i = 0; i != activeCheckers.size(); ++i)
	{

        // Comprueba si la ficha i puede saltar
		coords = Moveable::findJump(activeCheckers.at(i), enemyCheckers, checkerboard); // get coordinates
		if(coords != nullptr)
		{
			cout << "P2 has found a jump->coords[0,1] " << coords->at(0) << " " << coords->at(1) << endl;
			return coords; // Se detienen y regresa las coordenadas
		}
	}

	// Compruba si la ficha puede realizar movimiento, porque ya sabemos que no hay saltos posibles
	for(int i = 0; i != activeCheckers.size(); ++i)
	{
		coords = Moveable::findMove(activeCheckers.at(i), checkerboard);
		if(coords != nullptr)
		{
			cout << "P2 has found a move->coords[0,1] " << coords->at(0) << " " << coords->at(1) << endl;
			return coords;
		}
	}

	return coords; // Si se llega hasta aqui, implica el fin del juego
}
