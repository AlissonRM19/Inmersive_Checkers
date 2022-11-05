/*
*	The checkerpiece class represents the checkers. 
*	Each team starts with 12 checkerpieces. This class handles drawing to checkerboard (called within CheckerGame.cpp),
*	and construction of checkerpieces.
*	The actual checkerpieces are managed in the Player class (each player has a vector container).
*/

#ifndef CHECKERPIECE_HPP
#define CHECKERPIECE_HPP

#include <SFML/Graphics.hpp> // allows window drawing
#include <SFML/Window.hpp> // allows window instantation
#include <SFML/System.hpp> // needed for window instantion (and sf::Time)
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
	Checkerpiece(); // dummy constructor for drawing object in CheckerGame class
	Checkerpiece(sf::Color, int, int, int, const int, const int, const int);
	
	void drawCheckers(sf::RenderWindow&, const std::vector<Checkerpiece*>&);

	// setters
	void setKing(bool);
	//void setRank(int);
	// getters
	bool getKing();
	const int getKingRow();
	const int getStartingRow();
	const int getPlayer();
	//int getRank();
};

#endif