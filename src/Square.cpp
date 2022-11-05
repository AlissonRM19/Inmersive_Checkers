
#include "Square.hpp"

Square :: Square(const sf::Vector2f& area, int row, int col, int idNumber) 
	: sf::RectangleShape(area),
	row(row),
	col(col),
	idNumber(idNumber)
{

}

Square :: ~Square()
{
	
}

bool Square :: getOccupied()
{
	return occupied;
}

const int Square :: getRow()
{
	return row;
}

const int Square :: getCol()
{
	return col;
}

const int Square :: getIdNumber()
{
	return idNumber;
}

void Square :: setOccupied(bool occupied)
{
	this->occupied = occupied;
}