#include "CheckerGame.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

int CheckerGame::winner = 0; // No hay ganador al inicio

CheckerGame::CheckerGame(sf::RenderWindow& window, const bool& isHuman1, const bool& isHuman2)
{
	isPlaying = false; // Loop del juego
	checkerboard = new Checkerboard(window); // Crea un nuevo tablero
	p1 = new Player (1, isHuman1); // Crea un Jugador 1
	p2 = new Player (2, isHuman2); // Crea un Jugador 2
}

CheckerGame::~CheckerGame()
{
	//Borra los datos de un juego
	delete checkerboard;
	checkerboard = nullptr;
	delete p1;
	delete p2;
	p1 = nullptr;
	p2 = nullptr;
}

void CheckerGame::startCheckers(sf::RenderWindow& window, sf::Event& event)
{
	p1->setTurn(true); // Jugador 1 empieza el juego
	p2->setTurn(false);
	createTeams();
	isPlaying = true;

	// Empieza el loop del juego
	gameLoop(window, event);
}

// Loop del juego
void CheckerGame::gameLoop(sf::RenderWindow& window, sf::Event& event)
{
	int currentX = 0, currentY = 0; // Las coordenadas actuales
	int futureX = 0, futureY = 0; // Las coordenadas futuras
	int mouseOverX = 0, mouseOverY = 0; // Se usa para resaltar la casilla seleccionada
	bool selecting = true; // Deternima si el juegador esta seleccionando un salto o movimiento
	bool checkDoubleJump = false; // Compureba si hay un doble salto
	int currentIndex = 0; // Indice de la ficha actual (seleccionada) con respecto al vector de fichas del jugador
	int deleteIndex = 0; // Indice de la ficha actual (eliminada) con respecto al vector de fichas del jugador
	int generalDirection = 0; //  Direccion General
	Square* currentSquare = nullptr; // Casilla actual
	Square* futureSquare = nullptr; // Casilla futura
	Square* tempSquare = nullptr; // Casilla temporal
	bool activePlayerIsHuman = isActivePlayerHuman(); // Comprueba que los eventos en la ventana sean realizados solo por jugadores humanos

    // Juego continua hasta que haya un ganador o la ventana sea cerrada
	while(isPlaying)
	{
/************************************************************* MOVIMIENTOS DEL AI  ******************************************/

        // Turno del Jugador 2 cuando es un AI
		if(p2->getTurn() && !p2->getIsHuman())
		{
			// Creacion de un AI
			AI ai;
			currentIndex = ai.getCurrentMoveIndex(p2, p1, checkerboard);

            // Antes de otorgar coordenadas
			if(currentIndex == -1)
			{
				// Fin del juego para el Jugador 2
				p2->deleteAllCheckers();
			}

            // Coordenadas son de size 4 o 6
			else
			{
				std::vector<int> coords;
				coords = *ai.AI_Move(p2, p1, checkerboard);
				std::cout << "(AI) P2 turn : currentIndex " << currentIndex << std::endl;
				std::cout << "(AI) P2 turn : coord size " << coords.size() << std::endl;
				if(coords.size() == 4) // Si es de size 4 equivale a un movimiento
				{
					currentX = coords[0];
					currentY = coords[1];
					mouseOverX = coords[2];
					mouseOverY = coords[3];

					// Actualizamos variables (mediante movimiento)
					currentSquare = checkerboard->findSquare(coords[0], coords[1]); // Casilla salida
					futureSquare = checkerboard->findSquare(coords[2], coords[3]); // Casilla llegada
					if(currentSquare != nullptr && futureSquare != nullptr)
					{
						currentSquare->setOccupied(false); // La casilla donde la ficha estaba
						futureSquare->setOccupied(true); // La casilla donde la ficha va a estar
						p2->getChecker(currentIndex)->setPosition((float) coords[2], (float) coords[3]); // Actualiza la posicion de la ficha actual

						// Comprueba si la ficha se corono
						ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
						printChecker(p2->getChecker(currentIndex), "MOVING");
						changeTurn(); // Cambia al turno del Jugador 1
						selecting = true;
					}
				}
				else if(coords.size() == 6) // Si es de size 6 equivale a un salto
				{
					currentX = coords[0];
					currentY = coords[1];
					mouseOverX = coords[2];
					mouseOverY = coords[3];

                    // Actualizamos variables (mediante salto)
					currentSquare = checkerboard->findSquare(coords[0], coords[1]); // Casilla salida
					futureSquare = checkerboard->findSquare(coords[2], coords[3]); // Casilla saltada
					tempSquare = checkerboard->findSquare(coords[4], coords[5]); // Casilla llegada
					if(currentSquare != nullptr && futureSquare != nullptr && tempSquare != nullptr)
					{
						currentSquare->setOccupied(false); // La casilla donde la ficha estaba
						futureSquare->setOccupied(false); // La casilla donde la ficha va a estar
						tempSquare->setOccupied(true); // La casilla que se salto
						p2->getChecker(currentIndex)->setPosition((float) coords[4], (float) coords[5]); // update current checker's position

                        // Comprueba si la ficha se corono
						ifCheckerKinged(p2->getChecker(currentIndex), tempSquare);
						printChecker(p2->getChecker(currentIndex), "JUMPING");

						// Elimina la ficha que se salto
						deleteCheckerFromGame(p1, p1->findCheckerIndex(futureSquare));
						changeTurn(); // Cambia al turno del Jugador 1
						selecting = true;
					}
				}
			}
		}
		else if(!p2->getIsHuman() && !p1->getIsHuman()) // Ambos jugadores son AIs
		{
			// Ambos usan la clase AI
			// Cambios de variables y eliminacion de fichas se maneja en la clase Player
			changeTurn(); // Cambia al turno del Jugador 2
			selecting = true;
		}
/*********************************************************** FIN DE MOVIMIENTOS DEL AI ************************************/

		// Asumimos que los eventos en ventana son hechos por un jugador humano
		while(activePlayerIsHuman && window.pollEvent(event))
		{
			// Cierra la aplicacion
			if(event.type == sf::Event::Closed)  
			{
				std::cout << std::endl << "Checkers Application Terminated." << std::endl;
				isPlaying = false;
				window.close();
			}

			// Regresa al menu
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				std::cout << std::endl << "Terminating the game, returning to Main Menu." << std::endl;
				isPlaying = false;
			}

            // Maneja movimientos del juego
			else if (event.type == sf::Event::MouseButtonPressed)
			{
                // Selecciona una ficha para mover
				if(event.mouseButton.button == sf::Mouse::Left)
				{
                    // Jugador esta seleccionando una ficha para mover
					if(selecting)
					{
						// Obtiene las coordenadas donde se hace click
						currentX = event.mouseButton.x;
						currentY = event.mouseButton.y;
						currentSquare = checkerboard->findSquare(currentX, currentY);

/********************************************************** JUGADOR SELECCIONANDO ********************************************************************************/

                        // Valida la seleccion
						if(currentSquare != nullptr && currentSquare->getOccupied())
						{
                            // Turno del Jugador 1
							if(p1->getTurn())
							{
								currentIndex = p1->findCheckerIndex(currentSquare); 
								if(currentIndex != -1 && currentIndex < p1->getCounter())
								{
									printChecker(p1->getChecker(currentIndex), "SELECTED");

                                    // Comprueba si la ficha actual va a saltar
									if(Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard)) 
									{
										// La ficha actual tiene un salto, la seleccion es valida
										selecting = false;
									}
									else
									{
										if(playerHasToJump(p1, p2))
                                            // Jugador tiene un salto, fuerza a seleccionar otra ficha
											selecting = true;
										else
                                            // No hay saltos disponibles, el jugador esta listo para mover la pieza seleccionada
											selecting = false;
									}
								}
								else
                                    // Continua seleccionado (posible error o ficha no encontrada)
									selecting = true;
							}

                            // Turno del Jugador 2
							else
							{
								currentIndex = p2->findCheckerIndex(currentSquare); 
								if(currentIndex != -1 && currentIndex < p2->getCounter())
								{
									printChecker(p2->getChecker(currentIndex), "SELECTED");

                                    // Comprueba si la ficha actual va a saltar
									if(Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard)) 
									{
                                        // La ficha actual tiene un salto, la seleccion es valida
										selecting = false;
									}
									else
									{
										if(playerHasToJump(p2, p1))
                                            // Jugador tiene un salto, fuerza a seleccionar otra ficha
											selecting = true;
										else
                                            // No hay saltos disponibles, el jugador esta listo para mover la pieza seleccionada
											selecting = false;
									}
								}
								else
                                    // Continua seleccionado (ficha no encontrada)
									selecting = true;
							}
						}
                        // Continua seleccionado (selecciono una casilla vacia)
						else
							selecting = true;
/**************************************************** FIN DE SELECCION DEL JUGADOR *****************************************************************************/
					}
                    // Una ficha se va a mover o saltar
					else if(!selecting)
					{
						checkDoubleJump = false;

                        // Se analizan los movimientos de ambos jugadores para determinar si es un movimiento o un salto
						futureX = event.mouseButton.x;
						futureY = event.mouseButton.y;
						futureSquare = checkerboard->findSquare(futureX, futureY); // Casilla de llegada
						generalDirection = Moveable::findGeneralDirection(currentSquare, futureSquare); // Direccion General
						std::cout << "Moving in the general Direction of: " << generalDirection << std::endl;
						if(generalDirection == -1)
						{
							std::cout << "Moving nowhere (no turn change). Picking another checker to move." << std::endl;
							tempSquare = nullptr;
						}
						else if(futureSquare != nullptr)
						{
                            // Casilla de llegada esta ocupada, entonces estariamos haciendo un salto
							if(futureSquare->getOccupied())
								tempSquare = checkerboard->findJumpOntoSquare(futureSquare, generalDirection);

                            // Casilla de llegada vacia, puede estar seleccionando la casilla de llegada de un salto o podria ser un movimiento simple
							else
								tempSquare = checkerboard->findIntermSquare(futureSquare, generalDirection);
						}
						else
							std::cout << "The Future Square was not found." << std::endl;

/************************************************************* MOVIMIENTO DE AMBOS JUGADORES *******************************************************/

                        // Para que Casilla temporal sea nula es que la Direccion General sea -1 o que no se haya encontrado una Casilla de llegada
						if(tempSquare != nullptr)
						{
                            // Compueba que la casilla de llegada sea valida
							if(futureSquare->getFillColor() == sf::Color::Black)
							{
                                //Turno del Jugador 1
								if(p1->getTurn())
								{
									if(p1->getIsHuman())
									{
                                        //Comprueba si el jugador esta seleccionando un salto (al seleccionar una ficha)
										if(Moveable::jumpByChecker(p1->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex))
										{
											// Actualiza casillas
											currentSquare->setOccupied(false);
											futureSquare->setOccupied(false); 
											tempSquare->setOccupied(true);

											// Actualiza la posicion de la ficha que salta
											p1->getChecker(currentIndex)->setPosition(tempSquare->getPosition().x, tempSquare->getPosition().y);

											// Comprueba si la ficha que salta se corona
											ifCheckerKinged(p1->getChecker(currentIndex), tempSquare);
											printChecker(p1->getChecker(currentIndex), "JUMPING");

											// Elimina la ficha saltada
											deleteCheckerFromGame(p2, p2->findCheckerIndex(futureSquare));

                                            // Comprueba si hay un doble salto disponible
											checkDoubleJump = true;
											changeTurn();
										}

                                        //Comprueba si el jugador esta seleccionando un salto (al seleccionar una casilla)
										else if(Moveable::jumpBySquare(p1->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex)) 
										{
                                            // Actualiza casillas
											futureSquare->setOccupied(true);
											currentSquare->setOccupied(false);
											tempSquare->setOccupied(false); // La ficha saltada

											// Actualiza la posicion de la ficha que salta
											p1->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y);

                                            // Comprueba si la ficha que salta se corona
											ifCheckerKinged(p1->getChecker(currentIndex), futureSquare);
											printChecker(p1->getChecker(currentIndex), "JUMPING");

                                            // Elimina la ficha saltada
											deleteCheckerFromGame(p2, p2->findCheckerIndex(tempSquare));

                                            // Comprueba si hay un doble salto disponible
											checkDoubleJump = true; // we just completed a jump, let's see if there's a double jump available
											changeTurn();
										}

                                        //Comprueba si el jugador esta seleccionando un movimiento
										else if(Moveable::moveable(p1->getCheckersVector(), currentSquare, futureSquare, currentIndex) && !futureSquare->getOccupied()) 
										{

                                            // Se verifica que no se este evitando un salto (cuando se eata haciendo un doble salto, ya que la seleccion no se resetea en dobles saltos)
											if(!Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard))
											{
                                                // Actualiza casillas
												futureSquare->setOccupied(true);
												currentSquare->setOccupied(false);
												p1->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y);
												// check if the checker moved onto a king row
												ifCheckerKinged(p1->getChecker(currentIndex), futureSquare);
												printChecker(p1->getChecker(currentIndex), "MOVING");
												checkDoubleJump = false;
												changeTurn();
											}
										}

                                        // Verificamos si la ficha actual puede hacer un doble salto
										if(checkDoubleJump && Moveable::hasJump(p1->getChecker(currentIndex), p2->getCheckersVector(), checkerboard))
										{

                                            // Doble salto disponible, actualizamos casillas
											currentX = (int) p1->getChecker(currentIndex)->getPosition().x;
											currentY = (int) p1->getChecker(currentIndex)->getPosition().y;
											currentSquare = checkerboard->findSquare(currentX, currentY);
											selecting = false; // La seleccion no cambio pero para evitar algun error

                                            // Cambiamos el turno nuevamente para cancelar el cambio en el primer salto
											changeTurn();
											checkDoubleJump = false; // Al estar realizando un doble salto, se resetea la validacion a falso
											tempSquare = nullptr; // Se la misma manera se resetea la casilla temporal
										}
										else
											selecting = true; // No hubo doble salto, regresa a seleccionar
									}
								}

                                //Turno del Jugador 1
								else
								{
									if(p2->getIsHuman())
									{
                                        // Se realizan las mismas validaciones y comprobaciones
										if(Moveable::jumpByChecker(p2->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex))
										{
											currentSquare->setOccupied(false);
											futureSquare->setOccupied(false); 
											tempSquare->setOccupied(true);
											p2->getChecker(currentIndex)->setPosition(tempSquare->getPosition().x, tempSquare->getPosition().y); 
											ifCheckerKinged(p2->getChecker(currentIndex), tempSquare);
											printChecker(p2->getChecker(currentIndex), "JUMPING");
											deleteCheckerFromGame(p1, p1->findCheckerIndex(futureSquare));
											checkDoubleJump = true;
											changeTurn();
										}	
										else if(Moveable::jumpBySquare(p2->getCheckersVector(), currentSquare, futureSquare, tempSquare, currentIndex)) 
										{
											futureSquare->setOccupied(true);
											currentSquare->setOccupied(false);
											tempSquare->setOccupied(false); 
											p2->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y); 
											ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
											printChecker(p2->getChecker(currentIndex), "JUMPING");
											deleteCheckerFromGame(p1, p1->findCheckerIndex(tempSquare));
											checkDoubleJump = true;
											changeTurn();
										}
										else if(Moveable::moveable(p2->getCheckersVector(), currentSquare, futureSquare, currentIndex) && !futureSquare->getOccupied()) 
										{
											if(!Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard))
											{
												futureSquare->setOccupied(true);
												currentSquare->setOccupied(false);
												p2->getChecker(currentIndex)->setPosition(futureSquare->getPosition().x, futureSquare->getPosition().y);
												ifCheckerKinged(p2->getChecker(currentIndex), futureSquare);
												printChecker(p2->getChecker(currentIndex), "MOVING");
												checkDoubleJump = false;
												changeTurn();
											}
										}

										if(checkDoubleJump && Moveable::hasJump(p2->getChecker(currentIndex), p1->getCheckersVector(), checkerboard))
										{
											currentX = (int) p2->getChecker(currentIndex)->getPosition().x;
											currentY = (int) p2->getChecker(currentIndex)->getPosition().y;
											currentSquare = checkerboard->findSquare(currentX, currentY);
											selecting = false;
											changeTurn(); 
											checkDoubleJump = false;
											tempSquare = nullptr;
										}
										else
											selecting = true;
									}
								}
/************************************************* FIN DE VALIDACIONES DE MOVIMIENTO DE AMBOS JUGADORES *******************************************************/
							}
						}

                        // Esto es quivalente al cambiar de opinion al seleccionar una ficha para mover
						else
						{
                            // No cambia el turno pero valida al jugador actual, si no puede realizar un movimiento entonces ese jugador pierde
							std::cout << "Changing current checker. Code executed @ line " << __LINE__ << std::endl;
							selecting = true;
						}
					}
				}
			}

            // Guarda las coordenadas actuales del mouse para resaltar
			else if (event.type == sf::Event::MouseMoved)
			{
				mouseOverX = event.mouseMove.x;
				mouseOverY = event.mouseMove.y;
			}
			else if(event.type == sf::Event::Resized)
			{
				std::cout << std::endl << "new window x: " << window.getSize().x << std::endl;
				std::cout << "new window y: " << window.getSize().y << std::endl << std::endl;
			}
		} // end of window events loop

		activePlayerIsHuman = isActivePlayerHuman(); // determine if the active player is human

		// Secuencia de la creacion de la ventana SFML
		window.clear();
		checkerboard->drawGrid(window, mouseOverX, mouseOverY, currentX, currentY, selecting);
		cpDrawer.drawCheckers(window, p1->getCheckersVector());
		cpDrawer.drawCheckers(window, p2->getCheckersVector());
		window.display();
	}
}

// Cambia el turno de los jugadores
void CheckerGame::changeTurn()
{
	p1->setTurn(!p1->getTurn());
	p2->setTurn(!p2->getTurn());
}

// Crea los equipos
void CheckerGame::createTeams()
{
	int startingX = 0;
	int startingY = 0;
	int radius = 0;
	int idNumber = 0;

	for(int i = 0; i < SQUARES_VERTICAL; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{
            // Comprueba si la casilla actual esta ocupada
			if(checkerboard->findSquare(idNumber++)->getOccupied())
			{ 
				radius = SQUARES_VERTICAL / (2 * SQUARES_HORIZONTAL);
				if(i <= 2)
					p1->addChecker(new Checkerpiece(sf::Color::Red, radius, startingX, startingY, KING_ROW_7, i, p1->getNumber()));
				else
					p2->addChecker(new Checkerpiece(sf::Color::Blue, radius, startingX, startingY, KING_ROW_0, i, p2->getNumber()));
			}
			startingX += XOFFSET;
		}
		startingY += YOFFSET;
		startingX = 0;
	}
}

// Comprueba si el jugador tiene otros saltos
bool CheckerGame::playerHasToJump(Player*& active, Player*& notActive)
{
    // Itera cada ficha en el vector del jugador actual
	for(int i = 0; i < active->getCounter(); ++i)
		if(Moveable::hasJump(active->getChecker(i), notActive->getCheckersVector(), checkerboard))
			return true; // Jugador tiene un salto disponible
	return false;
}

// Determina si el jugador actual es humano
bool CheckerGame::isActivePlayerHuman()
{
	if(p1->getTurn() && p1->getIsHuman())
		return true;
	else if(p1->getTurn() && !p1->getIsHuman())
		return false;
	else if(p2->getTurn() && p2->getIsHuman())
		return true;
	else if(p2->getTurn() && !p2->getIsHuman())
		return false;
	else
		return false;
}

// Printea los atributos de la ficha seleccionada
void CheckerGame::printChecker(Checkerpiece* selected, const std::string& action)
{
	std::cout << action << " Checker (x, y): (" << selected->getPosition().x << ", " 
		<< selected->getPosition().y << ")" << " King: " 
			<< selected->getKing() << std::endl; 
}

// Elimina ficha emeniga
void CheckerGame::deleteCheckerFromGame(Player* deleteFrom, const int& deleteIndex)
{
    // Elimina una ficha que fue saltada
	if(deleteIndex != -1 && deleteIndex < deleteFrom->getCounter())
	{
		std::cout << "DELETING Checker (x, y): (" << deleteFrom->getChecker(deleteIndex)->getPosition().x << ", " 
			<< deleteFrom->getChecker(deleteIndex)->getPosition().y 
				<< ")" << " King: " << deleteFrom->getChecker(deleteIndex)->getKing() << std::endl;
		deleteFrom->deleteChecker(deleteIndex);
	}
}

// Comprueba si la ficha actual se corono
void CheckerGame::ifCheckerKinged(Checkerpiece* checker, Square* square)
{
	if((square->getRow() == KING_ROW_0 && checker->getKingRow() == KING_ROW_0) 
		|| (square->getRow() == KING_ROW_7 && checker->getKingRow() == KING_ROW_7))
	{
		checker->setKing(true); // Actualiza la ficha a ser rey
	}
}


// Muestra al ganador
void CheckerGame::showWinner(sf::RenderWindow& window, sf::Event& event)
{
	ostringstream ostr; // Crea un ostream
	ostr << "The\nwinner\nis\nPlayer " << winner << "\n"; // Ostr obtiene el texto
	const string winnerText = ostr.str(); // Convierte ostr a string y lo guarda

	// Carga la fuente
	sf::Font font;
	if(!font.loadFromFile("resources/Monterey.ttf"))
		cerr << "ERROR - cannot load resources" << endl;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(WINDOW_WIDTH / 8);
	text.setPosition(0, 0); // Dibuja abajo del titulo
	text.setColor(sf::Color::Black);
	text.setString(winnerText);

}