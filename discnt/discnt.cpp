#include <stdio.h>
#include <cstdlib>
#include <math.h>

// use selection sort that effective because we can sort only productCount / 3 elements
void substructDiscountBySelection(long long* products, double* priceWithoutDiscount, int productCount, double discountPercent)
{
    int max_element_pos;
    int elementsWithDiscount = (productCount / 3); // we need discount only for third part

    // use selection sort
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
        
        // let's do swap :)
        products[max_element_pos] ^= products[i];
        products[i] ^= products[max_element_pos];
        products[max_element_pos] ^= products[i];
    }

    return;
}

// use insertion sort that effective when sequence is almost sorted
void substructDiscountByInsertion(long long* products, double* priceWithoutDiscount, int productCount, double discountPercent)
{
    int current_pos;
    for (int i = 0; i < productCount; i++)
    {
        current_pos = i;

        while (current_pos > 0 && products[current_pos - 1] < products[current_pos])
        {
            // swap :)
            products[current_pos] ^= products[current_pos - 1];
            products[current_pos - 1] ^= products[current_pos];
            products[current_pos] ^= products[current_pos - 1];

            current_pos -= 1;
        }
    }

    // substruct discount
    int productWithDiscount = productCount / 3; // we don't want to do division each iteration
    for (int i = 0; i < productWithDiscount; i++)
    {
        *priceWithoutDiscount -= products[i] * discountPercent;
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

    long long *inputData = (long long*) malloc(sizeof(long long) * 10001); // the last element contains discount percent
    int productCount = 0;
    double priceWithoutDiscount = 0;
    int complexity = 0;

    // read input data into array
    FILE *inputFile = fopen(inputFileName, "r");
    if(inputFile)
    {
        while(fscanf(inputFile, "%lld ", inputData + productCount) > 0) // parse %d followed by ','
        {
            priceWithoutDiscount += inputData[productCount];
            complexity ^= inputData[productCount] ^ productCount;
            productCount++;
        }
        //
        productCount--;

        // in the array the last element is discount percent
        priceWithoutDiscount -= inputData[productCount];

        fclose(inputFile);
        inputFile = NULL;
    }

    // substructure discount size
    if (inputData[productCount] != 0) // no reason to do calculation if discount == 0 %
    {
        if(complexity > productCount * 2)
            substructDiscountBySelection(inputData, &priceWithoutDiscount, productCount, inputData[productCount] / 100.0);
        else
            substructDiscountByInsertion(inputData, &priceWithoutDiscount, productCount, inputData[productCount] / 100.0);
    }
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
