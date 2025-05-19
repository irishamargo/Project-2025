#include "Crossword.h"
#include "workingKeys.hpp"

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

// Создание сетки клеток
void CreateGrid(Cell** grid, int gridSize)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            grid[i][j].shape.setPosition(100 + (55 * j), 100 + (55 * i));
        }
    }
}

// Обработка нажатия на клетки
void HandleCellClick(Cell** grid, sf::Vector2i mousePos, int gridSize)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
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
bool Check(char* name, Cell** grid, int gridSize) {
    std::ifstream puzzleAnswerFile(name);
    if (!puzzleAnswerFile)
    {
        std::cerr << "File not found!" << std::endl;
        return false;
    }

    for (int i = 0; i < gridSize; ++i) {
        char buf[gridSize+1];
        puzzleAnswerFile >> buf;
        for (int j = 0; j < gridSize; ++j) {
            if (grid[i][j].status == 1 && buf[j] != '1') {
                return false;
            } else if ((grid[i][j].status == 0 || grid[i][j].status == 2) && buf[j] != '0') {
                return false;
            }
        }
    }
    return true;
}

// Добавляем новые функции в конец файла
void ResetGrid(Cell** grid, int gridSize) {
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            grid[i][j].status = 0;
            grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 150});
        }
    }
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
    int gridSize = kGridSize;
    Cell** grid = new Cell*[gridSize];
    for (int i = 0; i < gridSize; ++i) {
        grid[i] = new Cell[gridSize];
    }
    CreateGrid(grid, gridSize);

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

    // Для строк
    Number** lineNumbers = new Number*[gridSize];
    for (int i = 0; i < gridSize; ++i) {
        lineNumbers[i] = new Number[gridSize];
    }

    // Для столбцов
    Number** ColumnNumbers = new Number*[gridSize];
    for (int i = 0; i < gridSize; ++i) {
        ColumnNumbers[i] = new Number[gridSize];
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

    int currentPuzzle = 0;

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
                        bool result = Check("puzzleAnswer.txt", grid, gridSize);
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

                                currentPuzzle = i; // Сохраняем выбор

                                // Очищаем предыдущие данные
                                CleanupNumberArray(lineNumbers, gridSize);
                                CleanupNumberArray(ColumnNumbers, gridSize);
                                ResetGrid(grid, gridSize);
                                lineNumbers = new Number*[gridSize];
                                ColumnNumbers = new Number*[gridSize];
                                for (int i = 0; i < gridSize; ++i) {
                                    lineNumbers[i] = new Number[gridSize];
                                    ColumnNumbers[i] = new Number[gridSize];
                                }

                                // Вызываем соответствующую функцию загрузки
                                switch(currentPuzzle) {
                                    case 0:
                                        processKeys(lineNumbers, ColumnNumbers, "puzzle1.txt", gridSize, font);
                                        break;
                                    case 1:
                                        processKeys(lineNumbers, ColumnNumbers, "puzzle2.txt", gridSize, font);
                                        break;
                                    case 2:
                                        processKeys(lineNumbers, ColumnNumbers, "puzzle3.txt", gridSize, font);
                                        break;
                                }
                            }
                        }
                    }

                    // Обработка нажатия на клетки
                    HandleCellClick(grid, mousePos, gridSize);
                }
            }
        }

        // Отрисовка
        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
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

        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(lineNumbers[i][j].text);
            }
        }

        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
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

    CleanupNumberArray(lineNumbers, gridSize);
    CleanupNumberArray(ColumnNumbers, gridSize);

    for (int i = 0; i < gridSize; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
