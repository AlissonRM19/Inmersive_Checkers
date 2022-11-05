#include "Checkerboard.hpp"

Checkerboard::Checkerboard(sf::RenderWindow& window)
{
	initSquareArray(); // Inicializa un array de casillas
}

Checkerboard::~Checkerboard()
{
    // ELimina punteros en el array de casillas
	for(int i = 0; i < SQUARES_VERTICAL; ++i)
	{
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{
			delete squareArray[i][j];
			squareArray[i][j] = nullptr;
		}
	}
}

// Inicializa el array de casillas y determina si una casilla esta ocupada al inicio del juego
void Checkerboard::initSquareArray()
{
	Square* tempSquare;
	int idNumber = 0; // Casilla 0 es dibujada en (0,0) y tiene un id 0

	for(int i = 0; i < SQUARES_VERTICAL; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{	
			// Inicializa los objetos casilla arbitrarios
			tempSquare = new Square(sf::Vector2f(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET)), i, j, idNumber++);
			squareArray[i][j] = tempSquare;
			
			if(((j % 2 != 0 && i % 2 == 0) || (j % 2 == 0 && i % 2 != 0)) && (i <= 2 || i >= 5)) // initialize all the black squares in rows 0-2, 5-7
				squareArray[i][j]->setOccupied(true);
			else
				squareArray[i][j]->setOccupied(false);
		}
	}
}

/*
*	Draws an 8 by 8 square grid with checkered color pattern and also highlights the current square (green) and a mouseover square (magenta).
*	The current and mouseOver values come from the CheckerGame, based upon where the player is pointing @ the board.
*/

void Checkerboard::drawGrid(sf::RenderWindow& window, int mouseOverX, int mouseOverY, int currentX, int currentY, bool selecting)
{
	Square* tempSquare;
	int startingX = 0, startingY = 0;
	sf::RectangleShape tempRect;
	tempRect.setPosition(static_cast<float>(currentX), static_cast<float>(currentY));

	for(int i = 0; i < SQUARES_VERTICAL; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{
			if((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
				squareArray[i][j]->setFillColor(sf::Color::White); 
			else
				squareArray[i][j]->setFillColor(sf::Color::Black); 
			
			if(selecting)
			{
				// erase any previous color highlights from all squares
				squareArray[i][j]->setOutlineThickness(0);
				squareArray[i][j]->setOutlineColor(sf::Color(0, 0, 0, 255));

				// find the square in the array that a player is currently mousing over (have to use long version instead of the variable offsets because the offset keeps changing) 
				if(squareArray[i][j] == findSquare(mouseOverX, mouseOverY))
				{
					// give the selected square a nice green highlight
					squareArray[i][j]->setOutlineThickness(-SQUARES_VERTICAL);
					squareArray[i][j]->setOutlineColor(sf::Color::Green);	
				}
			}
			else if(!selecting)
			{
				// erase any previous color highlights from all squares
				squareArray[i][j]->setOutlineThickness(0);
				squareArray[i][j]->setOutlineColor(sf::Color(0, 0, 0, 255));

				// give the selected square a nice green square highlight
				tempSquare = findSquare(currentX, currentY);
				tempSquare->setOutlineThickness(-SQUARES_VERTICAL); // -8, (draws inward)
				tempSquare->setOutlineColor(sf::Color::Green);	

				// find the square in the array that a player is currently mousing over (have to use long version instead of the variable offsets because the offset keeps changing) 
				if(squareArray[i][j] == findSquare(mouseOverX, mouseOverY))
				{
					// give the move to square a nice magenta highlight
					squareArray[i][j]->setOutlineThickness(-SQUARES_VERTICAL);
					squareArray[i][j]->setOutlineColor(sf::Color::Magenta);	
				}
			}

			// prepare to draw
			squareArray[i][j]->setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			squareArray[i][j]->setSize(sf::Vector2f(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET)));
			window.draw(*squareArray[i][j]);

			startingX += XOFFSET;
		}

		startingY += YOFFSET;
		startingX = 0;
	}
}

// Printea las casillas en consola
void Checkerboard::printTheSquares()
{
	for(int i = 0; i < SQUARES_HORIZONTAL; ++i)
		for(int j = 0; j < SQUARES_VERTICAL; ++j)
			std::cout << std::endl << "Square #" << squareArray[i][j]->getIdNumber() << " " <<
				"Draw Coords: " << "(" << squareArray[i][j]->getPosition().x << ", " << squareArray[i][j]->getPosition().y << ") " << 
					"Square Occupied: " << squareArray[i][j]->getOccupied() << std::endl;
}

// Encuentra una casilla (comparando una casilla temporal con el array de casillas hsta encontrar una concordancia)
Square* Checkerboard::findSquare(int x, int y)
{	
	sf::Vector2f size(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET));
	sf::RectangleShape rect2;
	rect2.setPosition(static_cast<float>(x), static_cast<float>(y));
	rect2.setSize(size);

	for(int i = 0; i < SQUARES_VERTICAL; i++)
		for(int j = 0; j < SQUARES_HORIZONTAL; j++)
			if(intersects(*squareArray[i][j], rect2))
				return squareArray[i][j];					

	return nullptr;
}

// Encuentra una casilla (mediante el id)
Square* Checkerboard::findSquare(int idNumber)
{
	for(int i = 0; i < SQUARES_HORIZONTAL; ++i)
		for(int j = 0; j < SQUARES_VERTICAL; ++j)
			if(squareArray[i][j]->getIdNumber() == idNumber)
				return squareArray[i][j];

	return nullptr;
}

// Encuentra una casilla de llegada de un salto
Square* Checkerboard::findJumpOntoSquare(Square* temp, int generalDirection)
{
    // Casilla temporal es la que se va a saltar (encontramos la casilla de llegada del salto)
	sf::Vector2f size(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET));
	sf::RectangleShape rect2;
	rect2.setSize(size);
	float x, y;

	switch(generalDirection)
	{
	case 0: // Direccion general al este
		x = temp->getPosition().x + XOFFSET;
		y = temp->getPosition().y;
		break;
	case 45: // Direccion general al noreste
		x = temp->getPosition().x + XOFFSET;
		y = std::abs(temp->getPosition().y - YOFFSET);
		break;
	case 90: // Direccion general al norte
		x = temp->getPosition().x;
		y = std::abs(temp->getPosition().y - YOFFSET);
		break;
	case 135: // Direccion general al noroeste

        // Solucion a un bug ("kitty corner jump")
		if(temp->getPosition().x == 0) 
			return nullptr;
		else
		{
			x = std::abs(temp->getPosition().x - XOFFSET);
			y = std::abs(temp->getPosition().y - YOFFSET);
		}
		break;
	case 180: // Direccion general al oeste
		x = std::abs(XOFFSET - temp->getPosition().x);
		y = temp->getPosition().y;
		break;
	case 225: // Direccion general al suroeste

        // Solucion a un bug ("kitty corner jump")
		if(temp->getPosition().x == 0) 
			return nullptr;
		else
		{
			x = std::abs(XOFFSET - temp->getPosition().x);
			y = temp->getPosition().y + YOFFSET;
		}	
		break;
	case 270: // Direccion general al sur
		x = temp->getPosition().x;
		y = YOFFSET + temp->getPosition().y;
		break;
	case 315: // Direccion general al sureste
		x = temp->getPosition().x + XOFFSET;
		y = temp->getPosition().y + YOFFSET;
		break;
	default: std::cout << "CANNOT FIND JUMPONTOSQUARE, UNKNOWN GENERAL DIRECTION " << std::endl; return nullptr;
	}

	rect2.setPosition(x, y);

	for(int i = 0; i < SQUARES_HORIZONTAL; ++i)
		for(int j = 0; j < SQUARES_VERTICAL; ++j)
			if(intersects(*squareArray[i][j], rect2))
				return squareArray[i][j];

	return nullptr;
}

// Encuentra una casilla intermedia (casilla saltada)
Square* Checkerboard::findIntermSquare(Square* temp, int generalDirection)
{
    // Casilla temporal es la que se va a saltar (encontramos la casilla intermedia desde casilla llegada del salto)
	sf::Vector2f size(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET));
	sf::RectangleShape rect2;
	rect2.setSize(size);
	float x, y;

	switch(generalDirection)
	{
	case 0: // Direccion general al este
		x = std::abs(temp->getPosition().x - XOFFSET);
		y = temp->getPosition().y;
		break;
	case 45: // Direccion general al este
		x = std::abs(temp->getPosition().x - XOFFSET);
		y = temp->getPosition().y + YOFFSET;
		break;
	case 90: // Direccion general al este
		x = temp->getPosition().x;
		y = temp->getPosition().y + YOFFSET;
		break;
	case 135: // Direccion general al este
		x = XOFFSET + temp->getPosition().x;
		y = temp->getPosition().y + YOFFSET;
		break;
	case 180: // Direccion general al este
		x = temp->getPosition().x + XOFFSET;
		y = temp->getPosition().y;
		break;
	case 225: // Direccion general al este
		x = XOFFSET + temp->getPosition().x;
		y = std::abs(temp->getPosition().y - YOFFSET);
		break;
	case 270: // Direccion general al este
		x = temp->getPosition().x;
		y = std::abs(temp->getPosition().y - YOFFSET);
		break;
	case 315: // Direccion general al este
		x = std::abs(XOFFSET - temp->getPosition().x);
		y = std::abs(temp->getPosition().y - YOFFSET);
		break;
	default: std::cout << "CANNOT FIND INTERMSQUARE, UNKNOWN GENERAL DIRECTION" << std::endl; return nullptr;
	}

	rect2.setPosition(x, y);

	for(int i = 0; i < SQUARES_HORIZONTAL; ++i)
		for(int j = 0; j < SQUARES_VERTICAL; ++j)
			if(intersects(*squareArray[i][j], rect2))
				return squareArray[i][j];

	return nullptr;
}

Square** Checkerboard::getSquareArray()
{
	return *squareArray;
}

// Comprueba intersecciones entre pixeles de la figura 1 y la figura 2
bool Checkerboard::intersects(const sf::RectangleShape shape1, const sf::RectangleShape shape2)
{
	sf::FloatRect r1 = shape1.getGlobalBounds();
	sf::FloatRect r2 = shape2.getGlobalBounds();
	return r1.intersects (r2);
}