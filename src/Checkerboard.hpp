
// La clase Checkerboard representa el tablero. De aqui es donde se llaman las funciones de busqueda de casillas

#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <SFML/Graphics.hpp> // Permite dibujar en la ventana
#include <SFML/Window.hpp> // Permite instancias de la ventana
#include <SFML/System.hpp> // Necesaria para las instancias de la ventana
#include <iostream>
#include "Square.hpp"

class Checkerboard : public sf::FloatRect
{
private:
	Square* squareArray[SQUARES_HORIZONTAL][SQUARES_VERTICAL];

public:
	Checkerboard(sf::RenderWindow&);
	~Checkerboard();

	Square* findSquare(int, int);
	Square* findSquare(int);
	Square* findJumpOntoSquare(Square*, int);
	Square* findIntermSquare(Square*, int);
	void drawGrid(sf::RenderWindow&, int, int, int, int, bool);
	void printTheSquares();
	void initSquareArray();
	bool intersects(const sf::RectangleShape, const sf::RectangleShape);

	Square** getSquareArray();
};
#endif 