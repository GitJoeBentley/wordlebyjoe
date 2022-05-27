#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <SFML/Graphics.hpp>

enum SoundType {letter,enter,doh,win,lose};
enum GameStatus { Started, Win, Loss, GameOver, NewGame};
enum PlayerStatus { Inactive, Active, Practice };
using TileContents = unsigned [6][5];
using RowOfTiles = sf::RectangleShape[5];


#endif // TYPES_H_INCLUDED
