#include <iostream>
#include "Keyboard.h"
#include "Constants.h"

Keyboard::Keyboard()
{
    const char letters[] = {"QWERTYUIOPASDFGHJKLZXCVBNM"};
    // const sf::Vector2f KeySize(43.0f,58.0f);
    const sf::Vector2f KeySize(37.0f,49.0f);
    const float KeyMargin(4.0f);

    keyTexture.loadFromFile(KeyImageFile);
    enterTexture.loadFromFile(EnterImageFile);
    backspaceTexture.loadFromFile(BackspaceImageFile);

    sf::Vector2f keyPosition(QkeyPosition);
    for (auto i = 0; i < 26; i++)
    {
        if (i == 10)  // start of second row
        {
            keyPosition.x = QkeyPosition.x + KeySize.x / 2.0f + KeyMargin;
            keyPosition.y += KeySize.y + KeyMargin;
        }
        else if (i == 19)
        {
            keyPosition.x = QkeyPosition.x + 1.5f * KeySize.x + KeyMargin;
            keyPosition.y += KeySize.y + KeyMargin;
        }

        keys[i] = new Key(letters[i],keyTexture,keyPosition);
        keyPosition.x += KeySize.x + KeyMargin;
    }
    enter = new Key(' ',enterTexture,sf::Vector2f(QkeyPosition.x+10.0f,QkeyPosition.y + 2.0f * (KeySize.y + KeyMargin)));
    backspace = new Key(' ',backspaceTexture,sf::Vector2f(QkeyPosition.x + 9.0f * KeySize.x + 5.5f * KeyMargin,QkeyPosition.y + 2.0f * (KeySize.y + KeyMargin)));
}

void Keyboard::clearColors()
{
    for (unsigned i = 0; i < 26; i++)
    {
        getKey(i).getKey().setColor(LightGreyColor);
        getKey(i).getLetterText().setFillColor(sf::Color::Black);
    }
}

Keyboard::~Keyboard()
{
    for (auto i = 0; i < 26; i++)
    {
        delete keys[i];
    }
    delete enter;
    delete backspace;
}

unsigned Keyboard::getLetterIndex(char ch)
{
    for (unsigned i = 0; i < 26; i++)
    {
        if (getKey(i).getLetter() == ch) return i;
    }
    return 26;
}

sf::Vector2f Keyboard::getKeyPosition(char ch)
{
    unsigned index = getLetterIndex(ch);
    return getKey(index).getKey().getPosition();
}

char Keyboard::getKeyClicked(sf::Vector2f mousePosition)
{
    for (unsigned i = 0; i < 26; i++)
    {
        if (keys[i]->mouseClick(mousePosition)) return keys[i]->getLetter();
    }
    if (enter->mouseClick(mousePosition)) return 'e';  // 'e' = Enter key
    if (backspace->mouseClick(mousePosition)) return 'b';  // 'b' = backspace key
    return 0;
}


