#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
    string inputFilePath = "bugtrk.in";
    string outputFilePath = "bugtrk.out";

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    // read data from file
    ifstream inputFile(inputFilePath);
    if (inputFile.is_open())
    {
        // inputFile >> data
        inputFile.close();
    }
    else
    {
        cout << "Can't open file : " << inputFilePath << endl;
        return -1;
    }

    // write data to file
    ofstream outputFile(outputFilePath, ifstream::out);
    if (outputFile.is_open())
    {
        // outputFile << res;
        outputFile.close();
    }
    else
    {
        cout << "Can't open file : " << outputFilePath << endl;

    }

    return 1;
}