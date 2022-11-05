
/*
    La clase Checkerpiece representa las fichas. Donde cada jugador tiene 12 fichas.
    esta clase tambien se encarga de dibujar el tablero pero es llamado en el CheckerGame.cpp.
*/

#ifndef CHECKERPIECE_HPP
#define CHECKERPIECE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Global.hpp"

class Checkerpiece : public sf::CircleShape
{
private:
protected:
	const int startingRow;
	const int kingRow;
	const int player;
	bool isKing;
public:
	Checkerpiece(); // Constructor para dibujar objetos en la clase CheckerGame
	Checkerpiece(sf::Color, int, int, int, const int, const int, const int);
	
	void drawCheckers(sf::RenderWindow&, const std::vector<Checkerpiece*>&);

	void setKing(bool);

	bool getKing();
	const int getKingRow();
	const int getStartingRow();
	const int getPlayer();
};

#endif