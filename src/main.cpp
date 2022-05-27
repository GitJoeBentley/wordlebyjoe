#define _CRT_SECURE_NO_WARNINGS              // for MS Visual Studio

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;

#include "GameBoard.h"
#include "Constants.h"
#include "Words.h"
#include "Key.h"
#include "Statistics.h"
#include "Types.h"

char getKey();

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    unsigned targetRow = 0;
    unsigned targetCol = 0;
    unsigned hintCount;
    string message;
    string guess;
    char ltr;
    float deltaX;
    float deltaY;
    sf::Event event;
    sf::Vector2f mousePos;
    bool statsDisplayReturn;
    GameStatus gameStatus = Started;

    Statistics stats;
    GameBoard game;
    PlayerStatus playerStatus;

    // Music
    sf::Music music;
    music.openFromFile(Fifth);
    music.setVolume(15);

    // If this is the first game played, display "How to play"
    if (stats.getPlayed() == 0)
    {
        game.howtoplay();
        // This is a hack
        game.draw("");
        game.display();
    }

    // If less than one day since last game, set player status to Practice
    if (stats.getNumberOfDaysSinceLastGame() == 0)
    {
        playerStatus = Practice;
        gameStatus = GameOver;
        hintCount = 0;
        game.displayLastActiveGame(stats.getTileContents());
        // this is a hack to reduce flickering
        for (auto i = 0; i < 10; i++)
        {
            game.draw("");
            game.display();
        }

        game.get_background().setFillColor(sf::Color::White);
        stats.display(game.get_window(), gameStatus,playerStatus, game);
        message = "Press F1 to play a practice game";
    }
    else  // This is a new "actual" game
    {
        music.play();
        playerStatus = Active;
        stats.clearTileContents();
        stats.getNumberOfGuesses() = 0;
    }

    ////////////////////////// Game Loop /////////////////////////
    while (game.get_window().isOpen())
    {
        ltr = 0;   // "Clear" the letter entry

        // Event Loop to monitor mouse and keyboard
        while (game.get_window().pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                game.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (music.getStatus() == sf::SoundSource::Playing) music.stop();
                mousePos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                // check for How To Play Button
                deltaX = fabs(mousePos.x-game.gethowtoplayButtonPosition().x);
                deltaY = fabs(mousePos.y-game.gethowtoplayButtonPosition().y);
                if (deltaX < 13.0f and deltaY < 13.0f)
                {
                    game.howtoplay();
                    break;
                }

                // check for Stats Button
                deltaX = fabs(mousePos.x-game.getStatsButtonPosition().x);
                deltaY = fabs(mousePos.y-game.getStatsButtonPosition().y);
                if (deltaX < 12.0f and deltaY < 12.0f and stats.getPlayed() > 0)
                {
                    stats.display(game.get_window(), gameStatus,playerStatus, game);
                    break;
                }

                // Get the "key" from the "virtual keyboard"
                ltr = game.getKeyClicked(mousePos);
                break;
            case sf::Event::KeyPressed:
                if (music.getStatus() == sf::SoundSource::Playing) music.stop();

                // Remove this backdoor
//                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
//                {
//                    cout << "The word is " << game.getWords().whatsTheWord() << endl;
//                    break;
//                }

                // Did the player ask for a hint?
                // Only 3 hints are available for practice games
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && playerStatus == Practice && targetRow < 6 && hintCount < 3)
                {
                    hintCount++;
                    game.hint(targetRow);
                    break;
                }

                // Did the use ask for a practice game?
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) and gameStatus == GameOver)
                {
                    gameStatus = NewGame;
                    playerStatus = Practice;
                    ltr = 0;
                    message = "";
                    continue;
                }

                // Get the key from the keyboard
                ltr = getKey();

            default:
                ;
            }

            // Process keyboard/mouse input
            switch (ltr)
            {
            case 0:
                break;
            case 'b':                                    // Backspace
                if (targetCol)
                {
                    targetCol--;
                    game.get_tile(targetRow,targetCol).setLetter(' ');
                }
                break;
            case 'e':                                    // Enter key
                game.processEnter(playerStatus, gameStatus, stats, targetRow, targetCol);
                break;
            default:
                if (ltr && targetCol < 5)
                {
                    game.playSound(letter);
                    game.get_tile(targetRow,targetCol).setLetter(ltr);
                    targetCol++;
                }
            }

            game.draw(message);

            if (gameStatus == Win or gameStatus == Loss)
            {
                game.display();
                if (playerStatus == Active)
                {
                    statsDisplayReturn = stats.display(game.get_window(), gameStatus,playerStatus, game);

                    // If it is an "actual" game, write out the statistics file
                    if (playerStatus == Active)
                    {
                        stats.writeStatsFile();
                    }

                    if (statsDisplayReturn == false)
                        message = "Press F1 to play a practice game";
                    else
                        message = "";
                    game.draw(message);
                }
                else   // It's a practice game
                {
                    gameStatus = GameOver;
                    message = "Press F1 to play another practice game";
                    game.draw(message);
                }
            }

            // Player asks for a new "practice" game?
            if (gameStatus == NewGame)
            {
                playerStatus = Practice;
                gameStatus = Started;
                music.play();
                targetRow = 0;
                targetCol = 0;
                hintCount = 0;
                game.startPracticeGame();
            }

            game.display();
            ltr = 0;
        }
    }
}

// Determine what key is pressed
char getKey()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        return  'A';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        return  'B';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        return  'C';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        return  'D';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        return  'E';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        return  'F';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        return  'G';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        return  'H';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        return  'I';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        return  'J';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        return  'K';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        return  'L';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        return  'M';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        return  'N';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        return  'O';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        return  'P';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        return  'Q';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        return  'R';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        return  'S';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        return  'T';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
        return  'U';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        return  'V';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        return  'W';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return  'X';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        return  'Y';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        return  'Z';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
        return  'b';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        return  'e';
    return 0;
}
