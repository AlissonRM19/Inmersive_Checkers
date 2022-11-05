
/*
    La clase Checkers representa el manejo de la aplicacion mediante el manejo de una ventana con un menu
    y una simulacion del juego. Los graficos son creados usando SFML.
*/
#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include <SFML/Graphics.hpp> // Permite dibujar en la ventana
#include <SFML/Window.hpp> // Permite instancias de la ventana
#include <SFML/System.hpp> // Necesaria para las instancias de la ventana
#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "CheckerGame.hpp"

class Checkers
{
private:
	void animation(sf::RenderWindow&, const int&, const int&);
public:
	Checkers();
        void start(); // Inicia la ventana del menu
};

#endif 