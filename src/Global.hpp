
#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

const int MAX_CHECKERS_PER_TEAM = 12;
const int KING_ROW_0 = 0; // Se usan para determinar la fila donde la ficha se convertiria en rey
const int KING_ROW_7 = 7;

const int SQUARES_VERTICAL = 8;
const int SQUARES_HORIZONTAL = 8;
const int TOTAL_SQUARES = 64;


const int WINDOW_WIDTH = 600; // Ancho de la ventana
const int WINDOW_HEIGHT = 600; // Alto de la ventana
const int XOFFSET = (WINDOW_WIDTH / SQUARES_VERTICAL); // Se usa para medir el ancho de casillas y fichas
const int YOFFSET = (WINDOW_HEIGHT / SQUARES_HORIZONTAL); // Se usa para medir el alto de casillas y fichas
const int WINDOW_POSITION_X = 0; // La posicion x de la ventana cuando es creada
const int WINDOW_POSITION_Y = 0; // La posicion y de la ventana cuando es creada
const std::string WINDOW_TITLE = "Checkers";


#endif