#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 26 + 9 = 35 liter
using namespace std;


bool makeProbabilityFile(); // writing probability array to file. The array contains probability of next letter based on two previous letters.
bool readProbabilityFile(float tab[35][35][35], float tab2[35][35]); // reading probability file to arrays
int char2Int(char character);
char int2Char(int charNumber);
bool getTwoLetters(ifstream& file, int& firstLetterNumber, int& secondLetterNumber); // get two next letters from the file


int main()
{
    srand (time(NULL));

    // cout << makeProbabilityFile() << endl; // optional if probability file is already made (takes a lot of time)
    float probabilityArray [35][35][35]; // [0 - 100]
    float firstTwoLettersProbability [35][35]; // [0 - 100]
    readProbabilityFile(probabilityArray, firstTwoLettersProbability);
    int wordsNumber = 0;
    int minLength = 4;
    int maxLength = 9;
    cout << "Podaj liczbe slow do wygenerowania: ";
    cin >> wordsNumber;

    // ------------------------------------------------------- words number loop
    for (int words = 0; words < wordsNumber; words++)
    {
        // ------------------------------------------------------- get two first letter of a new word according to firstTwoLettersProbability array
        string word = "";
        float random = float(rand() % 10000) / 100 ; // [ 0 - 100 ]
        float sum = 0;

        for (int i = 0; i < 35; i++)
        {
            for (int j = 0; j < 35; j++)
            {
                sum += firstTwoLettersProbability[i][j];
                if (sum >= random)
                {
                    word += int2Char(i);
                    word += int2Char(j);
                    goto stop;
                }
            }
        }
        stop:

        int wordLength = rand()%(maxLength - minLength + 1) + minLength - 2;

        // ------------------------------------------------------- filling up the rest of the word according to probabilityArray
        for (int i = 0; i < wordLength; i++)
        {
            float random = float(rand() % 10000) / 100 ; // [ 0 - 100 ]
            float sum = 0;
            for (int k = 0; k < 35; k++)
            {
                sum += probabilityArray[char2Int(word[i])][char2Int(word[i+1])][k];
                if (sum >= random)
                {
                    word += int2Char(k);
                    break;
                }
            }
        }
        cout << word << endl;
    }

    cin.get();
    cin.get();
    cin.get();
    return 0;
}


//=======================================================================================================================================================
//=============================================== FUNCTIONS =============================================================================================
//=======================================================================================================================================================


bool makeProbabilityFile()
{
    ifstream file;
    file.open("input.txt");

    int nextLetterRepetitions [35][35][35];
    int firstTwoLettersRepetitions [35][35];
    for (int i = 0; i < 35; i++)
        for (int j = 0; j < 35; j++)
        {
            for (int k = 0; k < 35; k++)
                nextLetterRepetitions[i][j][k] = 0;
            firstTwoLettersRepetitions[i][j] = 0;
        }

    char letter;
    int letterNumber;
    int prevLetterNumber;
    int prevPrevLetterNumber;

    int twoLettersRepetitions = 0;

    if (file.good())
    {
        cout << "Plik zostal wczytany" << endl;
        if (getTwoLetters(file, prevPrevLetterNumber, prevLetterNumber) == true)
        {
            twoLettersRepetitions++;
            firstTwoLettersRepetitions[prevPrevLetterNumber][prevLetterNumber]++;
            while (file.get(letter))
            {
                if (letter != '\n')
                {
                    letterNumber = char2Int(letter);
                    nextLetterRepetitions[prevPrevLetterNumber][prevLetterNumber][letterNumber]++;
                    prevPrevLetterNumber = prevLetterNumber;
                    prevLetterNumber = letterNumber;
                }
                else
                {
                    if (getTwoLetters(file, prevPrevLetterNumber, prevLetterNumber) == false)
                        break;
                    else
                    {
                        firstTwoLettersRepetitions[prevPrevLetterNumber][prevLetterNumber]++;
                        twoLettersRepetitions++;
                    }
                }
            }
        }
        file.close();
    }
    else
    {
        cout << "Nie udalo sie wczytac pliku" << endl;
        return false;
    }


    fstream outFile;
    outFile.open("probability.txt");

    if (outFile.good())
    {
        for (int i = 0; i < 35; i++)
        {
            for (int j = 0; j < 35; j++)
            {
                int totalRepetitions = 0;
                for (int k = 0; k < 35; k++)
                    totalRepetitions += nextLetterRepetitions[i][j][k];
                for (int k = 0; k < 35; k++)
                {
                    if (totalRepetitions != 0)
                        outFile << (100 * float(nextLetterRepetitions[i][j][k])) / float(totalRepetitions) << " ";
                    else
                        outFile << 100.f / 35.f << " ";
                }

                outFile << endl;
            }
        }
        outFile << endl;

        for (int i = 0; i < 35; i++)
        {
            for (int j = 0; j < 35; j++)
            {
                outFile << (100 * float(firstTwoLettersRepetitions[i][j])) / float(twoLettersRepetitions) << endl;
            }
        }
        outFile.close();
        return true;
    }
    else
    {
        cout << "Nie udalo sie wczytac pliku" << endl;
        return false;
    }
}


bool readProbabilityFile(float tab[35][35][35], float tab2[35][35])
{
    ifstream file;
    file.open("probability.txt");
    if (file.good())
    {
        for (int i = 0; i < 35; i++)
            for (int j = 0; j < 35; j++)
                for (int k = 0; k < 35; k++)
                    file >> tab[i][j][k];

        for (int i = 0; i < 35; i++)
            for (int j = 0; j < 35; j++)
                file >> tab2[i][j];
        file.close();
        return true;
    }
    else
        return false;
}


int char2Int(char character)
{
    int charNumber = int(character);
    if (charNumber >= 97 && charNumber <= 122)
        charNumber = charNumber - 97;
    else
    {
        switch (charNumber)
        {
        case -91: // a
            charNumber = 26;
            break;
        case -122: // c
            charNumber = 27;
            break;
        case -87: // e
            charNumber = 28;
            break;
        case -120: // l
            charNumber = 29;
            break;
        case - 28: // n
            charNumber = 30;
            break;
        case -94: // o
            charNumber = 31;
            break;
        case -104: // s
            charNumber = 32;
            break;
        case -85: // z
            charNumber = 33;
            break;
        case -66: // z
            charNumber = 34;
            break;
        }
    }
    return charNumber;
}


char int2Char(int charNumber)
{
    char letter;
    if (charNumber >= 0 && charNumber <= 25)
        letter = charNumber + 97;
    else
    {
        switch (charNumber)
        {
        case 26: // a
            letter = -91;
            break;
        case 27: // c
            letter = -122;
            break;
        case 28: // e
            letter = -87;
            break;
        case 29: // l
            letter = -120;
            break;
        case 30: // n
            letter = -28;
            break;
        case 31: // o
            letter = -94;
            break;
        case 32: // s
            letter = -104;
            break;
        case 33: // z
            letter = -85;
            break;
        case 34: // z
            letter = -66;
            break;
        }
    }
    return letter;
}


bool getTwoLetters(ifstream& file, int& firstLetterNumber, int& secondLetterNumber)
{
    char letter;
    while (file.get(letter))
    {
        if (letter != '\n')
        {
            firstLetterNumber = char2Int(letter);
            if (file.get(letter))
            {
                if (letter != '\n')
                {
                    secondLetterNumber = char2Int(letter);
                    return true;
                }
            }
        }
    }
    return false;
}




