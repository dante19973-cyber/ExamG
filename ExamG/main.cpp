#include <SFML/Graphics.hpp>
#include "include/Application.h"
int main()
{
	srand(static_cast<unsigned>(time(nullptr)));
	Application app;
	app.Run();
	return 0;

}