#include "Words.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
using namespace std;

using std::string;
using std::ifstream;

Words::Words()
{
    readWordFile();
    getRandomWord();
}

void Words::readWordFile()
{
    // Read file in Binary
    ifstream fin(WordFile,ios::binary);
    if (!fin)
    {
        std::cerr << "Unable to open word file: " << WordFile << std::endl;
        exit(1);
    }

    char word[6];
    string sword;
    while (fin.read(word,5))
    {
        word[5] = 0;
        sword = word;
        // insert each word in uppercase
        for (auto it = sword.begin(); it != sword.end(); ++it)
            *it = toupper(*it);
        auto pr = words.insert(sword);
        if (pr.second == false) cout << *(pr.first) << endl;
    }
    //cout << "Number of words = " << words.size() << endl;
    //cout << "Last word is " << *words.crbegin() << endl;
}


void Words::getRandomWord()
{
    srand(static_cast<unsigned>(time(nullptr)));
    int index = rand() % words.size();
    auto it = words.cbegin();
    for (int i = 0; i < index; i++)
    {
        ++it;
    }
    theWord = *it;
    //cout << "The word is " << theWord << endl;
}

bool Words::isValidWord(const string& word)
{
    bool found = words.find(word) != words.end();
    return found;
}

// Evalutes guess:  assigns 5-element array, guessEvalution
// 0 = invalid Letter
// 1 = right letter, wrong place
// 2 = right letter, right place
// returns true if all 5 letters are right letter, right place
bool Words::evaluateGuess(string guess, unsigned* guessEvaluation)
{
    string theWordCopy = theWord;
    for (int i = 0; i < 5; i++) guessEvaluation[i] = 0;
    for (int i = 0; i < 5; i++)
    {
        if (guess[i] == theWordCopy[i])
        {
            guessEvaluation[i] = 2;
            guess[i] = ' ';
            theWordCopy[i] = ' ';
        }
    }
    size_t pos;
    for (int g = 0; g < 5; g++)
    {
        if (guess[g] != ' ')
        {
            pos = theWordCopy.find(guess[g]);
            if (pos != string::npos)
            {
                guessEvaluation[g] = 1;
                guess[g] = ' ';
                theWordCopy[pos] = ' ';
            }
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (guessEvaluation[i] != 2) return false;
    }
    return true;
}
