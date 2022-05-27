#include "GameBoard.h"
#include "Constants.h"
#include "Keyboard.h"
#include "Words.h"
#include <iostream>
#include <cmath>
using namespace std;

sf::Font GameBoard::TheFont;

GameBoard::GameBoard()
    :  words(),
       window(sf::VideoMode(static_cast<unsigned>(GameSize.x), static_cast<unsigned>(GameSize.y)), "", sf::Style::Close),
       background(GameSize),
       title(sf::String(Title),TheFont),
       line1(sf::Vector2f(GameSize.x,1.0f)),
       line2(sf::Vector2f(GameSize.x,1.0f))
{
    window.setTitle(sf::String(Title));
    TheFont.loadFromFile(FontFile);

    // Update title
    title.setCharacterSize(32);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color::Black);
    title.setPosition(GameSize.x/2.0f,0.022f*GameSize.y);
    title.setOrigin(title.getLocalBounds().width/2.0f,title.getLocalBounds().height / 2.0f);

    line1.setFillColor(GreyColor);
    line1.setPosition(0.0f,0.004f*GameSize.y);
    line2.setFillColor(GreyColor);
    line2.setPosition(0.0f,0.061f*GameSize.y);

    statsButtonTexture.loadFromFile(StatsButtonImageFile);
    statsButton.setTexture(statsButtonTexture);
    statsButton.setPosition(sf::Vector2f(0.96f*GameSize.x,0.03f*GameSize.y));
    statsButton.setOrigin(statsButton.getLocalBounds().width/2.0f,statsButton.getLocalBounds().height / 2.0f);

    howtoplayButtonTexture.loadFromFile(HowToPlayButtonImageFile);
    howtoplayButton.setTexture(howtoplayButtonTexture);
    howtoplayButton.setPosition(sf::Vector2f(0.04f*GameSize.x,0.03f*GameSize.y));
    howtoplayButton.setOrigin(howtoplayButton.getLocalBounds().width/2.0f,howtoplayButton.getLocalBounds().height / 2.0f);
}

sf::RenderWindow& GameBoard::get_window()
{
    return window;
}

Tiles& GameBoard::get_tiles()
{
    return tiles;
}

sf::RectangleShape& GameBoard::get_background()
{
    return background;
}

const sf::Vector2f& GameBoard::gethowtoplayButtonPosition() const
{
    return howtoplayButton.getPosition();
}

const sf::Vector2f& GameBoard::getStatsButtonPosition() const
{
    return statsButton.getPosition();
}

std::string GameBoard::getGuess(unsigned row) const
{
    string guess;
    for (auto col = 0; col < 5; col++)
        guess += tiles.getTile(row,col)->getTile().getChar();
    return guess;
}

Tile& GameBoard::get_tile(unsigned row, unsigned col)
{
    return *(tiles.getTile(row,col));
}

Keyboard& GameBoard::getKeyboard()
{
    return keyboard;
}

void GameBoard::playSound(SoundType type)
{
    sounds.playSound(type);
}

void GameBoard::close()
{
    window.close();
}

char GameBoard::getKeyClicked(sf::Vector2f mousePos)
{
    return keyboard.getKeyClicked(mousePos);
}

const Words& GameBoard::getWords() const
{
    return words;
}

Words& GameBoard::getWords()
{
    return words;
}

void GameBoard::updateRow(unsigned row, unsigned* colors)
{
    // Change tiles colors
    for (auto col = 0; col < 5; col++)
    {
        tiles.getTile(row,col)->getTile().setFillColor(TileColors[colors[col]]);
        tiles.getTile(row,col)->getLetter().setFillColor(sf::Color::White);
        // flip the tile
        for (auto j = 1; j <= 5; j++)
        {
            tiles.getTile(row,col)->flip(0.2f*j);
            tiles.getTile(row,col)->draw(window);
            window.display();
            sf::sleep(sf::Time(sf::seconds(0.09f)));
        }
        tiles.getTile(row,col)->draw(window);
    }
}

void GameBoard::displayLastActiveGame(const TileContents& tileContents)
{
    char letter;
    unsigned temp;
    unsigned colorCode;
    sf::Color currentKeyColor;
    sf::Color newKeyColor;
    sf::Sprite* ptrKey;
    for (auto row = 0; row < 6; row++)
    {
        for (auto col = 0; col < 5; col++)
        {
            temp = tileContents[row][col];
            letter = static_cast<char>(temp/100);
            colorCode = temp %100;
            newKeyColor = TileColors[colorCode];
            if (letter != 0)
            {
                tiles.getTile(row,col)->getTile().setFillColor(newKeyColor);
                tiles.getTile(row,col)->setLetter(letter);
                tiles.getTile(row,col)->getLetter().setFillColor(sf::Color::White);

                // update keyboard colors
                ptrKey = &keyboard.getKey(letter).getKey();
                currentKeyColor = ptrKey->getColor();
                if (currentKeyColor == LightGreyColor) ptrKey->setColor(newKeyColor);
                else if (currentKeyColor == GreyColor) ptrKey->setColor(newKeyColor);
                else if (currentKeyColor == YellowColor and newKeyColor == GreenColor) ptrKey->setColor(newKeyColor);
                else {}
            }
            else break;
        }
    }
}

void GameBoard::copyGameToClipboard(const TileContents& tileContents, unsigned gameNumber, unsigned numberOfGuesses)
{
    char letter;
    unsigned temp;
    sf::Color currentKeyColor;
    sf::Color newKeyColor;
    unsigned maxRow;

    // Determine the number of rows of tiles
    for (unsigned row = 0; row < 6; row++)
    {
        temp = tileContents[row][0];
        letter = static_cast<char>(temp/100);
        if (letter != 0)
        {
            maxRow = row;
        }
        else break;
    }

    // create text line
    std::string str = "Wordle " + to_string(gameNumber) + ' ' + to_string(numberOfGuesses) + "/6";
    sf::Text line(sf::String(str),TheFont);
    line.setCharacterSize(16);
    line.setFillColor(sf::Color::Black);
    line.setPosition(sf::Vector2f(10.0f, 10.0f));

    // Create an array of "tiles"
    RowOfTiles* ptr2Tiles = new RowOfTiles[maxRow+1];
    for (unsigned row = 0; row <= maxRow; row++)
    {
        for (unsigned col = 0; col < 5; col++)
        {
            ptr2Tiles[row][col].setSize(sf::Vector2f(TileWidth/3.0f,TileHeight/3.0f));
            ptr2Tiles[row][col].setFillColor(TileColors[tileContents[row][col]%100]);
            ptr2Tiles[row][col].setOutlineColor(sf::Color::Black);
            ptr2Tiles[row][col].setOutlineThickness(2.0f);
            ptr2Tiles[row][col].setPosition(sf::Vector2f(col*27.0f + 10.0f,row*29.0f + 40.0f));
        }
    }

    window.clear(sf::Color::White);
    window.draw(line);
    for (auto i = 0; i < 2; i++)
    {
        for (auto row = 0U; row <= maxRow; row++)  // draw tiles
        {
            for (auto col = 0U; col < Tiles::Cols; col++)
            {
                window.draw(ptr2Tiles[row][col]);
            }
        }
        window.display();
    }

    sf::Vector2u windowSize = window.getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(window);
    sf::Image screenshot = texture.copyToImage();
    screenshot.saveToFile("game_results.jpg");
    delete [] ptr2Tiles;
    ptr2Tiles = nullptr;
}

void GameBoard::updateKeyboard(const string& guess,unsigned int* eval)
{
    sf::Color currentKeyColor;
    sf::Color newKeyColor;
    for (auto col = 0; col < 5; col++)
    {
        unsigned keyboardIndex = keyboard.getLetterIndex(guess[col]);
        currentKeyColor = keyboard.getKey(keyboardIndex).getKey().getColor();
        newKeyColor = TileColors[eval[col]];

        if (currentKeyColor == LightGreyColor) keyboard.getKey(keyboardIndex).getKey().setColor(newKeyColor);
        else if (currentKeyColor == GreyColor) keyboard.getKey(keyboardIndex).getKey().setColor(newKeyColor);
        else if (currentKeyColor == YellowColor and newKeyColor == GreenColor) keyboard.getKey(keyboardIndex).getKey().setColor(newKeyColor);
        else {}
    }
}

void GameBoard::draw(string msg)
{
    window.draw(background);
    window.draw(title);
    window.draw(line1);
    window.draw(line2);
    window.draw(howtoplayButton);
    window.draw(statsButton);
    if (msg.size()) displayMessage(msg);

    // Draw the tiles
    for (auto row = 0U; row < Tiles::Rows; row++)
    {
        for (auto col = 0U; col < Tiles::Cols; col++)
        {
            tiles.getTile(row,col)->draw(window);
        }
    }

    // Draw the virtual keyboard
    for (unsigned i = 0; i < 26; i++)
    {
        keyboard.getKey(i).draw(window);
    }

    keyboard.getEnterKey().draw(window);          // Draw the enter key
    keyboard.getBackspaceKey().draw(window);      // Draw the backspace key
}

void GameBoard::display()
{
    window.display();
}

void GameBoard::wiggleTilesVertical(unsigned row,const string& msg)
{
    displayMessage(msg);
    for (unsigned col = 0; col < 5; col++)
    {
        tiles.getTile(row,col)->moveUp(10.0f);
        draw(msg);
        window.display();
        sf::sleep(sf::Time(sf::seconds(0.13f)));
    }
    for (unsigned col = 0; col < 5; col++)
    {
        tiles.getTile(row,col)->moveUp(-10.0f);
        draw(msg);
        window.display();
        sf::sleep(sf::Time(sf::seconds(0.13f)));
    }
    sf::sleep(sf::Time(sf::seconds(1.0)));
    draw();
    window.display();
    sf::sleep(sf::Time(sf::seconds(1.0)));
}

void GameBoard::displayMessage(const string& msg)
{
    // Message Box
    sf::Text message(sf::String(msg),TheFont);
    message.setCharacterSize(16);
    message.setStyle(sf::Text::Bold);
    message.setFillColor(sf::Color::White);
    message.setPosition(static_cast<float>(GameSize.x/2.0f),0.11f*GameSize.y);
    message.setOrigin(message.getLocalBounds().width/2.0f,message.getLocalBounds().height / 2.0f);
    sf::RectangleShape messageBox;
    if (msg == "Not enough letters")
        messageBox.setFillColor(GreyColor);
    else
        messageBox.setFillColor(sf::Color::Black);
    messageBox.setPosition(GameSize.x/2.0f,0.11f*GameSize.y);
    message.setString(msg);
    messageBox.setSize(sf::Vector2f(1.25f * message.getLocalBounds().width,40.0f));
    messageBox.setOrigin(messageBox.getLocalBounds().width/2.0f,messageBox.getLocalBounds().height / 2.0f);

    window.draw(messageBox);
    window.draw(message);
}

void GameBoard::resetTileRowPosition(unsigned row)
{
    float xpos, ypos;
    for (unsigned col = 0; col < 5; col++)
    {
        xpos = tiles.getTile(row,col)->getTile().getPosition().x;
        ypos = tiles.getInitialRowYPosition(row);
        tiles.getTile(row,col)->getTile().setPosition(xpos,ypos);
        tiles.getTile(row,col)->getLetter().setPosition(xpos-10,ypos-19);
    }
}

void GameBoard::wiggleTiles(unsigned targetRow,const string& msg)
{
    unsigned errorFlagCount = 16;
    while (errorFlagCount)
    {
        window.clear();

        switch (errorFlagCount % 4)
        {
        case 0:
            tiles.moveRowRight(targetRow);
            break;
        case 1:
            tiles.moveRowLeft(targetRow);
            tiles.moveRowLeft(targetRow);
            break;
        case 2:
            tiles.moveRowRight(targetRow);
            tiles.moveRowRight(targetRow);
            break;
        case 3:
            tiles.moveRowLeft(targetRow);
            break;
        default:
            ;
        }
        draw(msg);
        window.display();
        sf::sleep(sf::Time(sf::seconds(0.08f)));
        errorFlagCount--;
    }
}

GameStatus GameBoard::processEnter(const PlayerStatus& playerStatus, GameStatus& gameStatus, Statistics& stats, unsigned& row, unsigned& col)
{
    string guess;
    string message;
    bool guessStatus;
    unsigned tileValue;

    unsigned guessEvaluation[5];

    if (col < 5)
    {
        playSound(doh);
        wiggleTiles(row, string("Not enough letters"));
        return gameStatus;
    }
    else
    {
        guess = getGuess(row);

        if (words.isValidWord(guess))
        {
            playSound(enter);
            stats.getNumberOfGuesses()++;    // add a guess
            guessStatus = words.evaluateGuess(guess, guessEvaluation);

            // Record guess history if active game
            if (playerStatus == Active)
            {
                //cout << "guess = " << guess << endl;
                for (auto i = 0; i < 5; i++)
                {
                    tileValue = guess[i]*100u+guessEvaluation[i];
                    stats.setTileContents(row,i,tileValue);
                }
            }

            if (guessStatus) gameStatus = Win;
            updateRow(row,guessEvaluation);
            updateKeyboard(guess,guessEvaluation);

            ///////////////// Win ///////////////////
            if (gameStatus == Win)
            {
                sounds.playWinLoseSound(row+1);
                message = WinValues[row];
                draw(message);
                wiggleTilesVertical(row,message);
                return gameStatus;
            }

            ///////////////// Loss //////////////////
            if (row == 5)
            {
                gameStatus = Loss;
                sounds.playWinLoseSound(0);
                stats.getNumberOfGuesses() = 0;

                message = words.whatsTheWord();
                // This is a hack to keep the message from flickering
                for (auto i = 0; i < 2500; i++)
                {
                    draw(message);
                    window.display();
                }
                draw("");
                window.display();
                /////////////////////////////////////////////////////
                return gameStatus;
            }
        }
        else
        {
            wiggleTiles(row,string("Not in word list"));
            return gameStatus;
        }
        col = 0;
        row++;
    }
    return gameStatus;
}

void GameBoard::startPracticeGame()
{
    get_background().setFillColor(LightYellowColor);
    get_tiles().clear();
    getKeyboard().clearColors();
    getWords().getRandomWord();
    draw("");
}

// Display one random letter from the word temporarily
void GameBoard::hint(unsigned targetRow)
{
    int r = rand() % 5;
    get_tile(targetRow,r).setLetter(words.whatsTheWord()[r]);
    for (auto i = 0; i < 1000; i++)
    {
        draw("");
        window.display();
    }
    get_tile(targetRow,r).setLetter(' ');
}

void GameBoard::howtoplay()
{
    sf::RectangleShape howtoplayRect(GameSize);
    howtoplayRect.setFillColor(sf::Color(sf::Color::White));
    howtoplayRect.setOutlineColor(sf::Color(231,241,231));
    howtoplayRect.setOutlineThickness(10.0f);
    howtoplayRect.setPosition(0.0f,0.0f);

    std::string text;
    text = getHowToPlayText();
    sf::Text instructions;
    instructions.setString(text.c_str());
    instructions.setFont(GameBoard::TheFont);
    instructions.setCharacterSize(16); // in pixels, not points!
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(60.0f,10.0f);

    sf::Text lastLine("One new word is available to play each day, but you can practice all you want.",GameBoard::TheFont,16);
    lastLine.setFillColor(sf::Color::Black);
    lastLine.setPosition(60.0f,640.0f);

    sf::Texture howtoplayexampleTexture;
    howtoplayexampleTexture.loadFromFile(HowToPlayImageFile);
    sf::Sprite howtoplayexample(howtoplayexampleTexture);
    howtoplayexample.setScale(sf::Vector2f(1.0f,1.0f));
    howtoplayexample.setPosition(sf::Vector2f(210.0f,310.0f));

    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                return;
            default:
                ;
            }
        }
        window.clear();
        window.draw(howtoplayRect);
        window.draw(instructions);
        window.draw(howtoplayexample);
        window.draw(lastLine);
        window.display();
    }
}

std::string GameBoard::getHowToPlayText() const
{
    std::ifstream fin(HowToPlayFile);
    std::string text, buffer;
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return "";
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    fin.close();
    return text;
}
