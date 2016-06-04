#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>

using namespace std;
bool readInputData(const string& inputFilePath, int& keysCount, map<unsigned int, short>& keys);
unsigned int calcPrivateKey(const unsigned int key, short key_length);

int main(int argc, char** argv)
{
    auto start = chrono::high_resolution_clock::now();

    string inputFilePath = "sigkey.in";
    string outputFilePath = "sigkey.out";

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    int keysCount = 0;
    map<unsigned int, short> keys;

    if (!readInputData(inputFilePath, keysCount, keys))
        return -1;

    int pairsCount = 0;

    auto it = keys.begin();
    while (it != keys.end())
    {
        unsigned int privateKey = calcPrivateKey(it->first, it->second);

        if (keys.find(privateKey) != keys.end())
        {
            pairsCount++;

            // remove private key
            keys.erase(privateKey);

            // remove public key
            auto publicKey = it;
            ++it;
            keys.erase(publicKey);
        }
        else
            ++it;
    }

    // TODO : reimplement on C++ style
    FILE *outputFile = fopen(outputFilePath.c_str(), "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", pairsCount);

        fclose(outputFile);
        outputFile = NULL;
    }

    auto end = chrono::high_resolution_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;

    return 1;
}

bool readInputData(const string& inputFilePath, int& keysCount, map<unsigned int, short>& keys)
{
    ifstream inputFile(inputFilePath);

    if (inputFile.is_open())
    {
        string keysCountStr;
        getline(inputFile, keysCountStr);
        
        istringstream iss(keysCountStr);
        iss >> keysCount;

        string str;
        for (int i = 0; i < keysCount; ++i)
        {
            getline(inputFile, str);

            // represent each key as sequence of bits
            unsigned int bitKey = 0;
            short maxBitPosition = 0;

            for (int i = 0; i < str.size(); ++i)
            {
                // ASCII code 'a' == 97, so ASCII(char) - 97 tell us bit position
                int bitPosition = int(str[i]) - 97;
                if (bitPosition > maxBitPosition) maxBitPosition = bitPosition;
                bitKey |= (1 << bitPosition);
            }

            keys[bitKey] = (short) maxBitPosition;
        }

        return true;
    }
    else
    {
        cout << "Can't open file" << endl;
        return false;
    }
}

unsigned int calcPrivateKey(const unsigned int key, short key_length)
{
    unsigned int privateKey = ~key;

    unsigned int bitMask = 0;
    for (unsigned int i = 0; i < key_length; ++i)
        bitMask |= 1 << i;

    // 
    privateKey &= bitMask;

    return privateKey;
}
