#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstring>

namespace
{
    int kSizeCage = 50;
    int kSizeMas = 15;
}

class Cage
{
public:
    sf::RectangleShape cageI;
    int status;

    Cage()
    {
        cageI.setSize(sf::Vector2f(kSizeCage, kSizeCage));
        cageI.setFillColor(sf::Color{255, 255, 255, 150});
        cageI.setPosition(0, 0);
        status = 0;
    }

    Cage(int x, int y)
    {
        cageI.setSize(sf::Vector2f(kSizeCage, kSizeCage));
        cageI.setFillColor(sf::Color{255, 255, 255, 150});
        cageI.setPosition(x, y);
        status = 0;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg"))
    {
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

    Cage mas[kSizeMas][kSizeMas];
    int x = 100;
    int y = 100;
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            mas[i][j].cageI.setPosition(x + (55 * i), y + (55 * j));
        }
    }

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            return -1;
        }
    }

    sf::Text buttonText_close("Close", font, 24);
    buttonText_close.setFillColor(sf::Color::Black);
    buttonText_close.setPosition(1465, 910);

    sf::Text buttonText_check("Check", font, 24);
    buttonText_check.setFillColor(sf::Color::Black);
    buttonText_check.setPosition(1465, 810);

    //считывание данных из файла
    std::ifstream Keys("puzzle.txt");
    if (!Keys) {
        std::cout << "Файл не найден" << std::endl;
        return -1;
    }

    int countColumns{};
    int countLines{};
    char size[kSizeMas];
    Keys.getline(size, kSizeMas);

    char buf[kSizeMas];
    Keys.getline(buf, kSizeMas);
    int len = strlen(buf);

    for (int i = len; i < kSizeMas; ++i) {
         buf[i] = ' ';
    }

    buf[kSizeMas - 1] = '\0';
    sf::Text buttonText_len(buf, font, 24);
    buttonText_len.setFillColor(sf::Color::Red);
    buttonText_len.setPosition(1465, 910);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button_close.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                    }

                    for (int i = 0; i < 15; ++i)
                    {
                        for (int j = 0; j < 15; ++j)
                        {

                            if (mas[i][j].cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && mas[i][j].status == 0)
                            {
                                mas[i][j].cageI.setFillColor(sf::Color{255, 153, 204, 225});
                                mas[i][j].status = 1;
                            }
                            else if (mas[i][j].cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && mas[i][j].status == 1)
                            {
                                mas[i][j].cageI.setFillColor(sf::Color{255, 255, 255, 225});
                                mas[i][j].status = 2;
                            }
                            else if (mas[i][j].cageI.getGlobalBounds().contains(mousePos.x, mousePos.y) && mas[i][j].status == 2)
                            {
                                mas[i][j].cageI.setFillColor(sf::Color{255, 255, 255, 150});
                                mas[i][j].status = 0;
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        for (int i = 0; i < 15; ++i)
        {
            for (int j = 0; j < 15; ++j)
                window.draw(mas[i][j].cageI);
        }
        window.draw(button_close);
        window.draw(button_check);
        window.draw(buttonText_close);
        window.draw(buttonText_check);
        window.draw(buttonText_len);

        window.display();
    }

    return 0;
}
