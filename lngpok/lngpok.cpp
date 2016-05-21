#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct CardsOnHands
{
    std::vector<unsigned int> mCardSequence;
    unsigned int mJokerCounts = 0;
};

//
int partition(std::vector<unsigned int>& data, int left, int right);
void quickSort(std::vector<unsigned int>& data, int left, int right);

//
bool readCardSequence(const string& inputFilePath, CardsOnHands& cardsInfo);
int findSequenceLength(int startPoint, CardsOnHands& cardsInfo);

int main(int argc, char** argv)
{
    string inputFilePath = "lngpok.in";
    string outputFilePath = "lngpok.out";
    CardsOnHands mCardsInfo;

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    // read sequence from file
    readCardSequence(inputFilePath, mCardsInfo);
    quickSort(mCardsInfo.mCardSequence, 0, mCardsInfo.mCardSequence.size() - 1);

    // iterate over all available sequence and find max
    int maxSeqLength = (mCardsInfo.mCardSequence.size() == 0) ? mCardsInfo.mJokerCounts : 0;
    for (int i = 0; i < mCardsInfo.mCardSequence.size(); ++i)
    {
        int seqLength = findSequenceLength(i, mCardsInfo);
        if (seqLength > maxSeqLength)
            maxSeqLength = seqLength;
    }

    // TODO : reimplement on C++ style
    FILE *outputFile = fopen(outputFilePath.c_str(), "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", maxSeqLength);

        fclose(outputFile);
        outputFile = NULL;
    }

    return 1;
}

bool readCardSequence(const string& inputFilePath, CardsOnHands& cardsInfo)
{
    ifstream inputFile(inputFilePath);
    if (inputFile.is_open())
    {
        string cardSequenceStr;
        getline(inputFile, cardSequenceStr);
        
        istringstream iss(cardSequenceStr);
        unsigned int card;

        while (iss >> card)
        {
            if (card == 0)
                cardsInfo.mJokerCounts++;
            else
            {
                auto it = find(cardsInfo.mCardSequence.begin(), cardsInfo.mCardSequence.end(), card);
                if(it == cardsInfo.mCardSequence.end())
                    cardsInfo.mCardSequence.push_back(card);
            }
        }

        inputFile.close();
        return true;
    }
    else
    {
        cout << "Can't open file" << endl;
        return false;
    }
}

int findSequenceLength(int startPoint, CardsOnHands& cardsInfo)
{
    int seqLength = 1; // start point card initial length 
    int availableJokers = cardsInfo.mJokerCounts;
    int lastCard = cardsInfo.mCardSequence[startPoint];

    for (int i = startPoint + 1; i < cardsInfo.mCardSequence.size(); ++i)
    {
        int nextCard = cardsInfo.mCardSequence[i];
        int gapSize = (nextCard - lastCard) - 1; // (3 - 2) - 1 = 0; (4 - 2) - 1 = 1;
        
        if (gapSize > 0)
        {
            
            if (availableJokers >= gapSize)
            {
                // fill the gap by jokers
                seqLength += gapSize;
                availableJokers -= gapSize;
            }
            else
            {
                // append jokers at the begin of gap
                seqLength += availableJokers;
                availableJokers = 0;

                break; // we don't have enough jokers and can't cover our gap
            }
        }
        
        lastCard = nextCard;
        seqLength++;
    }

    return seqLength + availableJokers;
}

int partition(std::vector<unsigned int>& data, int left, int right)
{
    int pivotPos = left;
    int pivotValue = data[pivotPos];
    int tmpLeft = left + 1;
    int tmpRight = right;

    do
    {
        // looking for element in left part that must be swapped
        while (tmpLeft <= right && data[tmpLeft] <= pivotValue) ++tmpLeft;

        // looking for element in right part that must be swapped
        while (tmpRight >= left && data[tmpRight] > pivotValue) --tmpRight;

        if (tmpLeft < tmpRight)
        {
            std::swap(data[tmpLeft], data[tmpRight]);
            tmpLeft++;
            tmpRight--;
        }
    } while (tmpLeft <= tmpRight);

    // swap pivot with first element in right part
    std::swap(data[pivotPos], data[tmpRight]);
    
    return tmpRight;
}
void quickSort(std::vector<unsigned int>& data, int left, int right)
{
    if (right <= left)
        return;

    int pivotPos = partition(data, left, right);

    quickSort(data, left, pivotPos - 1);
    quickSort(data, pivotPos + 1, right);
}
