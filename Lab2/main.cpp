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
    CustomArray array;

    array.resize(10);
    printArr(array);

    for (unsigned i = 0; i < array.size(); ++i)
        array[i] = i * 2;

    printArr(array);

    array.resize(14);
    printArr(array);
    array.resize(4);
    printArr(array);
    array.resize(20);
    printArr(array);

    for (unsigned i = 0; i < array.size(); ++i)
        array[i] = rand() % 50 - 20;

    printArr(array);

    array.sort();

    printArr(array);

    array.insertKeepSorted(12);

    printArr(array);

    CustomArray second(20);
    for (unsigned i = 0; i < second.size(); ++i)
        second[i] = rand() % 50 - 20;

    printArr(second);

    second.sort();

    printArr(second);

    CustomArray merged = CustomArray::Unite(array, second);

    printArr(merged);

    CustomArray crossed = CustomArray::Cross(array, second);

    printArr(crossed);

    for (unsigned i = 0; i < 40; ++i)
    {
        int el = rand() % 50 - 20;
        cout << "Index of " << el << ": " << merged.indexOf(el) << endl;
    }

    return 0;
}
