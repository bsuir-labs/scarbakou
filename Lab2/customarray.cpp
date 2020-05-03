#include "customarray.h"
#include <cmath>

CustomArray::CustomArray()
    : m_data(nullptr)
    , m_data_size(0)
    , m_size(0)
{}

CustomArray::CustomArray(unsigned size)
{
    m_data_size = nearestPower(size);
    m_data = new int[m_data_size];
    m_size = size;
}

CustomArray::~CustomArray()
{
    if (m_data)
        delete[] m_data;
}

unsigned CustomArray::size() const
{
    return m_size;
}

bool CustomArray::isEmpty() const
{
    return m_size > 0;
}

void CustomArray::push_back(int element)
{
    if (m_size == m_data_size) expand();
    m_data[m_size++] = element;
}

void CustomArray::pop_back()
{
    m_size--;
    if (nearestPower(m_size) < m_data_size) shrink();
}

void CustomArray::clear()
{
    m_data_size = 0;
    m_size = 0;
    if (m_data) delete[] m_data;
    m_data = nullptr;
}

void CustomArray::resize(unsigned int size)
{
    while (m_data_size < size) expand();
    while (m_data_size > nearestPower(size)) shrink();
    m_size = size;
}

void CustomArray::sort()
{
    sort(m_data, 0, m_size - 1);
}

void CustomArray::insertKeepSorted(int element)
{
    resize(m_size + 1);
    int index = m_size - 1;
    for (; index > 0 && m_data[index - 1] > element; --index)
        m_data[index] = m_data[index - 1];
    m_data[index] = element;
}

int CustomArray::indexOf(int element)
{
    int l = 0, r = m_size - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (element > m_data[mid])
            l = mid + 1;
        else
            r = mid;
    }
    if (m_data[l] == element) return l;
    return -1;
}

void CustomArray::expand()
{
    if (m_data_size == 0)
        m_data_size = 1 << 3;
    else
        m_data_size <<= 1;

    int *tmp = new int[m_data_size];
    if (m_data)
    {
        for (unsigned i = 0; i < m_size; ++i)
            tmp[i] = m_data[i];
        delete[] m_data;
    }
    m_data = tmp;
}

void CustomArray::shrink()
{
    m_data_size >>= 1;
    int *tmp = nullptr;
    if (m_data_size > 0)
    {
        tmp = new int[m_data_size];
        for (unsigned i = 0; i < m_data_size; ++i)
            tmp[i] = m_data[i];
    }
    if (m_data)
        delete[] m_data;
    m_data = tmp;
}

unsigned CustomArray::nearestPower(unsigned x)
{
    return (1 << (int)(log2(1.0*(x - 1)) + 1));
}

void CustomArray::sort(int *arr, int left, int right)
{
    int l = left, r = right;
    int mid = arr[(l + r) / 2];
    do {
        while (arr[l] < mid) ++l;
        while (arr[r] > mid) --r;
        if (l <= r)
        {
            int tmp = arr[l];
            arr[l] = arr[r];
            arr[r] = tmp;
            ++l;
            --r;
        }
    } while (l <= r);
    if (left < r) sort(arr, left, r);
    if (l < right) sort(arr, l, right);
}

int& CustomArray::operator[](unsigned int x)
{
    return m_data[x];
}

const int& CustomArray::operator[](unsigned int x) const
{
    return m_data[x];
}

CustomArray CustomArray::Unite(const CustomArray &arrl, const CustomArray &arrr)
{
    CustomArray newArray(arrl.size() + arrr.size());
    unsigned i = 0, j = 0, size = 0;
    while (i < arrl.size() && j < arrr.size())
        if (arrl[i] < arrr[j])
            newArray[size++] = arrl[i++];
        else
            newArray[size++] = arrr[j++];
    while (i < arrl.size()) newArray[size++] = arrl[i++];
    while (j < arrr.size()) newArray[size++] = arrr[j++];
    return newArray;
}

CustomArray CustomArray::Cross(const CustomArray &arrl, const CustomArray &arrr)
{
    CustomArray newArray;
    unsigned i = 0, j = 0;
    while (i < arrl.size() && j < arrr.size())
        if (arrl[i] < arrr[j])
            ++i;
        else if (arrr[j] < arrl[i])
            ++j;
        else
        {
            newArray.push_back(arrl[i]);
            ++i, ++j;
        }
    return newArray;
}
