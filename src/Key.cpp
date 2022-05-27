#include "Key.h"
#include "GameBoard.h"
#include "Constants.h"

Key::Key(char ltr, sf::Texture& texture, sf::Vector2f position) : letter(ltr)
{
    key.setTexture(texture);
    key.setColor(LightGreyColor);
    key.setPosition(position);
    key.setOrigin(key.getLocalBounds().width/2.0f,key.getLocalBounds().height / 2.0f);

    letterText.setFont(GameBoard::TheFont);
    letterText.setCharacterSize(16);
    letterText.setFillColor(sf::Color::Black);
    // Notice the offset of the positioning of the keyboard letter
    letterText.setPosition(sf::Vector2f(position.x-7.0f,position.y-7.0f));
    letterText.setString(letter);
}

void Key::draw(sf::RenderWindow& window)
{
    window.draw(key);
    window.draw(letterText);
}

bool Key::mouseClick(sf::Vector2f mousePosition) const
{
   float keyTop = key.getPosition().y - key.getLocalBounds().height/2.0f;
   float keyBottom = key.getPosition().y + key.getLocalBounds().height/2.0f;
   float keyLeftSide = key.getPosition().x - key.getLocalBounds().width/2.0f;
   float keyRightSide = key.getPosition().x + key.getLocalBounds().width/2.0f;
   if (mousePosition.x < keyRightSide && mousePosition.x > keyLeftSide && mousePosition.y < keyBottom && mousePosition.y > keyTop) return true;
   return false;
}
