
#include "Checkerpiece.hpp"

using std::vector;

// Contructor para el dibujar el tablero llamado en CheckersGame
Checkerpiece::Checkerpiece()
	: startingRow(0),
	kingRow(0),
	player(0)
{

}

Checkerpiece::Checkerpiece(sf::Color color, int radius, int startingX, int startingY, const int kingRow, const int startingRow, const int player) 
	: kingRow(kingRow),
	startingRow(startingRow), 
	player(player),
	sf::CircleShape(static_cast<float>(radius))
{
	setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
	isKing = false;
	setFillColor(color);
}

bool Checkerpiece::getKing()
{
	return isKing;
}

const int Checkerpiece::getKingRow()
{
	return kingRow;
}

const int Checkerpiece::getStartingRow()
{
	return startingRow;
}

const int Checkerpiece::getPlayer()
{
	return player;
}

void Checkerpiece::setKing(bool isKing)
{
	this->isKing = isKing;
}

// Dibuja las piezas en la ventana SFML
void Checkerpiece::drawCheckers(sf::RenderWindow& window, const vector<Checkerpiece*>& checkers)
{
	sf::CircleShape circle;

	// DIbuja si el vector no esta vacio
	if(!checkers.empty())
	{
        // Comprueba que el ancho y alto de la ventana sea el mismo
		if(XOFFSET == YOFFSET){
            circle.setRadius(WINDOW_WIDTH / (2 * SQUARES_HORIZONTAL)); // Da el mismo radio a todas las fichas
        }

		else if(XOFFSET > YOFFSET){
            circle.setRadius(WINDOW_HEIGHT / (2 * SQUARES_HORIZONTAL));
        }

		else{
            circle.setRadius(WINDOW_WIDTH / (2 * SQUARES_HORIZONTAL));
        }

		
		vector<Checkerpiece*>::const_iterator it;
		for(it = checkers.begin(); it != checkers.end(); ++it)
		{
			circle.setPosition((*it)->getPosition().x, (*it)->getPosition().y);
			circle.setFillColor((*it)->getFillColor());

            // Resalta la ficha si es rey
			if((*it)->getKing())
            {
				circle.setOutlineThickness(-10);
				circle.setOutlineColor(sf::Color::Yellow);
			}
			else
            {
				circle.setOutlineThickness(0);
				circle.setOutlineColor(sf::Color(0, 0, 0, 255));
			}
			window.draw(circle);
		}
	}
}