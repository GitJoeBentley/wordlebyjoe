#include "Statistics.h"
#include "Constants.h"
#include "GameBoard.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

Statistics::Statistics()
{
    readStatsFile();
}

unsigned Statistics::getGameNumber() const
{
    return gameNumber;
}


unsigned Statistics::getPlayed() const
{
    return played;
}

const TileContents& Statistics::getTileContents() const
{
    return tileContents;
}

void Statistics::clearTileContents()
{
    for (auto row = 0; row < 6; row++)
    {
        for (auto col = 0; col < 5; col++)
        {
            tileContents[row][col] = 0u;
        }
    }
}

unsigned Statistics::getNumberOfGuesses() const
{
    return numberOfGuesses;
}

unsigned& Statistics::getNumberOfGuesses()
{
    return numberOfGuesses;
}

time_t Statistics::getDateFromDateTime(time_t when) const
{
    tm tm_when = *localtime(&when);
    tm_when.tm_hour = tm_when.tm_min = tm_when.tm_sec = 0;
    return mktime(&tm_when);
}

unsigned Statistics::getNumberOfDaysSinceLastGame() const
{
    time_t currentDate = getDateFromDateTime(time(nullptr));
    return static_cast<unsigned>((currentDate - lastDate))/ 86400;
}

void Statistics::readStatsFile()
{
    ifstream statfile(StatFile,ios::binary);
    if (statfile.is_open())
        statfile.read(reinterpret_cast<char*>(this),sizeof(*this));
    //cout << "Last Game # " << gameNumber << endl;
    //cout << "Last number of guesses = " << numberOfGuesses << endl;
    //cout << "Number of games played = " << played << endl;
    // test lastDate
    //lastDate = getDateFromDateTime(time(nullptr)) - 86400;
    statfile.close();
}

void Statistics::writeStatsFile()
{
    ofstream statfile(StatFile,ios::binary);
    statfile.write(reinterpret_cast<const char*>(this),sizeof(*this));
    statfile.close();
}

void Statistics::setTileContents(unsigned row, unsigned col, unsigned value)
{
    tileContents[row][col] = value;
}

unsigned Statistics::percent() const
{
    unsigned sum = 0;
    for (auto i = 1; i < 7; i++)
        sum += distribution[i];
    return static_cast<unsigned>(round(100.0f*sum/played));
}

void Statistics::calculateStats(GameStatus& gameStatus)
{
    if (gameStatus == Win or gameStatus == Loss)
    {
        played++;        // increment number of games played
        gameNumber++;    // increment the game number
        time_t currentDate = getDateFromDateTime(time(nullptr));

        if (gameStatus == Loss)
        {
            currentStreak = 0;
        }
        else if (currentDate - lastDate < 86401)
        {
            distribution[numberOfGuesses]++;
            currentStreak++;
        }
        else
        {
            distribution[numberOfGuesses]++;
            currentStreak = 1;
        }

        if (currentStreak > maxStreak)
        {
            maxStreak = currentStreak;
        }
    }
}

// Display the Statistics window
// Returns true if the share button is clicked
bool Statistics::display(sf::RenderWindow& window, GameStatus& gameStatus, PlayerStatus playerStatus, GameBoard& game)
{
    bool clickedShareButton = false;
    if (playerStatus == Active) calculateStats(gameStatus);

    lastDate = getDateFromDateTime(time(nullptr));

    sf::RectangleShape stats(StatWindowSize);
    stats.setFillColor(sf::Color(250,250,240));
    stats.setOutlineColor(sf::Color(231,241,231));
    stats.setOutlineThickness(10.0f);
    stats.setPosition(StatWindowPosition);
    sf::Event evnt;

    sf::Text* text[9];
    createText(text);

    sf::Vector2f mousePos;
    float deltaX;
    float deltaY;

    sf::RectangleShape bars[7];
    sf::Text distributionValues[7];

    createBarGraph(bars,distributionValues);

    // vertical line
    sf::RectangleShape verticalBar(sf::Vector2f(1.0f, 80.0f));
    verticalBar.setFillColor(sf::Color::Black);
    verticalBar.setPosition(StatWindowPosition.x + StatWindowSize.x / 2.0f,text[6]->getPosition().y - 10.0f);

    // Share button
    sf::RectangleShape shareButton(sf::Vector2f(176.0f, 52.0f));
    sf::Texture shareButtonTexture;
    shareButtonTexture.loadFromFile(ShareButtonImageFile);
    shareButton.setTexture(&shareButtonTexture);
    shareButton.setPosition(sf::Vector2f(StatWindowPosition.x + 0.73f*StatWindowSize.x,text[6]->getPosition().y+ 31.0f));
    shareButton.setOrigin(shareButton.getLocalBounds().width/2.0f,shareButton.getLocalBounds().height / 2.0f);

    while (window.isOpen())
    {
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::MouseButtonPressed:
                mousePos = sf::Vector2f(static_cast<float>(evnt.mouseButton.x), static_cast<float>(evnt.mouseButton.y));
                deltaX = fabs(mousePos.x-text[0]->getPosition().x);
                deltaY = fabs(mousePos.y-text[0]->getPosition().y);
                if (deltaX < 10.0f and deltaY < 10.0f)
                {
                    for (unsigned i = 0; i < 8; i++) delete text[i];
                    if (gameStatus == Win or gameStatus == Loss) gameStatus = GameOver;
                    return false;
                }
                else  // Check for share button
                {
                    deltaX = fabs(mousePos.x-shareButton.getPosition().x);
                    deltaY = fabs(mousePos.y-shareButton.getPosition().y);
                    if (deltaX < shareButton.getSize().x/2.0f and deltaY < shareButton.getSize().y/2.0f)
                    {
                        clickedShareButton = true;
                        game.copyGameToClipboard(getTileContents(),gameNumber,numberOfGuesses);
                        window.clear(sf::Color::White);
                        string msg = string("Your game results have been copied to the file, \"game_results.jpg\"");
                        game.draw(msg);
                        window.display();
                        sf::sleep(sf::Time(sf::seconds(5.0f)));
                        return clickedShareButton;
                    }
                }
                break;
            default:
                ;
            }
        }
        text[7]->setString(getTimeToNextGame());
        window.draw(stats);
        for (unsigned i = 0; i < 9; i++)
            window.draw(*(text[i]));
        window.draw(verticalBar);
        window.draw(shareButton);

        for (auto i = 1u; i <= 6; i++)
        {
            window.draw(bars[i]);
            window.draw(distributionValues[i]);
        }
        window.display();
    }
    return clickedShareButton;
}

void Statistics::createText(sf::Text* text[9]) const
{
    string str;
    text[0] = new sf::Text(sf::String('X'),GameBoard::TheFont,20);
    text[0]->setStyle(sf::Text::Bold);
    text[0]->setFillColor(sf::Color::Black);
    text[0]->setPosition(StatWindowPosition.x + 0.95f*StatWindowSize.x,StatWindowPosition.y + 0.04f*StatWindowSize.x);
    text[0]->setOrigin(text[0]->getLocalBounds().width/2.0f,text[0]->getLocalBounds().height / 2.0f);

    text[1] = new sf::Text(sf::String("STATISTICS"),GameBoard::TheFont,16);
    text[1]->setStyle(sf::Text::Bold);
    text[1]->setFillColor(sf::Color::Black);
    text[1]->setPosition(StatWindowPosition.x + 0.42f*StatWindowSize.x,StatWindowPosition.y + 0.08f*StatWindowSize.x);
    text[1]->setOrigin(text[0]->getLocalBounds().width/2.0f,text[0]->getLocalBounds().height / 2.0f);

    text[2] = new sf::Text(sf::String(getStats()),GameBoard::TheFont,32);
    text[2]->setStyle(sf::Text::Bold);
    text[2]->setFillColor(sf::Color::Black);
    text[2]->setPosition(StatWindowPosition.x + 0.3f*StatWindowSize.x,StatWindowPosition.y + 0.13f*StatWindowSize.x);

    text[3] = new sf::Text(sf::String("Played           Win %       Current          Max\n                                       Streak         Streak"),GameBoard::TheFont,13);
    text[3]->setFillColor(sf::Color::Black);
    text[3]->setPosition(StatWindowPosition.x + 0.29f*StatWindowSize.x,StatWindowPosition.y + 0.20f*StatWindowSize.x);

    text[4] = new sf::Text(sf::String("GUESS DISTRIBUTION"),GameBoard::TheFont,16);
    text[4]->setStyle(sf::Text::Bold);
    text[4]->setFillColor(sf::Color::Black);
    text[4]->setPosition(StatWindowPosition.x + 0.33f*StatWindowSize.x,StatWindowPosition.y + 0.30f*StatWindowSize.x);

    text[5] = new sf::Text(sf::String("1\n\n2\n\n3\n\n4\n\n5\n\n6"),GameBoard::TheFont,14);
    text[5]->setFillColor(sf::Color::Black);
    text[5]->setPosition(StatWindowPosition.x + 0.15f*StatWindowSize.x,StatWindowPosition.y + 0.38f*StatWindowSize.x-1.0f);

    text[6] = new sf::Text(sf::String("NEXT WORDLE"),GameBoard::TheFont,16);
    text[6]->setStyle(sf::Text::Bold);
    text[6]->setFillColor(sf::Color::Black);
    text[6]->setPosition(StatWindowPosition.x + 0.15f*StatWindowSize.x,StatWindowPosition.y + 0.77f*StatWindowSize.y);

    text[7] = new sf::Text(sf::String(getTimeToNextGame()),GameBoard::TheFont,36);
    text[7]->setStyle(sf::Text::Bold);
    text[7]->setFillColor(sf::Color::Black);
    text[7]->setPosition(StatWindowPosition.x + 0.14f*StatWindowSize.x,StatWindowPosition.y + 0.81f*StatWindowSize.y);

    text[8] = new sf::Text(sf::String(""),GameBoard::TheFont,14);
    text[8]->setFillColor(sf::Color::Blue);
    text[8]->setPosition(StatWindowPosition.x + 0.09f*StatWindowSize.x,StatWindowPosition.y + 0.94f*StatWindowSize.y);
}

string Statistics::getTimeToNextGame() const
{
    time_t now = time(nullptr);
    tm dateNow = *localtime(&now);
    dateNow.tm_hour = dateNow.tm_min = dateNow.tm_sec = 0;
    time_t nextGameTime = mktime(&dateNow) + 24 * 60 * 60;
    unsigned secondsToNextGame = static_cast<unsigned>(nextGameTime - now);
    unsigned hours, minutes, seconds;
    hours = secondsToNextGame / 3600;
    minutes = (secondsToNextGame - hours * 3600) / 60;
    seconds = secondsToNextGame % 60;
    ostringstream sout;
    sout << setfill('0');
    sout << setw(2) << hours << ':' << setw(2) << minutes << ':' << setw(2) << seconds;
    return sout.str();
}

string Statistics::getStats() const
{
    ostringstream sout;
    sout << left <<setw(6) << played << setw(6) << percent() << setw(6) << currentStreak << setw(6) << maxStreak;
    return sout.str();
}

void Statistics::createBarGraph(sf::RectangleShape bars[7], sf::Text distributionValues[7]) const
{
    for (auto i = 1u; i <= 6; i++)
    {
        bars[i].setPosition(sf::Vector2f(StatWindowPosition.x + 0.18f*StatWindowSize.x,StatWindowPosition.y + 0.38f*StatWindowSize.x + 32.f*(i-1.0f)-3.0f));
        if (i == numberOfGuesses) bars[i].setFillColor(GreenColor);
        else bars[i].setFillColor(GreyColor);
    }
    float distributionPercent[7] = {0.0f};
    unsigned maxDistribution = 1u;
    float barLength;
    string distributionValueString;
    unsigned distributionValueStringSize;

    for (auto element : distribution)
        maxDistribution = max(maxDistribution,element);

    for (auto i = 1u; i <= 6; i++)
    {
        distributionPercent[i] = 1.0f * distribution[i] / maxDistribution;
        barLength = 0.66f*StatWindowSize.x*distributionPercent[i] + 20.0f;
        bars[i].setSize(sf::Vector2f(barLength,20.0f));
        distributionValueString = to_string(distribution[i]);
        distributionValueStringSize = distributionValueString.size();
        distributionValues[i].setFont(GameBoard::TheFont);
        distributionValues[i].setString(distributionValueString);
        distributionValues[i].setCharacterSize(14);
        distributionValues[i].setFillColor(sf::Color::White);
        distributionValues[i].setPosition(bars[i].getPosition().x+barLength-14.0f*distributionValueStringSize,bars[i].getPosition().y+2.0f);
    }
}
