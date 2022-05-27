#include "Tile.h"
#include "GameBoard.h"
#include "Constants.h"


Tile::Tile(sf::Vector2f position)
    : tile(sf::Vector2f(TileWidth,TileHeight))
{
    tile.setPosition(position);
    tile.setOrigin(tile.getLocalBounds().width/2.0f,tile.getLocalBounds().height / 2.0f);
    tile.setOutlineColor(GreyColor);
    tile.setOutlineThickness(1.0f);

    // Position the letter in the center of the tile
    letter.setPosition(position.x+22,position.y+14);
    letter.setOrigin(tile.getLocalBounds().width/2.0f,tile.getLocalBounds().height / 2.0f);
    letter.setFont(GameBoard::TheFont);
    letter.setCharacterSize(32);
    letter.setStyle(sf::Text::Bold);
    clear();
}

void Tile::clear()
{
    tile.setFillColor(sf::Color::White);
    letter.setString(' ');
    letter.setFillColor(sf::Color::Black);
}

Tile Tile::getTile() const
{
    return *this;
}
sf::RectangleShape& Tile::getTile()
{
    return tile;
}

char Tile::getChar() const
{
    return letter.getString()[0];
}

sf::Text& Tile::getLetter()
{
    return letter;
}

void Tile::setLetter(char ltr)
{
    letter.setString(ltr);
}

void Tile::draw(sf::RenderWindow& window)
{
    window.draw(tile);
    window.draw(letter);
}

void Tile::drawTile(sf::RenderWindow& window)
{
    window.draw(tile);
}

void Tile::change_tile_color(sf::Color color)
{
    tile.setFillColor(color);
}

void Tile::moveLeft(float pixels)
{
    tile.setPosition(tile.getPosition().x - pixels,tile.getPosition().y);
    letter.setPosition(letter.getPosition().x - pixels,letter.getPosition().y);
}

void Tile::moveRight(float pixels)
{
    tile.setPosition(tile.getPosition().x + pixels,tile.getPosition().y);
    letter.setPosition(letter.getPosition().x + pixels,letter.getPosition().y);
}

void Tile::moveUp(float pixels)
{
    tile.setPosition(tile.getPosition().x,tile.getPosition().y-pixels);
    letter.setPosition(letter.getPosition().x,letter.getPosition().y-pixels);
}

void Tile::moveVertical(float pixels)
{
    tile.setPosition(tile.getPosition().x,tile.getPosition().y + pixels);
    letter.setPosition(letter.getPosition().x,letter.getPosition().y + pixels);
}

void Tile::flip(float percent)
{
    tile.setScale(1.0f,percent);
    letter.setScale(1.0f,percent);
    //  letter.setPosition(letter.getPosition().x,letter.getPosition().y-percent*TileBorder);
}
