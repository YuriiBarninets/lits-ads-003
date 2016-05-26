#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

class BinaryHeap
{
public:
    BinaryHeap(int heapSize)
        : mItems(new int[heapSize])
        , mHeapSize(heapSize)
        , mItemsCount(0)
    { }
    ~BinaryHeap() { delete[] mItems; }

    void add(int value)
    {
        mItems[mItemsCount] = value;
        mItemsCount++;

        // find parent index
        int childIndex = mItemsCount - 1;
        int parentIndex = (childIndex - 1) / 2;
        
        // bubble up inserted element
        while (parentIndex >= 0 && mItems[parentIndex] < mItems[childIndex])
        {
            int temp = mItems[parentIndex];
            mItems[parentIndex] = mItems[childIndex];
            mItems[childIndex] = temp;

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    // build binary heap for O(N)
    void buildHeap(const std::string& word)
    {
        for (int i = 0; i < word.size(); ++i)
            mItems[i] = (int) word[i];
        
        mItemsCount = mHeapSize;

        for (int i = mHeapSize / 2; i >= 0; --i)
            heapify(i);
    }
    
    int fetchMax()
    {
        int res = mItems[0];
        mItems[0] = mItems[mItemsCount - 1];
        mItemsCount--;

        heapify(0);

        return res;
    }

    void heapify(int rootIndex)
    {
        int leftChild;
        int rightChild;
        int largestChild;

        for ( ; ; )
        {
            leftChild = 2 * rootIndex + 1;
            rightChild = 2 * rootIndex + 2;
            largestChild = rootIndex;

            if (leftChild < mItemsCount && mItems[leftChild] > mItems[largestChild]) 
            {
                largestChild = leftChild;
            }

            if (rightChild < mItemsCount && mItems[rightChild] > mItems[largestChild])
            {
                largestChild = rightChild;
            }

            if (largestChild == rootIndex) 
                break;

            int temp = mItems[rootIndex];
            mItems[rootIndex] = mItems[largestChild];
            mItems[largestChild] = temp;
            rootIndex = largestChild;
        }
    }

    int getItemsCount() { return mItemsCount; }

private:
    int*    mItems;
    int     mHeapSize;
    int     mItemsCount;
};

bool readInputData(const string& inputFilePath, int& keysCount, vector<string>& keys);
bool isPair(const string& key1, const string& key2);
bool obviouslyNotPair(const string& key1, const string& key2);

int main(int argc, char** argv)
{
    // auto start = chrono::high_resolution_clock::now();

    string inputFilePath = "sigkey.in";
    string outputFilePath = "sigkey.out";

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    int keysCount = 0;
    vector<string> keys;

    if (!readInputData(inputFilePath, keysCount, keys)) return -1;

    int pairsCount = 0;

    for(int leftPos = 0; leftPos < keys.size() - 1; ++leftPos)
        for(int rightPos = leftPos + 1; rightPos < keys.size(); ++rightPos)
        {
            if (isPair(keys[leftPos], keys[rightPos]))
            {
                keys[leftPos].clear();
                keys[rightPos].clear();
                pairsCount++;

                break;
            }
        }

    // TODO : reimplement on C++ style
    FILE *outputFile = fopen(outputFilePath.c_str(), "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", pairsCount);

        fclose(outputFile);
        outputFile = NULL;
    }

    /*
    auto end = chrono::high_resolution_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    */

    return 1;
}

bool readInputData(const string& inputFilePath, int& keysCount, vector<string>& keys)
{
    ifstream inputFile(inputFilePath);

    if (inputFile.is_open())
    {
        string keysCountStr;
        getline(inputFile, keysCountStr);
        
        istringstream iss(keysCountStr);
        iss >> keysCount;
        keys.reserve(keysCount);

        string str;
        for (int i = 0; i < keysCount; ++i)
        {
            getline(inputFile, str);
            keys.push_back(str);
        }

        return true;
    }
    else
    {
        cout << "Can't open file" << endl;
        return false;
    }
}

bool isPair(const string& key1, const string& key2)
{
    string keyPair = key1 + key2;
    if (obviouslyNotPair(key1, key2)) return false;

    BinaryHeap heap(keyPair.length());
    heap.buildHeap(keyPair);

    int first, second;
    bool isPair = true;

    while (heap.getItemsCount() > 1)
    {
        int first = heap.fetchMax();
        int second = heap.fetchMax();

        if (abs(first - second) != 1)
        {
            isPair = false;
            break;
        }
    }

    return isPair;
}

bool obviouslyNotPair(const string& key1, const string& key2)
{
    string keyPair = key1 + key2;
    if (    keyPair.find('a') == string::npos
        ||  key1.empty() 
        ||  key2.empty()
        ) return true;

    for (int i = 0; i < key2.size(); ++i)
        if (key1.find(key2[i]) != string::npos)
            return true;

    return false;
}
