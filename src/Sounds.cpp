#include "Sounds.h"
#include <string>
using namespace std;

const std::string Sounds::winSoundFiles[7] =
{
    "lose","yeehaw","Cheering","Clapping","crowdhomerunapplause","crowdapplause","ywwowoo"
};

Sounds::Sounds()
{
    letterSoundBuffer.loadFromFile(LetterSoundFile);
    letterSound.setBuffer(letterSoundBuffer);
    letterSound.setVolume(2.0f);

    enterSoundBuffer.loadFromFile(EnterSoundFile);
    enterSound.setBuffer(enterSoundBuffer);
    enterSound.setVolume(3.0f);

    dohSoundBuffer.loadFromFile(Homer_DohSoundFile);
    dohSound.setBuffer(dohSoundBuffer);
    dohSound.setVolume(50.0f);
}

void Sounds::playWinLoseSound(unsigned numberOfGuesses)
{
    string soundFile = ResourcePath + winSoundFiles[numberOfGuesses] + ".wav";
    winloseSoundBuffer.loadFromFile(soundFile);
    winloseSound.setBuffer(winloseSoundBuffer);
    winloseSound.setVolume(40.0f);
    winloseSound.play();
}

void Sounds::playSound(SoundType type)
{
    switch (type)
    {
    case letter:
        letterSound.play();
        break;
    case enter:
        enterSound.play();
        break;
    case doh:
        dohSound.play();
        break;
    default:
        ;
    }
}
