#ifndef TILE_H
#define TILE_H

#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include "externs.h"


class Tile
{
public:
    Tile(sf::Vector2f position);
    virtual ~Tile() = default;
    Tile getTile() const;
    sf::RectangleShape& getTile();
    void draw(sf::RenderWindow& window);
    void drawTile(sf::RenderWindow& window);
    void change_tile_color(sf::Color color);
    void moveLeft(float pixels);
    void moveRight(float pixels);
    void moveUp(float pixels);
    void moveVertical(float pixels);
    void setLetter(char ltr);
    char getChar() const;
    sf::Text& getLetter();
    void flip(float percent);
    void clear();

private:
    sf::RectangleShape tile;
    sf::Text letter;
};

#endif // TILE_H
