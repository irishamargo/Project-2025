#include "Cell.h"
#include "GridManager.h"
#include "workingKeys.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

using namespace Constants;
namespace fs = std::filesystem;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Japanese Crossword");
    window.setFramerateLimit(60);

    // Загрузка фона
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fon2.jpg"))
    {
        std::cerr << "Failed to load background image!\n";
        return EXIT_FAILURE;
    }
    sf::Sprite background(backgroundTexture);

    // Инициализация шрифта
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Failed to load font!" << std::endl;
            return -1;
        }
    }

    const int gridSize = kGridSize;
    Cell **grid = new Cell *[gridSize];
    for (int i = 0; i < gridSize; ++i)
    {
        grid[i] = new Cell[gridSize];
    }
    GridUtils::CreateGrid(grid, gridSize);

    // Система подсказок
    Number **lineNumbers = new Number *[gridSize];
    Number **columnNumbers = new Number *[gridSize];
    for (int i = 0; i < gridSize; ++i)
    {
        lineNumbers[i] = new Number[gridSize];
        columnNumbers[i] = new Number[gridSize];
    }

    // Кнопки интерфейса
    const sf::Vector2f buttonSize(200, 50);
    const sf::Color buttonColor(255, 191, 223, 200);

    sf::RectangleShape closeButton(buttonSize);
    closeButton.setPosition(1600, 900);
    closeButton.setFillColor(buttonColor);

    sf::RectangleShape checkButton(buttonSize);
    checkButton.setPosition(1600, 700);
    checkButton.setFillColor(buttonColor);

    sf::RectangleShape chooseButton(buttonSize);
    chooseButton.setPosition(1300, 700);
    chooseButton.setFillColor(buttonColor);

    // Тексты кнопок
    sf::Text closeText("Close", font, 24);
    closeText.setPosition(1665, 910);
    closeText.setFillColor(sf::Color::Black);

    sf::Text checkText("Check", font, 24);
    checkText.setPosition(1665, 710);
    checkText.setFillColor(sf::Color::Black);

    sf::Text chooseText("Choose Puzzle", font, 24);
    chooseText.setPosition(1320, 710);
    chooseText.setFillColor(sf::Color::Black);

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

    // Меню выбора пазлов
    const int maxPuzzles = 3;
    sf::RectangleShape puzzleButtons[maxPuzzles];
    sf::Text puzzleTexts[maxPuzzles];
    for (int i = 0; i < maxPuzzles; ++i)
    {
        puzzleButtons[i].setSize(buttonSize);
        puzzleButtons[i].setPosition(1300, 750 + i * 70);
        puzzleButtons[i].setFillColor(buttonColor);

        puzzleTexts[i].setString("Puzzle " + std::to_string(i + 1));
        puzzleTexts[i].setFont(font);
        puzzleTexts[i].setCharacterSize(20);
        puzzleTexts[i].setPosition(1310, 760 + i * 70);
        puzzleTexts[i].setFillColor(sf::Color::Black);
    }

    // Состояния программы
    int currentPuzzle = 0;
    bool menuOpen = false;
    bool showResult = false;
    sf::Clock resultTimer;
    sf::Text resultText("", font, 72);
    resultText.setStyle(sf::Text::Bold);

    // Главный цикл
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка кликов
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    {event.mouseButton.x, event.mouseButton.y});

                // Закрытие программы
                if (closeButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }

                // Проверка решения
                if (checkButton.getGlobalBounds().contains(mousePos))
                {
                    std::string answerFile = "puzzles/puzzleAnswer" +
                                             std::to_string(currentPuzzle + 1) + ".txt";
                    bool correct = GridUtils::Check(answerFile.c_str(), grid, gridSize);

                    resultText.setString(correct ? "CORRECT!" : "WRONG!");
                    resultText.setFillColor(correct ? sf::Color::Green : sf::Color::Red);
                    resultText.setPosition(
                        (window.getSize().x - resultText.getLocalBounds().width) / 2,
                        (window.getSize().y - resultText.getLocalBounds().height) / 2);
                    showResult = true;
                    resultTimer.restart();
                }

                // Открытие меню
                if (chooseButton.getGlobalBounds().contains(mousePos))
                {
                    menuOpen = !menuOpen;
                }

                // Выбор пазла
                if (menuOpen)
                {
                    for (int i = 0; i < maxPuzzles; ++i)
                    {
                        if (puzzleButtons[i].getGlobalBounds().contains(mousePos))
                        {
                            std::string puzzleFile = "puzzle" +
                                                     std::to_string(i + 1) + ".txt";

                            if (!fs::exists(puzzleFile))
                            {
                                std::cerr << "Missing puzzle file: " << puzzleFile << "\n";
                                continue;
                            }

                            currentPuzzle = i;
                            chooseText.setString("Puzzle " + std::to_string(i + 1));

                            // Сброс данных
                            GridUtils::ResetGrid(grid, gridSize);
                            CleanupNumberArray(lineNumbers, gridSize);
                            CleanupNumberArray(columnNumbers, gridSize);

                            // Пересоздание массивов
                            lineNumbers = new Number *[gridSize];
                            columnNumbers = new Number *[gridSize];
                            for (int j = 0; j < gridSize; ++j)
                            {
                                lineNumbers[j] = new Number[gridSize];
                                columnNumbers[j] = new Number[gridSize];
                            }

                            // Загрузка новых подсказок
                            processKeys(lineNumbers, columnNumbers,
                                        puzzleFile.c_str(),
                                        gridSize, font);
                            menuOpen = false;
                        }
                    }
                }

                // Обработка клеток
                GridUtils::HandleCellClick(grid,
                                           sf::Mouse::getPosition(window),
                                           gridSize);
            }
        }

        // Отрисовка
        window.clear();
        window.draw(background);

        // Сетка
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(grid[i][j].shape);
            }
        }

        // Интерфейс
        window.draw(closeButton);
        window.draw(checkButton);
        window.draw(chooseButton);
        window.draw(closeText);
        window.draw(checkText);
        window.draw(chooseText);
        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);
        window.draw(line5);
        window.draw(line6);
        window.draw(line7);
        window.draw(line8);

        // Меню пазлов
        if (menuOpen)
        {
            for (int i = 0; i < maxPuzzles; ++i)
            {
                window.draw(puzzleButtons[i]);
                window.draw(puzzleTexts[i]);
            }
        }

        // Подсказки
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                if (!lineNumbers[i][j].text.getString().isEmpty())
                {
                    window.draw(lineNumbers[i][j].text);
                }
                if (!columnNumbers[i][j].text.getString().isEmpty())
                {
                    window.draw(columnNumbers[i][j].text);
                }
            }
        }

        // Результат проверки
        if (showResult)
        {
            window.draw(resultText);
            if (resultTimer.getElapsedTime().asSeconds() > 2.0f)
            {
                showResult = false;
            }
        }

        window.display();
    }

    // Очистка памяти
    GridUtils::ResetGrid(grid, gridSize);
    CleanupNumberArray(lineNumbers, gridSize);
    CleanupNumberArray(columnNumbers, gridSize);

    for (int i = 0; i < gridSize; ++i)
    {
        delete[] grid[i];
    }
    delete[] grid;

    return EXIT_SUCCESS;
}
