#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <SFML/Graphics.hpp>
#include "Tiles.h"
#include "Keyboard.h"
#include "Sounds.h"
#include "Words.h"
#include "Statistics.h"
#include "Types.h"

class GameBoard
{
public:
    GameBoard();
    virtual ~GameBoard() = default;
    sf::RenderWindow& get_window();
    Tiles& get_tiles();
    sf::RectangleShape& get_background();
    const Words& getWords() const;
    Words& getWords();
    Tile& get_tile(unsigned row, unsigned col);
    Keyboard& getKeyboard();
    char getKeyClicked(sf::Vector2f mousePos);
    std::string getGuess(unsigned row) const;
    const sf::Vector2f& getStatsButtonPosition() const;
    const sf::Vector2f& gethowtoplayButtonPosition() const;

    void display();
    void draw(string msg = "");
    void displayGuess(unsigned row);
    void wiggleTiles(unsigned targetRow,const string& msg);
    void wiggleTilesVertical(unsigned targetRow,const string& msg);
    void displayMessage(const string& msg);
    void updateRow(unsigned row, unsigned* colors);
    void updateKeyboard(const string& guess,unsigned int* eval);
    void resetTileRowPosition(unsigned row);
    void displayStats();
    GameStatus processEnter(const PlayerStatus& playerStatus, GameStatus& status, Statistics& stats, unsigned& row, unsigned& col);
    void playSound(SoundType type);
    void close();
    void displayLastActiveGame(const TileContents& tileContents);
    void hint(unsigned targetRow);
    void copyGameToClipboard(const TileContents& tileContents, unsigned gameNumber, unsigned numberOfGuesses);
    void howtoplay();
    void startPracticeGame();

    static sf::Font TheFont;

private:
    Words words;
    sf::RenderWindow window;
    sf::RectangleShape background;
    sf::Text title;
    sf::RectangleShape line1;
    sf::RectangleShape line2;
    sf::Texture howtoplayButtonTexture;
    sf::Sprite howtoplayButton;
    sf::Texture statsButtonTexture;
    sf::Sprite statsButton;
    Tiles tiles;
    Keyboard keyboard;
    Sounds sounds;

    std::string getHowToPlayText() const;
};

#endif // GAMEBOARD_H
