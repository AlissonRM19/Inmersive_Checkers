#include "Moveable.hpp"

using std::vector;
using std::abs;



// Verifica la validez de un movimiento sin importar que la casilla llegada este ocupada
bool Moveable::moveable(std::vector<Checkerpiece*> checkers, Square* current, Square* future, const int& currentIndex)
{
    // Si la ficha que se piensa saltar es amiga
	if(friendly(checkers, future))
		return false;

	if(static_cast<unsigned>(currentIndex) < checkers.size())
	{
		// Si la ficha a mover es un rey
		if(checkers.at(currentIndex)->getKing()) 
		{

            // Si las casillas de salida y llegada son del mismo color
			if(current->getFillColor() == future->getFillColor()) 
			{
				// Comprueba que la distancia este en el rango apropiado
				if(moveableDistance(static_cast<int>(current->getPosition().x), static_cast<int>(current->getPosition().y),
					static_cast<int>(future->getPosition().x), static_cast<int>(future->getPosition().y))) 
						return true;
				else
					return false;
			}
			else
				return false;
		}
		// Si no es rey, es un peon (solo se mueve en diagonal hacia el 'frente' dependiemndo de su fila para coronarse)
		else
		{
			// Si las casillas de salida y llegada son del mismo color
			if(current->getFillColor() == future->getFillColor())
			{
				// Si: KING_ROW_0 (Parte arriba de la ventana)
				if(checkers.at(currentIndex)->getKingRow() == KING_ROW_0)
				{

                    // Peon va hacia ROW 0, entonces coordenada Y debe disminuir
					if(current->getPosition().y > future->getPosition().y)
					{
						if(moveableDistance((int) current->getPosition().x, (int) current->getPosition().y, (int) future->getPosition().x, (int) future->getPosition().y))
							return true;
						else
							return false;
					}
					else
						return false;
				}
				// Si: KING_ROW_7 (parte de abajo de la ventana)
				else if(checkers[currentIndex]->getKingRow() == KING_ROW_7)
				{
					// Peon va hacia ROW 7, entonces coordenada Y debe aumentar
					if(current->getPosition().y < future->getPosition().y)
					{
						if(moveableDistance((int) current->getPosition().x, (int) current->getPosition().y, (int) future->getPosition().x, (int) future->getPosition().y))
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else 
					return false;
			}
			else
				return false;
		}
	}
	else
		return false;
}

// Funcion sobrecargado que verifica que la casilla llegada no este ocupada
bool Moveable::moveable(Checkerpiece* checker, Square* current, Square* future)
{

    // Comprueba que la casilla de llegada no este ocupada
	if(future->getOccupied())
		return false;

    // Si se esta moviendo un rey
	else if(checker->getKing())
	{
		// Si las casillas de salida y llegada son del mismo color
		if(current->getFillColor() == future->getFillColor())
		{
			// No se verifica la direccion del movimineto porque es un rey
			if(moveableDistance((int) current->getPosition().x, (int) current->getPosition().y, (int) future->getPosition().x, (int) future->getPosition().y))
				return true;
			else
				return false;
		}
		else
			return false;
	}

    // Si no es rey, es un peon
	else
	{
        // Si las casillas de salida y llegada son del mismo color
		if(current->getFillColor() == future->getFillColor()) 
		{
            //Si: KING_ROW_0 (parte de arriba de la ventana)
			if(checker->getKingRow() == KING_ROW_0) 
			{

                // Peon va hacia ROW 0, entonces coordenada Y debe disminuir
				if(current->getPosition().y > future->getPosition().y) 
				{
					if(moveableDistance((int) current->getPosition().x, (int) current->getPosition().y, (int) future->getPosition().x, (int) future->getPosition().y))
						return true;
					else
						return false;
				}
				else
					return false;
			}

            // Si: KING_ROW_7 (parte de abajo de la ventana)
			else if(checker->getKingRow() == KING_ROW_7)
			{
				// Peon va hacia ROW 7, entonces coordenada Y debe disminuir
				if(current->getPosition().y < future->getPosition().y)
				{
					if(moveableDistance((int) current->getPosition().x, (int) current->getPosition().y, (int) future->getPosition().x, (int) future->getPosition().y))
						return true;
					else
						return false;
				}
				else
					return false;
			}
			else 
				return false;
		}
		else
			return false;
	}
}

// Comprueba la distancia entre la casilla de salida y llegada con respecto al offset.
bool Moveable::moveableDistance(const int& currentX, const int& currentY, const int& futureX, const int& futureY)
{
	if((abs(currentX - futureX) <= XOFFSET) && (futureX <= (XOFFSET * SQUARES_HORIZONTAL)) && (futureX >= 0))
		if((abs(currentY - futureY) <= YOFFSET) && (futureY <= (YOFFSET * SQUARES_VERTICAL)) && (futureY >= 0))
			return true; // Distancia es valida
	return false; // Distancia no es valida
}

// Encuentra la direccion general de salida con respecto a la llegada (como si fueran angulos)
int Moveable::findGeneralDirection(Square* current, Square* future)
{
    // Pensar la direccion general como la direcion en coordenadas donde el angulo es la direccion
	if((current->getPosition().x < future->getPosition().x) && (current->getPosition().y == future->getPosition().y))
		return 0; // Direccion general al este
	else if((current->getPosition().x > future->getPosition().x) && (current->getPosition().y == future->getPosition().y))
		return 180; // Direccion general al oeste
	else if((current->getPosition().x == future->getPosition().x) && (current->getPosition().y < future->getPosition().y))
		return 270; // Direccion general al sur
	else if((current->getPosition().x == future->getPosition().x) && (current->getPosition().y > future->getPosition().y))
		return 90; // Direccion general al norte
	else if((current->getPosition().x < future->getPosition().x) && (current->getPosition().y > future->getPosition().y))
		return 45; // Direccion general al noreste
	else if((current->getPosition().x < future->getPosition().x) && (current->getPosition().y < future->getPosition().y))
		return 315; // Direccion general al sureste
	else if((current->getPosition().x > future->getPosition().x) && (current->getPosition().y > future->getPosition().y))
		return 135; // Direccion general al noroeste
	else if((current->getPosition().x > future->getPosition().x) && (current->getPosition().y < future->getPosition().y))
		return 225; // Direccion general al suroeste
	else return -1;
}

/*
    Verifica si el jugador puede realizar un salto al seleccionar la casilla a la que quiere llegar
    La casilla actual es la de salida, la temporal es la que se va a saltar y la de futura es la de llegada
*/
bool Moveable::jumpBySquare(std::vector<Checkerpiece*> checkers, Square* current, Square* future, Square* temp, const int& currentIndex)
{
	if(friendly(checkers, temp, future))
		return false;
	if(temp->getOccupied())
		if(moveable(checkers, current, temp, currentIndex)) // Comprueba si el movimiento de la casilla actual a la temporal es valido
			if(moveable(checkers, temp, future, currentIndex)) // Comprueba si el movimiento de la casilla temporal a la futura es valido
				return true;
	return false;						
}

/*
    Verifica si el jugador puede realizar un salto al seleccionar la ficha a la que va a saltar
    La casilla actual es la de salida, la temporal es la de llegada y la de futura es la que se va a saltar
*/
bool Moveable::jumpByChecker(std::vector<Checkerpiece*> checkers, Square* current, Square* future, Square* temp, const int& currentIndex)
{
	if(friendly(checkers, temp, future))
		return false;
	if(!temp->getOccupied())
		if(moveable(checkers, future, temp, currentIndex)) // // Comprueba si el movimiento de la casilla futura a la temporal es valido
			if(moveable(checkers, current, future, currentIndex)) // Comprueba si el movimiento de la casilla actual a la futura es valido
				return true;
	return false;
}


// Comprueba que la ficha intenta saltar a una ficha amiga, al compara la casilla temporal o futura con las coordenadas de las fichas del jugador
bool Moveable::friendly(const vector<Checkerpiece*>& checkers, Square* temp, Square* future)
{
	vector<Checkerpiece*>::const_iterator it;
	for(it = checkers.begin(); it != checkers.end(); ++it)
		if((*it)->getPosition() == temp->getPosition() || (*it)->getPosition() == future->getPosition())
			return true;
	return false;
}

// Comprueba que la ficha intenta saltar a una ficha amiga, al compara la casilla futura con las coordenadas de las fichas del jugador
bool Moveable::friendly(const vector<Checkerpiece*>& checkers, Square* future)
{
	vector<Checkerpiece*>::const_iterator it;
	for(it = checkers.begin(); it != checkers.end(); ++it)
		if((*it)->getPosition() == future->getPosition())
			return true;
	return false;
}


// Comprueba si la ficha en juego puede realizar un salto
bool Moveable::hasJump(Checkerpiece* checker, const vector<Checkerpiece*>& enemy, Checkerboard*& tempBoard)
{
	Square* future;
	int futureX, futureY;
	vector<Checkerpiece*>::const_iterator it;
	for(it = enemy.begin(); it != enemy.end(); ++it)
	{
        // Si NO es rey
		if(!checker->getKing())
		{
            // Si: KING_ROW_7 (se mueve hacia abajo)
			if(checker->getKingRow() == KING_ROW_7)
			{
                // Comprueba la direccion suroeste (255)
				if((*it)->getPosition().x != 0 && abs(checker->getPosition().x - XOFFSET) == (*it)->getPosition().x 
					&& (checker->getPosition().y  + YOFFSET) == (*it)->getPosition().y)
				{

                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) abs(checker->getPosition().x - (2*XOFFSET));
					futureY = (int) checker->getPosition().y + (2*YOFFSET);

					// Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
						return true; // Puede saltar
				}

                // Comprueba la direccion sureste (315)
				else if((checker->getPosition().x + XOFFSET) == (*it)->getPosition().x 
					&& (checker->getPosition().y + YOFFSET) == (*it)->getPosition().y)
				{
					// Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) checker->getPosition().x + (2*XOFFSET);
					futureY = (int) checker->getPosition().y + (2*YOFFSET);

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
						return true; // Puede saltar
				}
			}
            // Si NO es: KING_ROW_7 (se mueve hacia arriba)
			else
			{
                // Comprueba la direccion noreste (45)
				if((*it)->getPosition().y != 0 && (checker->getPosition().x + XOFFSET) == (*it)->getPosition().x 
					&& abs(checker->getPosition().y - YOFFSET) == (*it)->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) checker->getPosition().x + (2*XOFFSET);
					futureY = (int) abs(checker->getPosition().y - (2*YOFFSET));

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
						return true; // Puede saltar
				}

                // Comprueba la direccion noroeste (135)
				else if((*it)->getPosition().y != 0 && (*it)->getPosition().x != 0 && abs(checker->getPosition().x - XOFFSET) == (*it)->getPosition().x 
					&& std::abs(checker->getPosition().y - YOFFSET) == (*it)->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) abs(checker->getPosition().x - (2*XOFFSET));
					futureY = (int) abs(checker->getPosition().y - (2*YOFFSET));

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
						return true; // Puede saltar
				}
			}
		}

        // Es rey, comprueba los 4 casos
		else
		{
            // Comprueba la direccion suroeste (255)
			if((*it)->getPosition().x != 0 && checker->getPosition().x != 0 && std::abs(checker->getPosition().x - XOFFSET) == (*it)->getPosition().x 
				&& (checker->getPosition().y  + YOFFSET) == (*it)->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) std::abs(checker->getPosition().x - (2*XOFFSET));
				futureY = (int) checker->getPosition().y + (2*YOFFSET);

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
					return true; // Puede salta
			}

            // Comprueba la direccion sureste (315)
			else if((checker->getPosition().x + XOFFSET) == (*it)->getPosition().x 
				&& (checker->getPosition().y + YOFFSET) == (*it)->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) checker->getPosition().x + (2*XOFFSET);
				futureY = (int) checker->getPosition().y + (2*YOFFSET);

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
					return true; // Puede salta
			}

            // Comprueba la direccion noreste (45)
			else if((*it)->getPosition().y != 0 && (checker->getPosition().x + XOFFSET) == (*it)->getPosition().x 
				&& std::abs(checker->getPosition().y - YOFFSET) == (*it)->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) checker->getPosition().x + (2*XOFFSET);
				futureY = (int) std::abs(checker->getPosition().y - (2*YOFFSET));

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
					return true; // Puede salta
			}

            // Comprueba la direccion noroeste (135)
			else if((*it)->getPosition().y != 0 && (*it)->getPosition().x != 0 && checker->getPosition().x != 0 
				&& std::abs(checker->getPosition().x - XOFFSET) == (*it)->getPosition().x 
					&& std::abs(checker->getPosition().y - YOFFSET) == (*it)->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) abs(checker->getPosition().x - (2*XOFFSET));
				futureY = (int) abs(checker->getPosition().y - (2*YOFFSET));

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
					return true; // Puede salta
			}
		}
	}
	return false;
}

// Comprueba si la ficha en juego puede realizar un salto, regresa las coordenadas del salto (actual,futura y saltada)
vector<int>* Moveable::findJump(Checkerpiece* checker, const vector<Checkerpiece*>& enemy, Checkerboard*& tempBoard)
{
    // Coordenadas de la ficha activa actual, despues del salto y las de la ficha saltada
	const int TOTAL_POSITIONS = 6;
	vector<int>* coords = new vector<int> (TOTAL_POSITIONS);
	coords->reserve(TOTAL_POSITIONS);
	Square* future;
	int futureX, futureY;
	for(unsigned int i = 0; i != enemy.size(); ++i)
	{
        // Si NO es rey
		if(!checker->getKing())
		{
            // Si: KING_ROW_7 (se mueve hacia abajo)
			if(checker->getKingRow() == KING_ROW_7)
			{
                // Comprueba la direccion suroeste (255)
				if(enemy[i]->getPosition().x != 0 && std::abs(checker->getPosition().x - XOFFSET) == enemy[i]->getPosition().x 
					&& (checker->getPosition().y  + YOFFSET) == enemy[i]->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) std::abs(checker->getPosition().x - (2*XOFFSET));
					futureY = (int) checker->getPosition().y + (2*YOFFSET);

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
					{
						coords->at(0) = (int) checker->getPosition().x;
						coords->at(1) = (int) checker->getPosition().y;
						coords->at(2) = (int) enemy[i]->getPosition().x;
						coords->at(3) = (int) enemy[i]->getPosition().y;
						coords->at(4) = futureX;
						coords->at(5) = futureY;
						return coords; // Regresa las coordenadas del salto
					}
				}

                // Comprueba la direccion sureste (315)
				else if((checker->getPosition().x + XOFFSET) == enemy[i]->getPosition().x 
					&& (checker->getPosition().y + YOFFSET) == enemy[i]->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) checker->getPosition().x + (2*XOFFSET);
					futureY = (int) checker->getPosition().y + (2*YOFFSET);

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
					{
						coords->at(0) = (int) checker->getPosition().x;
						coords->at(1) = (int) checker->getPosition().y;
						coords->at(2) = (int) enemy[i]->getPosition().x;
						coords->at(3) = (int) enemy[i]->getPosition().y;
						coords->at(4) = futureX;
						coords->at(5) = futureY;
						return coords; // Regresa las coordenadas del salto
					}
				}
			}

            // Si NO es: KING_ROW_7 (se mueve hacia arriba)
			else
			{
                // Comprueba la direccion noreste (45)
				if(enemy[i]->getPosition().y != 0 && (checker->getPosition().x + XOFFSET) == enemy[i]->getPosition().x 
					&& std::abs(checker->getPosition().y - YOFFSET) == enemy[i]->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) checker->getPosition().x + (2*XOFFSET);
					futureY = (int) std::abs(checker->getPosition().y - (2*YOFFSET));

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
					{
						coords->at(0) = (int) checker->getPosition().x;
						coords->at(1) = (int) checker->getPosition().y;
						coords->at(2) = (int) enemy[i]->getPosition().x;
						coords->at(3) = (int) enemy[i]->getPosition().y;
						coords->at(4) = futureX;
						coords->at(5) = futureY;
						return coords; // Regresa las coordenadas del salto
					}
				}

                // Comprueba la direccion noroeste (135)
				else if(enemy[i]->getPosition().y != 0 && enemy[i]->getPosition().x != 0 && std::abs(checker->getPosition().x - XOFFSET) == enemy[i]->getPosition().x 
					&& std::abs(checker->getPosition().y - YOFFSET) == enemy[i]->getPosition().y)
				{
                    // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
					futureX = (int) std::abs(checker->getPosition().x - (2*XOFFSET));
					futureY = (int) std::abs(checker->getPosition().y - (2*YOFFSET));

                    // Ve si la casilla de llegada esta ocupada
					future = tempBoard->findSquare(futureX, futureY);
					if(future && !future->getOccupied())
					{
						coords->at(0) = (int) checker->getPosition().x;
						coords->at(1) = (int) checker->getPosition().y;
						coords->at(2) = (int) enemy[i]->getPosition().x;
						coords->at(3) = (int) enemy[i]->getPosition().y;
						coords->at(4) = futureX;
						coords->at(5) = futureY;
						return coords; // Regresa las coordenadas del saltop
					}
				}
			}
		}

        // Es rey, comprueba los 4 caso
		else
		{
            // Comprueba la direccion suroeste (255)
			if(enemy[i]->getPosition().x != 0 && checker->getPosition().x != 0 && std::abs(checker->getPosition().x - XOFFSET) == enemy[i]->getPosition().x 
				&& (checker->getPosition().y  + YOFFSET) == enemy[i]->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) std::abs(checker->getPosition().x - (2*XOFFSET));
				futureY = (int) checker->getPosition().y + (2*YOFFSET);

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
				{
					coords->at(0) = (int) checker->getPosition().x;
					coords->at(1) = (int) checker->getPosition().y;
					coords->at(2) = (int) enemy[i]->getPosition().x;
					coords->at(3) = (int) enemy[i]->getPosition().y;
					coords->at(4) = futureX;
					coords->at(5) = futureY;
					return coords; // Regresa las coordenadas del salto
				}
			}

            // Comprueba la direccion sureste (315)
			else if((checker->getPosition().x + XOFFSET) == enemy[i]->getPosition().x 
				&& (checker->getPosition().y + YOFFSET) == enemy[i]->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) checker->getPosition().x + (2*XOFFSET);
				futureY = (int) checker->getPosition().y + (2*YOFFSET);

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
				{
					coords->at(0) = (int) checker->getPosition().x;
					coords->at(1) = (int) checker->getPosition().y;
					coords->at(2) = (int) enemy[i]->getPosition().x;
					coords->at(3) = (int) enemy[i]->getPosition().y;
					coords->at(4) = futureX;
					coords->at(5) = futureY;
					return coords; // Regresa las coordenadas del salto
				}
			}

            // Comprueba la direccion noreste (45)
			else if(enemy[i]->getPosition().y != 0 && (checker->getPosition().x + XOFFSET) == enemy[i]->getPosition().x 
				&& std::abs(checker->getPosition().y - YOFFSET) == enemy[i]->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) checker->getPosition().x + (2*XOFFSET);
				futureY = (int) std::abs(checker->getPosition().y - (2*YOFFSET));

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
				{
					coords->at(0) = (int) checker->getPosition().x;
					coords->at(1) = (int) checker->getPosition().y;
					coords->at(2) = (int) enemy[i]->getPosition().x;
					coords->at(3) = (int) enemy[i]->getPosition().y;
					coords->at(4) = futureX;
					coords->at(5) = futureY;
					return coords; // Regresa las coordenadas del salto
				}
			}

            // Comprueba la direccion noroeste (135)
			else if(enemy[i]->getPosition().y != 0 && enemy[i]->getPosition().x != 0 && checker->getPosition().x != 0 
				&& std::abs(checker->getPosition().x - XOFFSET) == enemy[i]->getPosition().x 
					&& std::abs(checker->getPosition().y - YOFFSET) == enemy[i]->getPosition().y)
			{
                // Ficha enemiga esta en rango de salto, comprueba que tenga una ficha (amiga o enemiga) para saltar
				futureX = (int) std::abs(checker->getPosition().x - (2*XOFFSET));
				futureY = (int) std::abs(checker->getPosition().y - (2*YOFFSET));

                // Ve si la casilla de llegada esta ocupada
				future = tempBoard->findSquare(futureX, futureY);
				if(future && !future->getOccupied())
				{
					coords->at(0) = (int) checker->getPosition().x;
					coords->at(1) = (int) checker->getPosition().y;
					coords->at(2) = (int) enemy[i]->getPosition().x;
					coords->at(3) = (int) enemy[i]->getPosition().y;
					coords->at(4) = futureX;
					coords->at(5) = futureY;
					return coords; // Regresa las coordenadas del salto
				}
			}
		}
	}

	return nullptr; // Regresa las coordenadas del salto vacias
}

// Comprueba si la ficha en juego puede realizar un movimiento
bool Moveable::hasMove(Checkerpiece* active, const vector<Checkerpiece*>& enemy, Checkerboard*& checkerboard)
{
	const int TOTAL_POSITIONS = 4;
	std::vector<int> coords;
	coords.reserve(TOTAL_POSITIONS);
	int currentX = (int) active->getPosition().x;
	int currentY = (int) active->getPosition().y;
	int futureX, futureY;
	Square* current, *future; // Casillas actual y futura

	// Realiza iteraciones para los posibles casos

    // Comprueba la direccion noreste (45)
	futureX = currentX + XOFFSET;
	futureY = currentY + YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future))
		if(!future->getOccupied())
			return true;

    // Comprueba la direccion suroeste (255)
	futureX = currentX - XOFFSET;
	futureY = currentY - YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future))
		if(!future->getOccupied())
			return true;

    // Comprueba la direccion noroeste (135)
	futureX = currentX - XOFFSET;
	futureY = currentY + YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future))
		if(!future->getOccupied())
			return true;

    // Comprueba la direccion sureste (315)
	futureX = currentX + XOFFSET;
	futureY = currentY - YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future))
		if(!future->getOccupied())
			return true;
	
	return false; // No hay movimiento disponible
}

/*
*	Find a simple move (no jump), based on the first unoccupied
*	square that is within moveable distance to the active checkerpiece.
*	We simply try different combinations of adding, subtracting
*	the X/Y Offsets until we find one that's moveable.
*	the coords return corresponds to (coords[0,1] = currentX,Y, and coords[2,3] = futureX,Y).
*/

// Comprueba si la ficha en juego puede realizar un movimiento, regresa las coordenadas del movimiento (actual y futura)
vector<int>* Moveable::findMove(Checkerpiece* active, Checkerboard*& checkerboard)
{
	const int TOTAL_POSITIONS = 4;
	vector<int>* coords = new vector<int> (TOTAL_POSITIONS);
	coords->reserve(TOTAL_POSITIONS);
	int currentX = static_cast<int>(active->getPosition().x);
	int currentY = static_cast<int>(active->getPosition().y);
	int futureX, futureY;
	Square* current, *future; // Casillas actual y futura

    // Realiza iteraciones para los posibles casos

    // Comprueba la direccion noreste (45)
	futureX = currentX + XOFFSET;
	futureY = currentY + YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future) 
		&& !future->getOccupied())
	{
		coords->at(0) = currentX;
		coords->at(1) = currentY;
		coords->at(2) = futureX;
		coords->at(3) = futureY;
		return coords; // Regresa las coordenadas del movimiento
	}

    // Comprueba la direccion suroeste (255)
	futureX = currentX - XOFFSET;
	futureY = currentY - YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future) 
		&& !future->getOccupied())
	{
		coords->at(0) = currentX;
		coords->at(1) = currentY;
		coords->at(2) = futureX;
		coords->at(3) = futureY;
		return coords; // Regresa las coordenadas del movimiento
	}

    // Comprueba la direccion noroeste (135)
	futureX = currentX - XOFFSET;
	futureY = currentY + YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future) 
		&& !future->getOccupied())
	{
		coords->at(0) = currentX;
		coords->at(1) = currentY;
		coords->at(2) = futureX;
		coords->at(3) = futureY;
		return coords; // Regresa las coordenadas del movimiento
	}

    // Comprueba la direccion sureste (315)
	futureX = currentX + XOFFSET;
	futureY = currentY - YOFFSET;
	current = checkerboard->findSquare(currentX, currentY);
	future = checkerboard->findSquare(futureX, futureY);
	if(current != nullptr && future != nullptr && moveable(active, current, future) 
		&& !future->getOccupied())
	{
		coords->at(0) = currentX;
		coords->at(1) = currentY;
		coords->at(2) = futureX;
		coords->at(3) = futureY;
		return coords; // Regresa las coordenadas del movimiento
	}

	return nullptr; // Regresa las coordenadas del movimiento vacias
}