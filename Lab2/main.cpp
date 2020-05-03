#include <iostream>
#include "customarray.h"

using namespace std;


void printArr(const std::string &name, const CustomArray &arr)
{
    cout << name << ": ";
    for (unsigned i = 0; i < arr.size(); ++i)
        cout << arr[i] << " ";
    cout << endl;
}

#define printArr(x) \
    printArr(#x, x)


int main()
{
    srand(time(nullptr));

    // Example of array resizing
    CustomArray array;
    array.resize(10);
    printArr(array);


    // Exmaple of elements value assigning
    for (unsigned i = 0; i < array.size(); ++i)
        array[i] = i * 2;
    printArr(array);


    // Example of array resizing
    array.resize(14);
    printArr(array);
    array.resize(4);
    printArr(array);

    // Example of array sorting
    array.resize(20);
    printArr(array);
    for (unsigned i = 0; i < array.size(); ++i)
        array[i] = rand() % 50 - 20;
    printArr(array);

    array.sort();
    printArr(array);


    // Example of inserting element into sorted array
    array.insertKeepSorted(12);
    printArr(array);


    // Example of merging sorted arrays
    CustomArray second(20);
    for (unsigned i = 0; i < second.size(); ++i)
        second[i] = rand() % 50 - 20;
    printArr(second);

    second.sort();

    printArr(second);

    CustomArray merged = CustomArray::Unite(array, second);

    printArr(merged);


    // Example of crossing sorted arrays
    CustomArray crossed = CustomArray::Cross(array, second);
    printArr(crossed);

    // Example of searching in sorted array
    for (unsigned i = 0; i < 40; ++i)
    {
        int el = rand() % 50 - 20;
        cout << "Index of " << el << ": " << merged.indexOf(el) << endl;
    }

    return 0;
}
