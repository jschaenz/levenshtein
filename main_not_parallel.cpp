#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <cassert>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;

struct Cards
{
    string name;
    string type;
    string mana;
    int cmc;
    int count;
};

void readCardsFromScrambled(list<Cards> &listOfScrambledCards);                         //reads cards from scrambled.txt into the list of scrambled cards
void readCardsFromReference(list<Cards> &listOfReferenceCards);                         //reads cards from reference.txt into the list of reference cards
void cardsToFile(list<Cards> &listOfCards);                                             //writes passed list into a file
void restoreName(list<Cards> &listOfScrambledCards, list<Cards> &listOfReferenceCards); //restores (tries to) the card name
int calculate(string originalName, string referenceName);                               // Calculates the number of operations, to convert a string A to a string B, returns: number of operations required

int main(int argc, char const *argv[]) //main
{
    /*
    parts of the time calculation taken from: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/ 29.11 19:45
    */
    auto start = chrono::high_resolution_clock::now();
    list<Cards> scrambledCards;
    list<Cards> referenceCards;

    cout << "start reading from scrambled.txt ..." << endl;
    readCardsFromScrambled(scrambledCards);

    cout << "start reading from reference.txt ..." << endl;
    readCardsFromReference(referenceCards);

    cout << "start restore process ..." << endl;
    restoreName(scrambledCards, referenceCards);

    cout << "writing restored cards to File ..." << endl;
    cardsToFile(scrambledCards);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Duration: " << duration.count() << "ms" << endl;
    return 0;
}

void readCardsFromScrambled(list<Cards> &listOfScrambledCards) //reads cards from scrambled.txt into the list of scrambled cards
{
    assert(listOfScrambledCards.size() == 0);

    string content;
    string innerContent;
    Cards card;
    vector<string> tokens;
    ifstream scrambled;
    stringstream s;
    scrambled.open("scrambled.txt");

    if (scrambled)
    {
        while (getline(scrambled, content)) //loop through scrambled.txt one line at a time
        {
            s.str(content);
            while (getline(s, innerContent, '|'))
            {
                tokens.push_back(innerContent);
            }

            card.name = tokens[0];
            card.mana = tokens[1];
            istringstream(tokens[2]) >> card.cmc;
            card.type = tokens[3];
            istringstream(tokens[4]) >> card.count;

            listOfScrambledCards.push_back(card);
            s.clear();
            tokens.clear();
        }
    }
    scrambled.close();

    assert(!scrambled);
}

void readCardsFromReference(list<Cards> &listOfReferenceCards) //reads cards from reference.txt into the list of reference cards
{
    assert(listOfReferenceCards.size() == 0);

    string content;
    ifstream reference;
    Cards card;

    reference.open("reference.txt");

    if (reference)
    {
        while (getline(reference, content)) //reads reference line for line
        {
            card.name = content;
            listOfReferenceCards.push_back(card);
        }
    }

    reference.close();

    assert(!reference);
}

void cardsToFile(list<Cards> &listOfCards) //writes passed list into a file
{
    assert(listOfCards.size() > 0);

    ofstream repairedCards("repairedCards.txt");

    //www.geeksforgeeks.org/list-cpp-stl/, 28.11 18:52
    list<Cards>::iterator it;

    for (it = listOfCards.begin(); it != listOfCards.end(); ++it)
    { //Name | {Mana} | Cmc | Type | Amount
        repairedCards << it->name << "|" << it->mana << "|" << it->cmc << "|" << it->type << "|" << it->count << endl;
    }
}

void restoreName(list<Cards> &listOfScrambledCards, list<Cards> &listOfReferenceCards) //(tries to) restore the card name
{
    assert(listOfReferenceCards.size() > 0);
    assert(listOfScrambledCards.size() > 0);

    list<Cards>::iterator it;
    list<Cards>::iterator it2;

    float cardNameLength;
    for (it = listOfScrambledCards.begin(); it != listOfScrambledCards.end(); ++it)
    {
        for (it2 = listOfReferenceCards.begin(); it2 != listOfReferenceCards.end(); ++it2)
        {
            cardNameLength = it->name.size();
            if (calculate(it->name, it2->name) < (31 * (cardNameLength / 100))) //if LD < 31% of name length
            {
                it->name = it2->name;
            }
        }
    }
}

int calculate(string originalName, string referenceName)
{ /* Calculates the number of operations, to convert a string A to a string B
    returns: number of operations required*/
    assert(originalName.length() > 0);
    assert(referenceName.length() > 0);
    int minCosts;
    int sameChar;
    int matrix[originalName.length() + 1][referenceName.length() + 1];

    for (int i = 0; i <= originalName.length(); i++)
    {
        matrix[i][0] = i;
    }
    for (int i = 0; i <= referenceName.length(); i++)
    {
        matrix[0][i] = i;
    }

    for (int row = 1; row <= originalName.length(); row++)
    {
        for (int column = 1; column <= referenceName.length(); column++)
        {
            sameChar = 0;

            if (originalName.at(row - 1) != referenceName.at(column - 1))
            {
                sameChar = 1;
            }
            minCosts = matrix[row - 1][column] + 1;

            if (matrix[row][column - 1] + 1 < minCosts)
            {
                minCosts = matrix[row][column - 1] + 1;
            }

            if (matrix[row - 1][column - 1] + sameChar < minCosts)
            {
                minCosts = matrix[row - 1][column - 1] + sameChar;
            }
            matrix[row][column] = minCosts;
        }
    }
    //assert(matrix[originalName.length()][referenceName.length()] != NULL);
    return matrix[originalName.length()][referenceName.length()];
}