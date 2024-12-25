#include <SFML/Window.hpp>
 
int main()
{
  sf::Window window(sf::VideoMode(800, 600), "SFML works!");
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      /* code */
      if (event.type == sf::Event::Closed)
        window.close();
    }
  }
}