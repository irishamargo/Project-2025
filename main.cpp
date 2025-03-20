#include "Crossword.h"

// Реализация методов класса Cell
Cell::Cell()
{
    shape.setSize(sf::Vector2f(kCellSize, kCellSize));
    shape.setFillColor(sf::Color{255, 255, 255, 150});
    shape.setPosition(0, 0);
    status = 0;
}

Cell::Cell(int x, int y)
{
    shape.setSize(sf::Vector2f(kCellSize, kCellSize));
    shape.setFillColor(sf::Color{255, 255, 255, 150});
    shape.setPosition(x, y);
    status = 0;
}

// Реализация методов класса Number
Number::Number()
{
    text.setString(' ');
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(0, 0);
}

Number::Number(const char *buffer, const sf::Font &font, int x, int y)
{
    text.setString(buffer);
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");

    // Загрузка фонового изображения
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg"))
    {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Создание кнопок
    sf::RectangleShape closeButton(sf::Vector2f(200, 50));
    closeButton.setPosition(1600, 900);
    closeButton.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape checkButton(sf::Vector2f(200, 50));
    checkButton.setPosition(1300, 900);
    checkButton.setFillColor(sf::Color{255, 191, 223, 200});

    // Создание сетки клеток
    Cell grid[kGridSize][kGridSize];
    int startX = 100;
    int startY = 100;
    for (int i = 0; i < kGridSize; ++i)
    {
        for (int j = 0; j < kGridSize; ++j)
        {
            grid[i][j].shape.setPosition(startX + (55 * i), startY + (55 * j));
        }
    }

    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Failed to load font!" << std::endl;
            return -1;
        }
    }

    // Текст для кнопок
    sf::Text closeButtonText("Close", font, 24);
    closeButtonText.setFillColor(sf::Color::Black);
    closeButtonText.setPosition(1665, 910);

    sf::Text checkButtonText("Check", font, 24);
    checkButtonText.setFillColor(sf::Color::Black);
    checkButtonText.setPosition(1365, 910);

    // Загрузка данных из файла
    std::ifstream puzzleFile("puzzle.txt");
    if (!puzzleFile)
    {
        std::cerr << "File not found!" << std::endl;
        return -1;
    }

    int countColumns{};
    int countLines = 5;
    char sizeBuffer[kGridSize];
    puzzleFile.getline(sizeBuffer, kGridSize);

    Number lineNumbers[kGridSize];
    int numberX = 35;
    int numberY = 110;
    for (int i = 0; i < kGridSize; ++i)
    {
        char buffer[kGridSize];
        puzzleFile.getline(buffer, kGridSize);
        lineNumbers[i] = Number(buffer, font, numberX, numberY + (i * 55));
    }

    // Основной цикл программы
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

                    // Обработка нажатия на кнопку закрытия
                    if (closeButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                    }

                    // Обработка нажатия на клетки
                    for (int i = 0; i < kGridSize; ++i)
                    {
                        for (int j = 0; j < kGridSize; ++j)
                        {
                            if (grid[i][j].shape.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (grid[i][j].status == 0)
                                {
                                    grid[i][j].shape.setFillColor(sf::Color{255, 153, 204, 225});
                                    grid[i][j].status = 1;
                                }
                                else if (grid[i][j].status == 1)
                                {
                                    grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 225});
                                    grid[i][j].status = 2;
                                }
                                else if (grid[i][j].status == 2)
                                {
                                    grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 150});
                                    grid[i][j].status = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Отрисовка
        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < kGridSize; ++i)
        {
            for (int j = 0; j < kGridSize; ++j)
            {
                window.draw(grid[i][j].shape);
            }
        }

        window.draw(closeButton);
        window.draw(checkButton);
        window.draw(closeButtonText);
        window.draw(checkButtonText);

        for (int i = 0; i < kGridSize; ++i)
        {
            window.draw(lineNumbers[i].text);
        }

        window.display();
    }

    return 0;
}
