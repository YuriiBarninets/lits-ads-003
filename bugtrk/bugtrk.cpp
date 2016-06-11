#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;
bool canConsist(uint64_t sequereSide, uint64_t n, uint64_t w, uint64_t h);

int main(int argc, char** argv)
{
    auto start = chrono::high_resolution_clock::now();

    string inputFilePath = "bugtrk.in";
    string outputFilePath = "bugtrk.out";

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    // read input data
    uint64_t N = 0, W = 0, H = 0;
    std::string inputData;

    ifstream inputFile(inputFilePath);
    if (inputFile.is_open())
    {
        inputFile >> N >> W >> H;
        inputFile.close();
    }
    else
    {
        cout << "Can't open file" << endl;
        return false;
    }


    // perform binary search of min square
    uint64_t left = 1;
    uint64_t right = N * std::max(W, H);
    uint64_t mid, minSquare = 0;
    
    while (right - left > 1)
    {
        mid = (uint64_t) (left + right) / 2;
        bool res = canConsist(mid, N, W, H);

        if (res)
            right = minSquare = mid;
        else
            left = mid;
    }

    // TODO : reimplement on C++ style
    FILE *outputFile = fopen(outputFilePath.c_str(), "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", minSquare);

        fclose(outputFile);
        outputFile = NULL;
    }

    return 1;
}

bool canConsist(uint64_t sequereSide, uint64_t n, uint64_t w, uint64_t h)
{
    uint64_t maxCountOnWidth = (uint64_t) sequereSide / w;
    uint64_t maxCountOnHeight = (uint64_t) sequereSide / h;
    return (maxCountOnHeight * maxCountOnWidth) >= n;
}