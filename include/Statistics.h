#ifndef STATISTICS_H
#define STATISTICS_H
#ifdef _MSC_VER       // for MS Visual Studio
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <fstream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Types.h"

class GameBoard;  // forward declaration

class Statistics
{
public:
    Statistics();
    virtual ~Statistics() = default;
    unsigned getGameNumber() const;
    unsigned getPlayed() const;
    std::string getStats() const;
    unsigned getNumberOfGuesses() const;
    unsigned& getNumberOfGuesses();
    bool display(sf::RenderWindow& window, GameStatus& gameStatus, PlayerStatus playerStatus, GameBoard& game);
    void calculateStats(GameStatus& gameStatus);
    unsigned percent() const;
    void readStatsFile();
    void writeStatsFile();
    void createText(sf::Text* text[9]) const;
    void createBarGraph(sf::RectangleShape bars[7], sf::Text distributionValues[7]) const;
    time_t getDateFromDateTime(time_t when) const;
    unsigned getNumberOfDaysSinceLastGame() const;
    void setTileContents(unsigned row, unsigned col, unsigned value);
    const TileContents& getTileContents() const;
    void clearTileContents();

protected:

private:
    unsigned gameNumber = 0;
    time_t lastDate;  // Last date (completed game) played - stores only mmddyy
    unsigned played = 0;
    unsigned distribution[7] = {0};
    unsigned currentStreak = 0;
    unsigned maxStreak = 0;
    unsigned numberOfGuesses = 0;
    unsigned tileContents[6][5] = {0};  // contains the (letter/evaluation) history of the last active game
    std::string getTimeToNextGame() const;
};

#endif // STATISTICS_H
