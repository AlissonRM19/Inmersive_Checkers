
/*
    La clase Player representa los jugadores. Se puede tener hasta 2 jugadores donde al menos 1 es controlador por un usuario
    mientras que el segundo puede ser controlado por un AI o tambien por un usuario.
*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Checkerboard.hpp"
#include "Checkerpiece.hpp"
#include "Moveable.hpp"
#include <vector>
#include <iostream>
#include <string>

class Player
{
private:
	const int number;
	bool turn;
	const bool isHuman; // Revisa si el jugador es humano o un AI
	std::vector<Checkerpiece*> checkers; // Almacena la cantidad de fichas del jugador
protected:
public:
	Player(const int, const bool);
	~Player();

	int findCheckerIndex(const int&, const int&);
	int findCheckerIndex(const Square*);
	int* findJump (Square*, int, int, int, int);
	void deleteChecker(const int&);
	void deleteAllCheckers();
	void displayCheckers();
	void addChecker(Checkerpiece*);

	bool getTurn();
    int getCounter();
	const bool getIsHuman();
	const int getNumber();
	Checkerpiece* getChecker(const int&);
	std::vector<Checkerpiece*>& getCheckersVector();

	void setTurn(const bool&);
};
#endif 