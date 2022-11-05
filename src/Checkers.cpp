/*
*	This is a checkers game programmed in C++ using SFML.
*	By Zach Mertens (https://github.com/zmertens, azzach19@yahoo.com)
*	Free software! GPL license.
*
*	This is the main program file for Checkers. It contains a command loop for the main menu.
*	The Main class handles menu selections such as 1) play, 2) view rules, 3) view best scores.
*	Menu options are selected by pressing the appropriate key on the keyboard.
*	There is a "game in progress" animation below the main menu options.
*	All graphics are drawn using SFML primitives (circles, rectangles, and basic polygons).
*/

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

/* Start the Checkers game by creating a window, menu options, and performing a graphical animation on the window */
void Checkers::start()
{
	// constant strings
	const string RESOURCE_ERROR_IMAGE = "ERROR - cannot open \"/resources/Checkerboard_8x8_125px.png\"";
	const string RESOURCE_ERROR_FONT = "ERROR - cannot open \"/resources/Monterey.ttf\"";
	const string TITLE = "_CHECKERS_";
	const string MAIN_MENU =
		"Press 'C' for 1 PLayer Game\n"
        "Press 'P' for 2 Players Game\n"
        "Press 'ESCAPE' to close the window or\n"
        "in game to return to menu";

	// setup the window (give dimensions, followed by a window title, and the default format)
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	// the creation coordinates of the window context on the monitor
	window.setPosition(sf::Vector2i(WINDOW_POSITION_X, WINDOW_POSITION_Y));
	
	// load an image to use as an icon on the titlebar
	sf::Image image;
	if(!image.loadFromFile("/home/greivin/CLionProjects/Inmersive_Checkers/resources/Checkerboard_8x8_125px.png"))
		cerr << RESOURCE_ERROR_IMAGE << endl;
	// Vector out of range error
	//window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	
	// load the text font
	sf::Font font;
	if(!font.loadFromFile("/home/greivin/CLionProjects/Inmersive_Checkers/resources/Monterey.ttf"))
		cerr << RESOURCE_ERROR_FONT << endl;
	
	// initialize menu fonts
	sf::Text menuMessage, menuTitle;
	menuTitle.setFont(font);
	menuTitle.setCharacterSize(WINDOW_WIDTH / 25);
	menuTitle.setPosition(0, 0); // draw @ top-left corner of window
	menuTitle.setColor(sf::Color::Red);
	menuTitle.setString(TITLE);
	menuMessage.setFont(font);
	menuMessage.setCharacterSize(WINDOW_WIDTH / 25);
	menuMessage.setPosition(0, WINDOW_HEIGHT / 25); // draw slightly below the title
	menuMessage.setColor(sf::Color::Black);
	menuMessage.setString(MAIN_MENU);
	
	int mouseOverX = 0, mouseOverY = 0; // used to gather information about user's mouse coords on the window
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if((event.type == sf::Event::Closed) 
				|| ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) // close SFML window
			{
				window.close();
			}
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) // human verses human
            {
                CheckerGame checkerGame(window, true, true); // both players are human
                checkerGame.startCheckers(window, event);
            }
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C)) // human verses computer
			{
				CheckerGame checkerGame(window, true, false); // only player 1 is human
				checkerGame.startCheckers(window, event);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				// saves these coordinates and draws a nice green or purple box around the closest moused over square on the checkerboard.
				mouseOverX = event.mouseMove.x;
				mouseOverY = event.mouseMove.y;
			}
		}

		// SFML window drawing sequence
		window.clear(sf::Color::White);
		window.draw(menuTitle);
		window.draw(menuMessage);
		animation(window, mouseOverX, mouseOverY);
		window.display();
	} // end of window.isOpen()
}

/* create a fun little animation on the main menu (sort of like showing a game in progress) */
void Checkers::animation(sf::RenderWindow& window, const int& mouseOverX, const int& mouseOverY)
{ 
	/* The animation is of a mock-checker game. It takes up roughly 3/4 the window */
	// reset startingX, startingY, and k
	int startingX = 0, startingY = WINDOW_HEIGHT / 4;
	sf::CircleShape circle (WINDOW_WIDTH / (2 * SQUARES_HORIZONTAL));
	circle.setPosition(WINDOW_WIDTH, 0); // get rid of the 0,0 default constructed circle
	sf::RectangleShape square (sf::Vector2f(static_cast<float>(XOFFSET), static_cast<float>(YOFFSET)));
	for(int i = 0; i < SQUARES_VERTICAL - 2; ++i)
	{	
		for(int j = 0; j < SQUARES_HORIZONTAL; ++j)
		{	
			// position the square
			square.setPosition(static_cast<float>(startingX), static_cast<float>(startingY));
			sf::RectangleShape temp (sf::Vector2f(static_cast<float>(XOFFSET / 3), static_cast<float>(YOFFSET / 3)));
			temp.setPosition(static_cast<float>(mouseOverX), static_cast<float>(mouseOverY));
			if(square.getGlobalBounds().intersects(temp.getGlobalBounds()))
			{
				// give the move to square a nice green highlight
				square.setOutlineThickness(-(SQUARES_VERTICAL / 2));
				square.setOutlineColor(sf::Color::Green);	
			}
			// create some random checkerpieces
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
			// this crazy looking if statement gives the checkered B/W pattern
			if((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
				square.setFillColor(sf::Color::White); 
			else
				square.setFillColor(sf::Color::Black);
			window.draw(square);
			window.draw(circle);
			startingX += XOFFSET;
			// erase any previous color highlights from the
			square.setOutlineThickness(0);
			square.setOutlineColor(sf::Color(0, 0, 0, 255));
		}
		startingY += YOFFSET;
		startingX = 0;
	}
}