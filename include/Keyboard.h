#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>
#include "Key.h"


class Keyboard
{
public:
    Keyboard();
    virtual ~Keyboard();
    Key& getKey(unsigned index) { return *(keys[index]); }
    Key& getKey(char ch) { return getKey(getLetterIndex(ch)); }
    Key& getEnterKey() { return *enter; }
    Key& getBackspaceKey() { return *backspace; }
    unsigned getLetterIndex(char ch);
    sf::Vector2f getKeyPosition(char ch);
    char getKeyClicked(sf::Vector2f mousePosition);
    void clearColors();

private:
    Key* keys[26];  // key size is 43x58, enter, backspace 65x58
    Key* enter;
    Key* backspace;
    sf::Texture keyTexture;
    sf::Texture enterTexture;
    sf::Texture backspaceTexture;
    sf::Vector2f backspacePosition;
};

#endif // KEYBOARD_H
