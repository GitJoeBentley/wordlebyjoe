#ifndef WORDS_H
#define WORDS_H

#include <set>
#include <string>

class Words
{
public:
    Words();
    virtual ~Words() = default;
    bool isValidWord(const std::string& word);
    bool evaluateGuess(std::string guess, unsigned* guessEvaluation);
    std::string whatsTheWord() const
    {
        return theWord;
    }
    void getRandomWord();

private:
    std::set<std::string> words;
    std::string theWord;

    void readWordFile();
};

#endif // WORDS_H
