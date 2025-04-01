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

// Создание сетки клеток
void CreateGrid(Cell grid[kGridSize][kGridSize])
{
    for (int i = 0; i < kGridSize; ++i)
    {
        for (int j = 0; j < kGridSize; ++j)
        {
            grid[i][j].shape.setPosition(100 + (55 * i), 100 + (55 * j));
        }
    }
}

// Обработка нажатия на клетки
void HandleCellClick(Cell grid[kGridSize][kGridSize], sf::Vector2i mousePos)
{
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

//Обработка нажатия на кнопку проверки
bool Check(char* name, Cell grid[kGridSize][kGridSize]) {
    std::ifstream puzzleAnswerFile(name);
    if (!puzzleAnswerFile)
    {
        std::cerr << "File not found!" << std::endl;
        return -1;
    }

    for (int i = 0; i < kGridSize; ++i) {
        char buf[kGridSize+1];
        puzzleAnswerFile >> buf;
        std::cout << buf << '\n';
        for (int j = 0; j < kGridSize; ++j) {
            if (grid[i][j].status == 1 && buf[j] == '0') {
                return false;
            } else if ((grid[i][j].status == 0 || grid[i][j].status == 2) && buf[j] == '1') {
                return false;
            }
        }
    }
    return true;
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
    checkButton.setPosition(1600, 700);
    checkButton.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape chooseButton(sf::Vector2f(200, 50));
    chooseButton.setPosition(1300, 700);
    chooseButton.setFillColor(sf::Color{255, 191, 223, 200});

    sf::RectangleShape optionsButton[kMaxPuzzles];

    // Создание сетки клеток
    Cell grid[kGridSize][kGridSize];
    CreateGrid(grid);

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
    checkButtonText.setPosition(1665, 710);

    sf::Text chooseButtonText("Choose Task", font, 24);
    chooseButtonText.setFillColor(sf::Color::Black);
    chooseButtonText.setPosition(1325, 710);

    sf::Text optionsText[kMaxPuzzles];

    // Создание кнопок для выплывающего меню
    const char *choosePuzzles[kMaxPuzzles] = {"Puzzle 1", "Puzzle 2", "Puzzle 3"};

    for (int i = 0; i < kMaxPuzzles; ++i)
    {
        optionsButton[i].setSize(sf::Vector2f(200, 50));
        optionsButton[i].setPosition(1300, 700 + 50 + i * 60 + 30);
        optionsButton[i].setFillColor(sf::Color{255, 191, 223, 200});
        optionsText[i] = sf::Text(choosePuzzles[i], font, 20);
        optionsText[i].setFillColor(sf::Color::Black);
        optionsText[i].setPosition(1310, 700 + 50 + i * 60 + 10 + 30);
    }

    bool menuOpen = false;

    // Создание ключей
    std::ifstream puzzleFile("puzzle.txt");
    if (!puzzleFile)
    {
        std::cerr << "File not found!" << std::endl;
        return -1;
    }

    // Для строк
    int countColumns{};
    int countLines = 5;
    char sizeBuffer[kGridSize];
    puzzleFile.getline(sizeBuffer, kGridSize);

    Number lineNumbers[kGridSize][kGridSize];
    int numberX = 80;
    int numberY = 110;
    for (int i = 0; i < kGridSize; ++i)
    {
        char buffer[kGridSize];
        puzzleFile.getline(buffer, kGridSize);

        char str[kGridSize];
        int j = 0;
        int k = 0;
        int p = 0;
        while (buffer[j] != '\0')
        {
            if (buffer[j] != ' ')
            {
                str[k] = buffer[j];
                ++k;
            }
            else
            {
                lineNumbers[i][p].text.setFont(font);
                lineNumbers[i][p].text.setString(str);
                lineNumbers[i][p].text.setPosition(numberX - (p * 35), numberY);
                for (int s = 0; s < kGridSize; ++s)
                {
                    str[s] = ' ';
                }
                k = 0;
                ++p;
            }
            ++j;
        }
        lineNumbers[i][p].text.setFont(font);
        lineNumbers[i][p].text.setString(str);
        lineNumbers[i][p].text.setPosition(numberX - (p * 35), numberY);
        numberY += 55;
    }

    // Для столбцов
    Number ColumnNumbers[kGridSize][kGridSize];
    numberX = 120;
    numberY = 65;
    for (int i = 0; i < kGridSize; ++i)
    {
        char buffer[kGridSize];
        puzzleFile.getline(buffer, kGridSize);

        char str[kGridSize];
        int j = 0;
        int k = 0;
        int p = 0;
        while (buffer[j] != '\0')
        {
            if (buffer[j] != ' ')
            {
                str[k] = buffer[j];
                ++k;
            }
            else
            {
                ColumnNumbers[i][p].text.setFont(font);
                ColumnNumbers[i][p].text.setString(str);
                ColumnNumbers[i][p].text.setPosition(numberX, numberY - (p * 30));
                for (int s = 0; s < kGridSize; ++s)
                {
                    str[s] = ' ';
                }
                k = 0;
                ++p;
            }
            ++j;
        }
        ColumnNumbers[i][p].text.setFont(font);
        ColumnNumbers[i][p].text.setString(str);
        ColumnNumbers[i][p].text.setPosition(numberX, numberY - (p * 30));
        numberX += 55;
    }

    // Линии разделения сетки
    sf::RectangleShape line1(sf::Vector2f(830, 5));
    line1.setPosition(95, 95);
    line1.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line2(sf::Vector2f(820, 5));
    line2.setPosition(100, 370);
    line2.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line3(sf::Vector2f(820, 5));
    line3.setPosition(100, 645);
    line3.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line4(sf::Vector2f(830, 5));
    line4.setPosition(95, 920);
    line4.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line5(sf::Vector2f(5, 820));
    line5.setPosition(95, 100);
    line5.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line6(sf::Vector2f(5, 820));
    line6.setPosition(370, 100);
    line6.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line7(sf::Vector2f(5, 820));
    line7.setPosition(645, 100);
    line7.setFillColor(sf::Color{255, 191, 223, 255});

    sf::RectangleShape line8(sf::Vector2f(5, 820));
    line8.setPosition(920, 100);
    line8.setFillColor(sf::Color{255, 191, 223, 255});


    sf::Text Answer("None", font, 24);
    Answer.setFillColor(sf::Color::White);
    Answer.setPosition(20, 20);

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

                    if (closeButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                    }

                    if (checkButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        bool result = Check("puzzleAnswer.txt", grid);
                        if (result) {
                            Answer.setString("True");
                        } else {
                            Answer.setString("False");
                        }


                    }

                    if (chooseButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuOpen = !menuOpen;
                    }

                    // Обработка выбора опции в выпадающем меню
                    if (menuOpen)
                    {
                        for (int i = 0; i < kMaxPuzzles; ++i)
                        {
                            if (optionsButton[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                menuOpen = false;
                                chooseButtonText.setString(optionsText[i].getString());
                            }
                        }
                    }

                    // Обработка нажатия на клетки
                    HandleCellClick(grid, mousePos);
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
        window.draw(chooseButton);
        window.draw(closeButtonText);
        window.draw(checkButtonText);
        window.draw(chooseButtonText);

        if (menuOpen)
        {
            for (int i = 0; i < kMaxPuzzles; ++i)
            {
                window.draw(optionsButton[i]);
                window.draw(optionsText[i]);
            }
        }

        for (int i = 0; i < kGridSize; ++i)
        {
            for (int j = 0; j < kGridSize; ++j)
            {
                window.draw(lineNumbers[i][j].text);
            }
        }

        for (int i = 0; i < kGridSize; ++i)
        {
            for (int j = 0; j < kGridSize; ++j)
            {
                window.draw(ColumnNumbers[i][j].text);
            }
        }

        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);
        window.draw(line5);
        window.draw(line6);
        window.draw(line7);
        window.draw(line8);

        window.draw(Answer);

        window.display();
    }

    return 0;
}
