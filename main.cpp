#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    setlocale(LC_ALL, "RUS");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg")) {
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    sf::RectangleShape button_close(sf::Vector2f(200, 50));
    button_close.setPosition(1400, 900);
    button_close.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape button_check(sf::Vector2f(200, 50));
    button_check.setPosition(1400, 800);
    button_check.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape square(sf::Vector2f(1000, 1060));
    square.setFillColor(sf::Color{255, 255, 255, 150});
    square.setPosition(5, 5);


    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        if (!font.loadFromFile("arial.ttf")) {
            return -1;
        }
    }

    sf::Text buttonText_close("Close", font, 24);
    buttonText_close.setFillColor(sf::Color::Black);
    buttonText_close.setPosition(1465, 910);

    sf::Text buttonText_check("Check", font, 24);
    buttonText_check.setFillColor(sf::Color::Black);
    buttonText_check.setPosition(1465, 810);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button_close.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(square);
        window.draw(button_close);
        window.draw(button_check);
        window.draw(buttonText_close);
        window.draw(buttonText_check);

        window.display();
    }

    return 0;
}
