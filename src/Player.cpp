
#include "Player.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::string;

Player::Player(const int number, const bool isHuman)
	: isHuman(isHuman)
	, number(number)
{
	vector<Checkerpiece*> checkers;
	checkers.reserve(MAX_CHECKERS_PER_TEAM); // Reserva espacio para 12 fichas(maxima cantidad de fichas por jugador)
}

// Borra el vector de fichas del jugador
Player::~Player()
{
	checkers.erase(checkers.begin(), checkers.end());
}

// Muestra las fichas del jugador
void Player::displayCheckers() 
{
	cout << endl <<"Display Player# " << this->getNumber() << "'s active checkers" << endl;
	vector<Checkerpiece*>::const_iterator it;
	for(it = this->checkers.begin(); it != this->checkers.end(); ++it)
		cout << "Position(x,y): (" << (*it)->getPosition().x << ", " << (*it)->getPosition().y << ") " << 
			"King Row: " << (*it)->getKingRow() << " King: " << (*it)->getKing() << endl;
}

// Agrega fichas al vector de fichas del jugador
void Player::addChecker(Checkerpiece* checkerpiece)
{
	checkers.push_back(checkerpiece);
}

// Encuentra el indice de una ficha en el vector de fichas del jugador
int Player::findCheckerIndex(const int& x, const int& y)
{
	for(unsigned int i = 0; i != this->checkers.size(); ++i)
		if(checkers[i]->getPosition().x == x && checkers[i]->getPosition().y == y)
			return i;
	return -1; // Si la ficha no se encontro
}

// Encuentra una ficha en el vector de fichas del jugador al comparar la posicion de una casilla y la posicion de una ficha
int Player::findCheckerIndex(const Square* tempSquare)
{
	for(unsigned int i = 0; i < checkers.size(); ++i)
		if((checkers[i]->getPosition().x == tempSquare->getPosition().x) && (checkers[i]->getPosition().y == tempSquare->getPosition().y))
			return i;
	return -1; // Si la ficha no se encontro
}

// Borra una ficha
void Player::deleteChecker(const int& deleteIndex)
{
	this->checkers.erase(this->checkers.begin() + deleteIndex);
}

// Borra todas las fichas
void Player::deleteAllCheckers()
{
	this->checkers.erase(this->checkers.begin(), this->checkers.end());
}

vector<Checkerpiece*>& Player::getCheckersVector()
{
	return checkers;
}

// Regresa la ficha en el indice dado
Checkerpiece* Player::getChecker(const int& index)
{
	return this->checkers.at(index);
}

bool Player::getTurn()
{
	return turn;
}

int Player::getCounter()
{
    return checkers.size();
}

const bool Player::getIsHuman()
{
	return isHuman;
}

const int Player::getNumber()
{
	return number;
}

void Player::setTurn(const bool& turn)
{
	this->turn = turn;
}