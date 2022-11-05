
/*
    La clase CheckerGame representa el estado del juego. se encarga del loop principal del juego, manejo de las piezas
    y operaciones que realiza el AI para realizar dichas modificaciones.
*/

#ifndef CHECKERGAME_HPP
#define CHECKERGAME_HPP

#include <SFML/Graphics.hpp> // Permite dibujar en la ventana
#include <SFML/Window.hpp> // Permite instancias de la ventana
#include <SFML/System.hpp> // Necesaria para las instancias de la ventana
#include <iostream>
#include <string>
#include <fstream> // Para archivos IO
#include <ios> // Para agregar a archivos guardados
#include <sstream> // Para cadenas de strings
#include <string> // Para strings
#include <vector> // Para vectores
#include "AI.hpp"

class CheckerGame
{
private:
	bool isPlaying;
	static int winner;
	Checkerboard* checkerboard;
	Checkerpiece cpDrawer;
	Player* p1;
	Player* p2;

	bool playerHasToJump(Player*&, Player*&);
	void gameLoop(sf::RenderWindow&, sf::Event&);
	void showWinner(sf::RenderWindow&, sf::Event&);
	void createTeams();
	void changeTurn();
	bool isActivePlayerHuman();
	void printChecker(Checkerpiece*, const std::string&);
	void deleteCheckerFromGame(Player*, const int&); 
	void ifCheckerKinged(Checkerpiece*, Square*);
public:
	CheckerGame(sf::RenderWindow&, const bool&, const bool&);
	~CheckerGame();
	void startCheckers(sf::RenderWindow&, sf::Event&);
};

#endif