
#include "Checkers.hpp"
#include "CheckerGame.hpp"

using std::string;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::vector;
using std::sort;

Checkers::Checkers() {}

// Inicia el juego al crear una ventana con el menu y una simulacion del juego
void Checkers::start()
{
	// String de texto para el menu y errores
	const string RESOURCE_ERROR_IMAGE = "ERROR - cannot open \"/resources/Checkerboard_8x8_125px.png\"";
	const string RESOURCE_ERROR_FONT = "ERROR - cannot open \"/resources/Monterey.ttf\"";
	const string TITLE = "_CHECKERS_";
	const string MAIN_MENU =
		"Press 'C' for 1 PLayer Game\n"
        "Press 'P' for 2 Players Game\n"
        "Press 'ESCAPE' to close the window or\n"
        "in game to return to menu";

    // Creacion de la ventana
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default);
	window.setVerticalSyncEnabled(true);

    // Creacion inicial de la ventana con coordenadas 0s
	window.setPosition(sf::Vector2i(WINDOW_POSITION_X, WINDOW_POSITION_Y));

    // Carga la imagen para usar como icono
	sf::Image image;
	if(!image.loadFromFile("/home/greivin/CLionProjects/Inmersive_Checkers/resources/Checkerboard_8x8_125px.png"))
		cerr << RESOURCE_ERROR_IMAGE << endl;
	
	// Carga la fuente del texto
	sf::Font font;
	if(!font.loadFromFile("/home/greivin/CLionProjects/Inmersive_Checkers/resources/Monterey.ttf"))
		cerr << RESOURCE_ERROR_FONT << endl;
	
	// Inicializa la fuente del menu
	sf::Text menuMessage, menuTitle;
	menuTitle.setFont(font);
	menuTitle.setCharacterSize(WINDOW_WIDTH / 25);
	menuTitle.setPosition(0, 0); // Dibuja al inicio de la ventana
	menuTitle.setColor(sf::Color::Red);
	menuTitle.setString(TITLE);
	menuMessage.setFont(font);
	menuMessage.setCharacterSize(WINDOW_WIDTH / 25);
	menuMessage.setPosition(0, WINDOW_HEIGHT / 25); // Dibuja abajo del titulo
	menuMessage.setColor(sf::Color::Black);
	menuMessage.setString(MAIN_MENU);
	
	int mouseOverX = 0, mouseOverY = 0; // Se usa para obtener las coordenadas del mouse en la ventana
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if((event.type == sf::Event::Closed) 
				|| ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) // Cierra la ventana al presionar 'ESCAPE'
			{
				window.close();
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) // Inicia un juego para 2 (usuarios) al presionar 'P'
            {
                CheckerGame checkerGame(window, true, true); // Ambos jugadores son humanos
                checkerGame.startCheckers(window, event);
            }
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)) // Inicia un juego para 1 (vs AI) al presionar 'C'
			{
				CheckerGame checkerGame(window, true, false); // Solo 1 jugador es humano
				checkerGame.startCheckers(window, event);
			}
			else if (event.type == sf::Event::MouseMoved)
			{

                // Guarda las coordenadas y dibuja un cuadro verde o morado alrededor de la casilla mas cercana al mouse
				mouseOverX = event.mouseMove.x;
				mouseOverY = event.mouseMove.y;
			}
		}

        // Secuencia para crear la ventana SFML
		window.clear(sf::Color::White);
		window.draw(menuTitle);
		window.draw(menuMessage);
		animation(window, mouseOverX, mouseOverY);
		window.display();
	}
}

// Crea la simulacion del tablero en el menu
void Checkers::animation(sf::RenderWindow& window, const int& mouseOverX, const int& mouseOverY)
{ 

	int startingX = 0, startingY = WINDOW_HEIGHT / 4;
	sf::CircleShape circle (WINDOW_WIDTH / (2 * SQUARES_HORIZONTAL));
	circle.setPosition(WINDOW_WIDTH, 0);
	sf::RectangleShape square (sf::Vector2f(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET)));
	for(int i = 0; i < SQUARES_VERTICAL - 2; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{	
			// Coloca la casilla
			square.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			sf::RectangleShape temp (sf::Vector2f(static_cast<float>(XOFFSET / 3), static_cast<float>(YOFFSET / 3)));
			temp.setPosition(static_cast<float>(mouseOverX), static_cast<float>(mouseOverY));
			if(square.getGlobalBounds().intersects(temp.getGlobalBounds()))
			{

                // Resalta la casilla seleccionada de verde
				square.setOutlineThickness(-(SQUARES_VERTICAL / 2));
				square.setOutlineColor(sf::Color::Green);	
			}

			// Crea las fichas
			if(i <= 1 && !((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0)))
			{
				circle.setFillColor(sf::Color::Blue);
				circle.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			}
			else if(i >= 4 && !((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0)))
			{
				circle.setFillColor(sf::Color::Red);
				circle.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			}

            // Da color a las casillas
			if((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
				square.setFillColor(sf::Color::White); 
			else
				square.setFillColor(sf::Color::Black);
			window.draw(square);
			window.draw(circle);
			startingX += XOFFSET;

			// Borra cualquier borde creado anteriormente
			square.setOutlineThickness(0);
			square.setOutlineColor(sf::Color(0, 0, 0, 255));
		}
		startingY += YOFFSET;
		startingX = 0;
	}
}