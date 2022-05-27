#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <SFML/Graphics.hpp>

const std::string ResourcePath = "resources/";
//const std::string ResourcePath = "";
const std::string FontFile = ResourcePath + "arial.ttf";
const std::string WordFile = ResourcePath + "wordfile.bin";
const std::string StatsFile = ResourcePath + "stats.txt";
const std::string HowToPlayFile = ResourcePath + "howtoplay.txt";
const std::string StatFile = ResourcePath + "stats.bin";
const std::string KeyImageFile = ResourcePath + "key.png";
const std::string EnterImageFile = ResourcePath + "enter_key.png";
const std::string BackspaceImageFile = ResourcePath + "backspace_key.png";
const std::string ShareButtonImageFile = ResourcePath + "share.png";
const std::string StatsButtonImageFile = ResourcePath + "stats_button.png";
const std::string HowToPlayButtonImageFile = ResourcePath + "howtoplay_button.png";
const std::string HowToPlayImageFile = ResourcePath + "howtoplayexample.png";
const std::string LetterSoundFile = ResourcePath + "letter.wav";
const std::string EnterSoundFile = ResourcePath + "enter.wav";
const std::string Homer_DohSoundFile = ResourcePath + "homer_doh.wav";
const std::string WinSoundFile = ResourcePath + "applause.wav";
const std::string LossSoundFile = ResourcePath + "letter.wav";
const std::string Fifth = ResourcePath + "fifth.ogg";
const std::string Title= "WORDLE by Joe";
const std::string WinValues[7] = {"Genius","Magnificient","Impressive","Splendid","Great","Phew","Choke"};

const sf::Vector2f GameSize(720.0f, 768.0f);
const sf::Vector2f QkeyPosition(172.0f,560.0f);
const sf::Vector2f StatWindowSize(515.0f, 515.0f);
const sf::Vector2f StatWindowPosition(98.f,160.0f);

const sf::Color GreenColor =  sf::Color(106,170,100);
const sf::Color YellowColor = sf::Color(218,192,71);
const sf::Color GreyColor =   sf::Color(128,128,128);
const sf::Color LightGreyColor = sf::Color(210,210,210);
const sf::Color LightYellowColor = sf::Color(245,235,185);
const sf::Color TileColors[3] = {GreyColor,YellowColor,GreenColor};

const float TileWidth = 60.0f;
const float TileHeight = 60.0f;
const float TileBorder = 8.0f;


#endif // CONSTANTS_H_INCLUDED
