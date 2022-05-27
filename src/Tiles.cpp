#include <iostream>
#include "Tiles.h"
#include "Constants.h"

Tiles::Tiles()
{
    sf::Vector2f location;
    for (auto row = 0U; row < Rows; row++)
    {
        for (auto col = 0U; col < Cols; col++)
        {
            location = sf::Vector2f(0.265f*GameSize.x+TileWidth/2.0f+col*(TileWidth + TileBorder),0.19f*GameSize.y+row * (TileHeight + TileBorder));
            array[row][col] = new Tile(location);
        }
    }
}

Tiles::~Tiles()
{
    for (auto row = 0U; row < Rows; row++)
    {
        for (auto col = 0U; col < Cols; col++)
        {
            delete array[row][col];
        }
    }
}

void Tiles::clear()
{
    for (auto row = 0U; row < Rows; row++)
    {
        for (auto col = 0U; col < Cols; col++)
        {
            array[row][col]->clear();
        }
    }
}

const Tile* Tiles::getTile(unsigned row, unsigned col) const
{
    return array[row][col];
}

Tile* Tiles::getTile(unsigned row, unsigned col)
{
    return array[row][col];
}

float Tiles::getInitialRowYPosition(unsigned row) const
{
    return 0.19f*GameSize.y+row * (TileHeight + TileBorder);
}

void Tiles::moveRowLeft(unsigned row)
{
    for (unsigned col = 0; col < 5; col++)
    {
        array[row][col]->moveLeft(5.0);
    }
}

void Tiles::moveRowRight(unsigned row)
{
    for (unsigned col = 0; col < 5; col++)
    {
        array[row][col]->moveRight(5.0);
    }
}

void Tiles::wiggleRowVertical(unsigned row)
{
    for (unsigned col = 0; col < 5; col++)
    {
        array[row][col]->moveUp(5.0f);
        sf::sleep(sf::Time(sf::seconds(0.1f)));
        array[row][col]->moveUp(5.0f);
        sf::sleep(sf::Time(sf::seconds(0.1f)));
    }
}

void Tiles::resetRowPosition(unsigned row)
{
    float xpos, ypos;
    for (unsigned col = 0; col < 5; col++)
    {
        xpos = array[row][col]->getTile().getPosition().x;
        ypos = getInitialRowYPosition(row);
        array[row][col]->getTile().setPosition(xpos,ypos);
        array[row][col]->getLetter().setPosition(xpos-10,ypos-19);
    }
}
