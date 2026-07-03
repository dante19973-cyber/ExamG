#include <SFML/Graphics.hpp>
#include "Application.h"
int main()
{
	//Application app;
	//app.Run();
 //   return 0;
 //   sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Gwent Clone");
 //   while (window.isOpen())
 //   {
 //       while (const std::optional event = window.pollEvent())
 //       {
 //           if (event->is<sf::Event::Closed>())
 //               window.close();
 //       }
 //       window.clear(sf::Color(30, 30, 30));
 //       window.display();
 //   }
	Application app;
	app.Run();
	return 0;

}