#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Crossword");

    sf::RectangleShape square(sf::Vector2f(1000, 1060));
    square.setFillColor(sf::Color::White);
    square.setPosition(5, 5);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(square);
        window.display();
    }

    return 0;
}
