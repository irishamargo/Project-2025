#include <SFML/Graphics.hpp>
#include <iostream>

namespace {
    int kSizeCage = 50;
}

class Cage {
    public:
      sf::RectangleShape cageI;
      int status;

      Cage(int x, int y) {
        cageI.setSize(sf::Vector2f(kSizeCage, kSizeCage));
        cageI.setFillColor(sf::Color{255, 255, 255, 150});
        cageI.setPosition(x, y);
        status = 0;
      }

};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg")) {
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);


    sf::RectangleShape button_close(sf::Vector2f(200, 50));
    button_close.setPosition(1400, 900);
    button_close.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape button_check(sf::Vector2f(200, 50));
    button_check.setPosition(1400, 800);
    button_check.setFillColor(sf::Color{255, 191, 223, 200});

    Cage cage1(100, 100);
    Cage cage2(200, 200);

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

                    if (cage1.cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && cage1.status == 0) {
                        cage1.cageI.setFillColor(sf::Color{0, 200, 0, 200});
                        cage1.status = 1;
                    } else if (cage1.cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && cage1.status == 1) {
                        cage1.cageI.setFillColor(sf::Color{255, 255, 255, 255});
                        cage1.status = 2;
                    } else if (cage1.cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && cage1.status == 2) {
                        cage1.cageI.setFillColor(sf::Color{255, 255, 255, 150});
                        cage1.status = 0;
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(cage1.cageI);
        window.draw(cage2.cageI);
        window.draw(button_close);
        window.draw(button_check);
        window.draw(buttonText_close);
        window.draw(buttonText_check);

        window.display();
    }

    return 0;
}
