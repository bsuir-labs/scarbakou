#ifndef CUSTOMARRAY_H
#define CUSTOMARRAY_H

///
/// \brief The CustomArray class - Класс для работы с динамическим массивом
/// Сделан примерно как std::vector
///
class CustomArray
{
    // Приватные члены класса
    // Они не видны снаружи

    /// Указатель на обычный сишный динамический массив
    int *m_data;

    /// Настоящий размер сишного массива внутри
    /// Наружу отдаваться не будет. Настоящий размер будет отличаться от
    /// того, который мы будет показывать пользователю, для уменьшения
    /// количества аллокаций памяти. На самом деле мы будем изменять размер
    /// по степеням двойки. Т.е. 1, 2, 4, 8, 16, 32, 64 и т.д.
    /// В итоге если пользователь будет добавлять в конец массива по одному элементу
    /// 100 раз, то вместо ста выделений памяти и копирования в неё старого массива
    /// мы будем это делать только 8 раз.
    unsigned m_data_size;

    /// Собственно, размер, который будет видеть пользователь
    unsigned m_size;

    /// \brief expand Метод для расширения массива на следующую ступень,
    /// то есть если настоящий размер был 8, то после выполнения этого метода он будет 16
    void expand();

    ///
    /// \brief shrink Метод для уменьшения массива на предыдущую ступень.
    /// Как расширение, только наоборот
    ///
    void shrink();

    ///
    /// \brief nearestPower Метод для нахождения ближайшей сверху степени двойки
    /// \param x - Число, ближайшую степень двойки к которому требуется найти
    /// Например, для 5 ближайшая сверху степень двойки это 8. Для 9 - это 16.
    ///
    /// Метод статический, т.к. он не привязан к состоянию объекта никак, а просто как обычная функция
    /// Можно было даже не делать его членом класса (и лучше, наверное)
    ///
    static unsigned nearestPower(unsigned x);

    ///
    /// \brief sort Реализация алгоритма QSort
    /// \param arr  массив, который будет сортироваться
    /// \param left левая граница сортировки
    /// \param right и правая
    ///
    /// Подробнее можно где-нибудь на википедии прочитать или у вас в методах где-то было вроде объяснение
    ///
    static void sort(int* arr, int left, int right);

public:

    // Тут начинаются публичные методы, которые может снаружи дёргать пользователь

    ///
    /// \brief CustomArray Конструктор по умолчанию, который должен создать пустой массив
    ///
    CustomArray();

    ///
    /// \brief CustomArray Конструктор копирования, который создаёт копию другого массива
    /// \param arr - ссылка на другой массив, копию которого требуется создать
    ///
    CustomArray(const CustomArray &arr);

    ///
    /// \brief CustomArray Конструктор, создающий массив заданного размера
    /// \param size - размер массива
    ///
    CustomArray(unsigned size);

    /// Деструктор, должен память очистить
    ~CustomArray();

    ///
    /// \brief size Возвращает размер массива (который для пользователя)
    ///
    unsigned size() const;

    ///
    /// \brief isEmpty Проверяет массив на пустоту
    /// \return Возвращает тру, если массив пустой (размер равен 0), иначе фолс
    ///
    bool isEmpty() const;

    ///
    /// \brief push_back Добавление элемента в конец массива
    /// \param element Новый элемент, который нужно добавить
    ///
    void push_back(int element);

    ///
    /// \brief pop_back Удаление последнего элемента с конца массива
    ///
    void pop_back();

    ///
    /// \brief clear Очистка массива. Удаляет все элементы и изменяет размер до 0
    ///
    void clear();

    ///
    /// \brief resize Изменение размера массива
    /// \param size Новый размер
    ///
    void resize(unsigned int size);

    ///
    /// \brief sort Сортировка текущего массива
    ///
    void sort();

    ///
    /// \brief insertKeepSorted Вставка элемента в отсортированный массив, сохраняя его отсортированность
    /// \param element Элемент, который нужно вставить
    /// Перед вызовом этого метода массив должен быть отсортирован
    ///
    void insertKeepSorted(int element);

    ///
    /// \brief indexOf Бинарный поиск в отсортированном массиве
    /// \param element Элемент, который нужно найти
    /// \return Номер элемента в массиве. Если элемент не найден, то возвращает -1
    ///
    int indexOf(int element);

    ///
    /// \brief Unite Функция, объединяющая два отсортированных массива в один
    /// \param arrl Первый массив
    /// \param arrr Второй массив
    /// \return Возвращает новый массив, содержащий в себе объединение двух массивов
    ///
    static CustomArray Unite(const CustomArray &arrl, const CustomArray &arrr);

    ///
    /// \brief Cross Функция, которая возвращает пересечение двух отсортированных массивов
    /// \param arrl Первый массив
    /// \param arrr Второй массив
    /// \return Новый массив, содержащий в себе элементы, которые есть и в первом, и во втором массиве
    ///
    static CustomArray Cross(const CustomArray &arrl, const CustomArray &arrr);

    ///
    /// \brief operator [] - перегрузка оператора [], чтобы к элементам массива можно было обращаться
    /// в виде arr[x], как в сишном массиве
    /// \param x индекс элемента, который нужно вернуть
    /// \return Возвращаем ссылку на нужный элемент в массиве
    ///
    int& operator[](unsigned int x);

    ///
    /// \brief operator [] - Такая же перегрузка, как в предыдущем случае, только для константных объектов
    /// \param x индес элемента
    /// \return Возвращает КОНСТАНТНУЮ ссылку на нужный элемент
    ///
    const int& operator[](unsigned int x) const;

    ///
    /// \brief operator = - Перегрузка оператора присваивания, чтобы правильно отрабатывали строки типа:
    /// CustomArray a, b;
    /// a = b;
    /// \param arr - массив, который будем присваивать
    /// \return
    ///
    const CustomArray& operator=(const CustomArray& arr);

    ///
    /// \brief operator = - Ну а это уже чтобы выпендриться. Перегрузка присваивания для временных объектов.
    /// Чтобы не копировать весь массив, мы можем скопировать только указатель, ведь объект временный и ему всё равно.
    /// \param arr
    /// \return
    ///
    const CustomArray& operator=(CustomArray&& arr);
};

#endif // CUSTOMARRAY_H
