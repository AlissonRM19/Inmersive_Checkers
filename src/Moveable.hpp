
/*
    La clase Moveable representa las validaciones de los movimiento realizados por las fichas.
    Las validaciones se dan con respecto a basicas reglas del juego de Checkers.
 */

#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include "Checkerboard.hpp"
#include "Checkerpiece.hpp"
#include "Player.hpp"
#include <vector>
#include <cmath> // for absolute function

class Moveable
{
private:
protected:
public:
	static bool moveable(Checkerpiece*, Square*, Square*);
	static bool moveable(std::vector<Checkerpiece*>, Square*, Square*, const int&);
	static bool moveableDistance(const int&, const int&, const int&, const int&);
	static bool friendly(const std::vector<Checkerpiece*>&, Square*);
	static bool friendly(const std::vector<Checkerpiece*>&, Square*, Square*);
	static int findGeneralDirection(Square*, Square*);
	static bool jumpBySquare(std::vector<Checkerpiece*>, Square*, Square*, Square*, const int&);
	static bool jumpByChecker(std::vector<Checkerpiece*>, Square*, Square*, Square*, const int&);
	static bool hasJump(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // Valida si una ficha puede saltar
	static bool hasMove(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // Valida si una ficha puede moverse
	static std::vector<int>* findJump(Checkerpiece*, const std::vector<Checkerpiece*>&, Checkerboard*&); // Cuando el AI necesita encontrar un salto
	static std::vector<int>* findMove(Checkerpiece*, Checkerboard*&); // Cuando el AI debe encontrar un movimiento
};

#endif