#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <ctime>

int calculateMaxHamsterUsage(int dailyRate, int avarice, int hamsterCount);
void quickSort(unsigned int* arr, long size);

int main(int ac, char** av)
{
    const char* inputFileName = NULL;
    const char* outputFileName = NULL;

    // read input arguments
    if(ac > 1)
    {
        // file name had been passed via command line
        inputFileName = av[1];
        outputFileName = av[2];
    }
    else
    {
        // use default file name
        inputFileName = "hamstr.in";
        outputFileName = "hamstr.out";
    }

    FILE *inputFile = fopen(inputFileName, "r");
    unsigned int dailyLimit, hamsterCount = 0;
    unsigned int *dailyRate = NULL;
    unsigned int *avarice = NULL;
    unsigned int *maxHamsterUsage = NULL;

    if (inputFile)
    {
        fscanf(inputFile, "%u", &dailyLimit);
        fscanf(inputFile, "%u", &hamsterCount);

        dailyRate = (unsigned int*) malloc(sizeof(int) * hamsterCount);
        avarice = (unsigned int*) malloc(sizeof(int) * hamsterCount);
        maxHamsterUsage = (unsigned int*) malloc(sizeof(int) * hamsterCount);
    }

    // read food usage and avarice values
    for (int i = 0; i < hamsterCount; ++i)
        fscanf(inputFile, "%u %u", dailyRate + i, avarice + i);

    int maxCount = 0;
    int chosenHamsterCount = round(hamsterCount / 2.0);
    int first = 0;
    int last = hamsterCount;

    do
    {
        // calcualte max food usage for each hamster
        for (int j = 0; j < hamsterCount; ++j)
            maxHamsterUsage[j] = calculateMaxHamsterUsage(dailyRate[j], avarice[j], chosenHamsterCount - 1);

        unsigned int sumResult = 0;
        quickSort(maxHamsterUsage, hamsterCount);

        // sum hamsters which eat the least food
        for (int z = 0; z < chosenHamsterCount && sumResult <= dailyLimit; ++z)
            sumResult += maxHamsterUsage[z];

        if (sumResult <= dailyLimit)
        {
            maxCount = chosenHamsterCount;
            if (chosenHamsterCount == hamsterCount) break;

            first = chosenHamsterCount;
            chosenHamsterCount = round((first + last) / 2.0);
        }
        else
        {
            if (last - first == 1) break;
            last = chosenHamsterCount;
            chosenHamsterCount = round((first + last) / 2.0);
        }
    } while (true);

    // save result to file
    FILE *outputFile = fopen(outputFileName, "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", maxCount);
        
        fclose(outputFile);
        outputFile = NULL;
    }

    return 0;
}

int calculateMaxHamsterUsage(int dailyRate, int avarice, int hamsterCount)
{
    return dailyRate + hamsterCount * avarice;
}

void quickSort(unsigned int* arr, long size)
{
  long i = 0, j = size-1;
  int temp, pivot;

  pivot = arr[ size>>1 ];

  do {
    while ( arr[i] < pivot ) i++;
    while ( arr[j] > pivot ) j--;

    if (i <= j) {
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++; j--;
    }
  } while ( i<=j );

  if ( j > 0 ) quickSort(arr, j);
  if ( size > i ) quickSort(arr+i, size-i);
}