#include "customarray.h"
#include <cmath>

// Конструктор без аргументов
// Вызовется, если напишем, например
// CustomArray a;
CustomArray::CustomArray()
    : m_data(nullptr)       // память выделять не будем, укажем в 0
    , m_data_size(0)        // оба размер тоже по умолчанию 0
    , m_size(0)
{}                          // Больше делать не надо ничего


// Конструктор копирования
// Вызывается, если сделаем, например
// CustomArray a(b);
CustomArray::CustomArray(const CustomArray & arr)
    : CustomArray()         // Сначала вызовем дефолтный конструктор
{
    *this = arr;            // А потом просто вызовем оператор =. Его определение написано ниже.
}

// Конструктор с размером
// Чтоб создать сразу массив определённого размера
CustomArray::CustomArray(unsigned size)
{
    m_data_size = nearestPower(size);   // Найдём ближайшую сверху степень двойки к размеру
    m_data = new int[m_data_size];      // Выделим себе столько памяти
    m_size = size;                      // И пометим размер для пользователя, как он и попросил
}

// Деструктор
CustomArray::~CustomArray()
{
    if (m_data)                         // Когда объект удаляется, нам надо обязательно освободить память
        delete[] m_data;                // Иначе будет утечка. Поэтому делаем это в деструкторе
}

// Метод, возвращающий размер
// То же самое, что std::vector::size().
unsigned CustomArray::size() const
{
    return m_size;          // ничего особенного, просто размер возвращаем
}

// Проверка на пустоту
bool CustomArray::isEmpty() const
{
    return m_size > 0;          // Тоже вроде ничего сложного. Вернём true, если в массиве что-то есть. Иначе фолс.
}


// Добавление элемента в конец массива
void CustomArray::push_back(int element)
{
    if (m_size == m_data_size) expand();        // Если нам не хватает места в массиве, то придётся выделить ещё памяти
    m_data[m_size++] = element;                 // И добавляем элемент в конец, увеличивая "пользовательский" размер
}

// Удаление элемента с конца массива
void CustomArray::pop_back()
{
    m_size--;                                   // Уменьшаем размер
    if (nearestPower(m_size) < m_data_size) shrink();   // И если можем, то ужимаем размер реальной памяти.
                                                // Напомню, что реальный размер памяти у нас это всегда степень двойки
}

// Очистка массива
void CustomArray::clear()
{
    m_data_size = 0;                    // Размеры в ноль
    m_size = 0;
    if (m_data) delete[] m_data;        // Удаляем данные
    m_data = nullptr;
}

// Изменение размера в нужную сторону
void CustomArray::resize(unsigned int size)
{
    while (m_data_size < size) expand();                // Если нужно увеличить, то расширяемся, пока не будет достаточно
    while (m_data_size > nearestPower(size)) shrink();  // если нужно уменьшить, то сжимаемся
    m_size = size;                                      // и устанавливаем размер, как попросили
}

// Сортировка массива
void CustomArray::sort()
{
    sort(m_data, 0, m_size - 1);            // Вызываем QSort на наших данных
}


// Вставка элемента в отсортированный массив
void CustomArray::insertKeepSorted(int element)
{
    resize(m_size + 1);                                         // Увеличим размер массива на 1
    int index = m_size - 1;                                     // Будем идти с конца до тех пор, пока
    for (; index > 0 && m_data[index - 1] > element; --index)   // текущий рассматриваемый элемент больше нового
        m_data[index] = m_data[index - 1];                      // и просто сдвигаем эту часть массива на 1 вправо (в конец)
    m_data[index] = element;                                    // Когда остановимся (дошли до начала либо до последнего
                                                                // элемента, большего нового), можем вставить наш новый элемент
}

// Бинарный поиск в массиве
int CustomArray::indexOf(int element)
{
    // Ну тут просто бинарный поиск. В методе у вас есть описание, можно на википедии ещё посмотреть
    // Напомню просто, что идея в том, чтобы делить массив на две части каждую итерацию
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

// Расширение массива
void CustomArray::expand()
{
    if (m_data_size == 0)               // Если у нас размер был 0
        m_data_size = 1 << 3;           // поставим сразу 2^3, т.е. 8.
    else
        m_data_size <<= 1;              // Иначе просто умножим размер на 2

    // оператор << - это побитовый сдвиг влево.
    // Арифметически это то же самое, что умножить на 2.

    int *tmp = new int[m_data_size];    // Выделяем память под новый размер
    if (m_data)                         // если есть, что копировать, то копируем в новую память
    {
        for (unsigned i = 0; i < m_size; ++i)
            tmp[i] = m_data[i];
        delete[] m_data;                // И удаляем старые данные
    }
    m_data = tmp;                       // И переводим указатель на новую память
}

// Уменьшение размера
void CustomArray::shrink()
{
    m_data_size >>= 1;                  // Ставим размер в 2 раза меньше
    int *tmp = nullptr;
    if (m_data_size > 0)                // если новый размер не нулевой
    {
        tmp = new int[m_data_size];     // выделяем под него память
        for (unsigned i = 0; i < m_data_size; ++i)
            tmp[i] = m_data[i];         // и копируем туда старые данные, которые влезают
    }
    if (m_data)                         // Если что-то было в старом массиве, то удаляем
        delete[] m_data;
    m_data = tmp;                       // И переводим указатель
}

// Хитрая формула, чтоб найти ближайшую сверху степень двойки
unsigned CustomArray::nearestPower(unsigned x)
{
    return (1 << (int)(log2(1.0*(x - 1)) + 1));
}

// QSort
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

// Оператор []
int& CustomArray::operator[](unsigned int x)
{
    // Возвращаем тип int& (ссылка на інт)
    // Чтобы можно было писать так:
    // CustomArray a(5);
    // a[2] = 5;
    // То есть, присвоить значение какому-то индексу.
    return m_data[x];
}

// Аналогичный метод, только для константных объектов
// Тут уже присвоить не получиться, т.к. это константный объект
const int& CustomArray::operator[](unsigned int x) const
{
    return m_data[x];
}

// Оператор присваивания
// Такие операторы обычно возвращают ссылку на себя, чтобы можно было писать
// CustomArray a, b, c;
// ...
// a = b = c;
const CustomArray & CustomArray::operator=(const CustomArray & arr)
{
    if (&arr == this) return *this;                 // Если пытаемся себе присвоить себя, то ничего не делаем
    this->clear();                                  // Иначе очищаем себя
    this->resize(arr.size());                       // Меняем свой размер, чтоб был как у второго массива
    for (unsigned i = 0; i < arr.size(); ++i)       // И копируем себе данные
        m_data[i] = arr[i];
    return *this;                                   // Возвращаем ссылку на себя
}

// Оператор присваивания но для временных объектов
// Например, для случая:
// CustomArray a;
// a = CustomArray(12);
// Тут во второй строчке создаётся временный об'ект, который потом присваивается "а".
// Т.к. этот объект временный, мы можем считать, что его состояние можно испортить после присваивания
const CustomArray & CustomArray::operator=(CustomArray && arr)
{
    if (&arr == this) return *this;
    this->clear();                              // Поэтому просто очищаем себя
    this->m_data = arr.m_data;                  // И просто копируем себе его поля
    this->m_size = arr.m_size;                  // Данные можно не копировать, а только указатель на них
    this->m_data_size = arr.m_data_size;
    arr.m_data = nullptr;                       // А ему указатель занулим, чтоб он данные не удалил в деструкторе
    return *this;
}

// Объединение двух отсортированных массивов
CustomArray CustomArray::Unite(const CustomArray &arrl, const CustomArray &arrr)
{
    CustomArray newArray(arrl.size() + arrr.size());                // Создадим новый массив, тут будет объединение лежать
    unsigned i = 0, j = 0, size = 0;                                // заведём два индекса: для первого и второго массивов
    while (i < arrl.size() && j < arrr.size())                      // Пока не дошли до конца хотя бы одного массива
        if (arrl[i] < arrr[j])                                      // Смотрим, из какого массива будем доставать новый элемент
            newArray[size++] = arrl[i++];                           // Если в первом массиве i-тый элемент меньше, чем j-тый
        else                                                        // во втором, то в новый массив нужно переложить именно его.
            newArray[size++] = arrr[j++];                           // И наоборот. Увеличиваем соответствующий индекс, когда перекладываем
    while (i < arrl.size()) newArray[size++] = arrl[i++];           // Если мы оказались тут, то мы дошли до конца одного массива,
    while (j < arrr.size()) newArray[size++] = arrr[j++];           // но какой-то второй индекс ещё не дошёл до конца.
                                                                    // Мы не знаем, какой именно, поэтому пробуем пройтись
                                                                    // в каждом массиве и переложить по очереди элементы в новый
    return newArray;                                                // Возвращаем новый массив
}

// Пересечение двух массивов
CustomArray CustomArray::Cross(const CustomArray &arrl, const CustomArray &arrr)
{
    // Идея такая же как с объединением
    CustomArray newArray;                       // Заведём новый массив
    unsigned i = 0, j = 0;                      // Заведём два индекса для исходных массивов
    while (i < arrl.size() && j < arrr.size())  // Пока не дошли до конца хотя бы одного
        if (arrl[i] < arrr[j])                  // Если в первом элемент меньше, чем во втором
            ++i;                                // То двигаем первый индекс
        else if (arrr[j] < arrl[i])             // Если во втором элемент меньше, чем в первом
            ++j;                                // Двигаем второй индекс
        else                                    // А если они равны, то мы можем переложить элемент
        {                                       // в новый массив
            newArray.push_back(arrl[i]);        // Что мы и делаем
            ++i, ++j;                           // И двигаем индексы
        }
    return newArray;
}
