#include <stdio.h>
#include <cstdlib>
#include <math.h>

// use selection sort
void substructDiscount(long long* products, double* priceWithoutDiscount, int productCount, double discountPercent)
{
    int max_element_pos;
    int elementsWithDiscount = (productCount / 3); // we need discount only for third part
    int tmp;

    for (int i = 0; i < elementsWithDiscount; i++)
    {
        max_element_pos = i;

        // looking for max element
        for (int j = i + 1; j < productCount; j++)
        {
            if(products[max_element_pos] < products[j])
                max_element_pos = j;
        }

        // substructure discount
        *priceWithoutDiscount -= (double) products[max_element_pos] * discountPercent;
        
        // in our case no reason to do swap and waste resources
        tmp = products[max_element_pos];
        products[max_element_pos] = products[i];
        products[i] = tmp;
    }

    return;
}

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
        inputFileName = "discnt.in";
        outputFileName = "discnt.out";
    }

    long long *inputData = (long long*) malloc(sizeof(long long)*10001); // the last element contains discount percent
    int productCount = 0;
    double priceWithoutDiscount = 0;

    // read input data into array
    FILE *inputFile = fopen(inputFileName, "r");
    if(inputFile)
    {
        while(fscanf(inputFile, "%lld ", inputData + productCount) > 0) // parse %d followed by ','
        {
            priceWithoutDiscount += inputData[productCount];
            productCount++;
        }
        //
        productCount--;

        // int array the last element is discount percent
        priceWithoutDiscount -= inputData[productCount];

        fclose(inputFile);
        inputFile = NULL;
    }

    // substructure discount size
    if(inputData[productCount] != 0) // no reason to do calculation if discount == 0 %
        substructDiscount(inputData, &priceWithoutDiscount, productCount, inputData[productCount] / 100.0);

    // write result into file
    FILE *outputFile = fopen(outputFileName, "w");
    if(outputFile)
    {
        // save to file
        fprintf (outputFile, "%.2f", floor(100 * priceWithoutDiscount) / 100);
        
        fclose(outputFile);
        outputFile = NULL;
    }

    return 0;
}
