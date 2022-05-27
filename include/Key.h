#ifndef KEY_H
#define KEY_H

#include <SFML/Graphics.hpp>

class Key
{
   public:
      Key(char ch, sf::Texture& texture, sf::Vector2f position);
      virtual ~Key() = default;
      char getLetter() const { return letter; }
      sf::Sprite& getKey() { return key; }
      const sf::Text& getLetterText() const { return letterText; }
      sf::Text& getLetterText() { return letterText; }
      void draw(sf::RenderWindow& window);
      sf::Vector2f getKeyPosition() const { return key.getPosition(); }
      bool mouseClick(sf::Vector2f mousePosition) const;

   private:
      char letter;
      sf::Sprite key;
      sf::Text letterText;
};

#endif // KEY_H
