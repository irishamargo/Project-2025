#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    setlocale(LC_ALL, "RUS");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg")) {
        return -1; // Ошибка загрузки текстуры
    }

    // Создание спрайта для фона
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Масштабирование спрайта под размер окна (опционально)
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setPosition(1400, 900);
    button.setFillColor(sf::Color::Green);
    sf::RectangleShape square(sf::Vector2f(1000, 1060));
    square.setFillColor(sf::Color::White);
    square.setPosition(5, 5);


    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        if (!font.loadFromFile("arial.ttf")) {
            return -1;
        }
    }

    sf::Text buttonText("Close", font, 24);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(1465, 910);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(square);
        window.draw(button);
        window.draw(buttonText);

        window.display();
    }

    return 0;
}
