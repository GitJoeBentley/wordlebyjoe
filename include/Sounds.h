#ifndef SOUNDS_H
#define SOUNDS_H
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Types.h"

class Sounds
{
public:

    Sounds();
    virtual ~Sounds() = default;
    void playSound(SoundType type);
    void playWinLoseSound(unsigned numberOfGuesses);
    static const std::string winSoundFiles[7];

private:
    sf::SoundBuffer letterSoundBuffer;
    sf::Sound letterSound;
    sf::SoundBuffer enterSoundBuffer;
    sf::Sound enterSound;
    sf::SoundBuffer dohSoundBuffer;
    sf::Sound dohSound;
    sf::SoundBuffer winloseSoundBuffer;
    sf::Sound winloseSound;
};

#endif // SOUNDS_H
