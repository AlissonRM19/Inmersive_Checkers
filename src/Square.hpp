
//La clase Square representa las casillas en el tablero. Y se maneja mediante la clase sf:RectangleShape.

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <SFML/Graphics.hpp>
#include "Global.hpp"

class Square : public sf::RectangleShape
{
private:
	const int row;
	const int col;
	const int idNumber;
	bool occupied; // Revisa si la casilla esta ocupada.
protected:
public:
	Square(const sf::Vector2f&, const int, const int, const int);
	~Square();

	const int getIdNumber();
	bool getOccupied();
	const int getRow();
	const int getCol();

	void setOccupied(bool);
	void setIdNumber(int);
};

#endif