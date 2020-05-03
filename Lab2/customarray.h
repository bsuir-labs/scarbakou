#ifndef CUSTOMARRAY_H
#define CUSTOMARRAY_H


class CustomArray
{

    int *m_data;

    unsigned m_data_size;
    unsigned m_size;

    void expand();
    void shrink();

    static unsigned nearestPower(unsigned x);
    static void sort(int* arr, int left, int right);

public:
    CustomArray();
    CustomArray(unsigned size);
    ~CustomArray();

    unsigned size() const;
    bool isEmpty() const;

    void push_back(int element);
    void pop_back();
    void clear();

    void resize(unsigned size);
    void sort();
    void insertKeepSorted(int element);

    static CustomArray Unite(const CustomArray &arrl, const CustomArray &arrr);
    static CustomArray Cross(const CustomArray &arrl, const CustomArray &arrr);

    int& operator[](unsigned x);
    const int& operator[](unsigned x) const;
};

#endif // CUSTOMARRAY_H
