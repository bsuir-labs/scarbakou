#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

// Создаём дефайны для типов, чтоб легче было понимать,
// что происходит. Это вообще не обязательно, просто так код понятнее.
// Тип AdjacencyMatrix понятнее, чем std::vector<std::vector<int>>
typedef int                         Connection;         // На самом деле, не очень логично вышло, лучше назвать Node
typedef std::vector<Connection>     MatrixRow;          // Одна строка в матрице
typedef std::vector<MatrixRow>      IncidenceMatrix;    // Матрица инцидентности
typedef std::vector<MatrixRow>      AdjacencyMatrix;    // Матрица смежности

// Опишем класс-перечисление для типов графов.
// Перечисление (enum) это что-то вроде набора констант, связанных по смыслу.
// Можно прочитать подробнее на каком-нибудь cppreferece, например.
enum class GraphTransitivity
{
    Transitive          = 0,    // Транзитивный
    NonTransitive       = 1,    // Нетранзитивный
    PartlyTransitive    = 2     // Частично транзитивный
};

// Опишем прототипы функций, которые будем использовать

///
/// \brief readGraphFromFile Функция для чтения тестового файла
/// \param fin - параметр, в который по ссылке передаётся поток ввода, из которого следует читать
/// \param ok - указатель на булевскую переменную, куда после выполнения функции будет помещён результат.
/// Если чтение прошло успешно, то там будет true, иначе - false.
/// \return Возвращает функция матрицу инцидентности, прочитанную из файла
///
IncidenceMatrix readGraphFromFile(std::ifstream &fin, bool* ok);

///
/// \brief fromIncidenceMatrix Функция для преобразования матрицы инцидентности к более удобной матрице смежности
/// (зачем вообще матрицу инцидентности придумали)))
/// \param graph - параметр, в который передаётся по ссылке (чтобы не копировать) исходная матрица инцидентности
/// \param ok - указатель на булевскую переменную. Работает, как и в предыдущей функции. Если преобразование получилось,
/// то будет тру. Иначе - фолс (например, если в тесте ошибка и матрица инцидентности некорректная какая-то)
/// \return Возвращает функция матрицу смежности, полученную после преобразования матрицы инцидентности
///
AdjacencyMatrix fromIncidenceMatrix(const IncidenceMatrix &graph, bool *ok);

///
/// \brief getGraphTransitivity Функция, которая, собственно и определяет вид графа
/// \param graph - Параметр, принимающий матрицу смежности графа
/// \return Возвращает значение типа GraphTransitivity (см. enum выше) в зависимости от полученного ответа.
///
GraphTransitivity getGraphTransitivity(const AdjacencyMatrix &graph);

int main()
{
    // Константой задаётся количество тестов
    // Тесты должны иметь имена вида input-X.txt, где вместо Х - номер теста.
    // Тесты нумеруются с 1, так что если количество равно 50, то первый тест имеет номер 1, а последний - 50.
    const unsigned kTestCaseNumber = 50;

    // По очереди обрабатываем каждый тест
    for (unsigned test = 1; test <= kTestCaseNumber; ++test)
    {
        // Сразу соберём имена файлов ввода и вывода в зависимости от номера теста
        std::string inputFileName = "input-" + std::to_string(test) + ".txt";
        std::string outputFileName = "output-" + std::to_string(test) + ".txt";

        // Создадим потоки ввода и вывода для работы с файлами
        std::ifstream fin;  // ввод
        std::ofstream fout; // вывод

        // Пробуем открыть файлы
        fin.open(inputFileName);
        fout.open(outputFileName);

        if (!fin.is_open() || !fout.is_open())  // Если хотя бы один файл не получилось открыть
        {
            std::cout << "Can't open files for test #" << test << std::endl;    // Ругаемся в консоль
            std::cout << "Skipping test." << std::endl;
            continue;   // И пропускаем тест
        }

        bool ok = true; // Тут будет результат чтения файла (см. описание функции чтения выше)
        IncidenceMatrix graph_in = readGraphFromFile(fin, &ok); // Пытаемся прочитать матрицу инцидентности
        if (!ok)                                                // Если не получилось, пропускаем тест
        {
            std::cout << "Test #" << test << " contains broken input data.\n";
            std::cout << "Skipping test." << std::endl;
            continue;
        }

        // Теперь пытаемся преобразовать матрицу инцидентности к матрице смежности
        AdjacencyMatrix graph_adj = fromIncidenceMatrix(graph_in, &ok);
        if (!ok)            // Опять же, если не получилось, то пропускаем тест
        {
            std::cout << "Test #" << test << " contains broken input data.\n";
            std::cout << "Skipping test." << std::endl;
            continue;
        }

        auto result = getGraphTransitivity(graph_adj);  // Теперь находим вид графа
        std::string output;                             // Подготовим переменную для ответа

        switch (result) {                               // В зависимости от вида графа, заполняем строку с ответом
            case GraphTransitivity::Transitive:         output = "transitive"; break;
            case GraphTransitivity::NonTransitive:      output = "nontransitive"; break;
            case GraphTransitivity::PartlyTransitive:   output = "partly"; break;
            default: output = "undefined";
        }

        fout << output << std::endl;        // И записываем ответ в файл
    }

    return 0;
}

// Чтение графа из файла. Параметры описаны выше
IncidenceMatrix readGraphFromFile(std::ifstream &fin, bool* ok)
{
    // Матрица инцидентности, куда будем читать
    IncidenceMatrix result;

    // Так как мы изначально не знаем количество строк и столбцов в файле,
    // то придётся читать построчно и парсить строку, а потом смотреть, что получилось.

    std::string line;           // Сюда будем читать строку
    std::getline(fin, line);    // Получаем строку
    unsigned rowcnt = 0;        // Тут будем держать прочитанных строк
    // std::ifstream работает так, что если мы не смогли прочитать ничего из файла (подобрались к концу),
    // то попытка прочитать исключений не выбросит, однако после этого метод std::ifstream::eof() вернёт true
    // EOF == End Of File
    while (!fin.eof())          // Проверяем, что мы ещё не добрались до конца, значит строка прочиталась
    {
        ++rowcnt;               // Тогда увеличиваем счётчик

        // Теперь надо распарсить строку
        // Для этого нам нужно будет поделить её на подстроки, разделённые пробелами

        std::list<std::string> row; // Сохраним эти подстроки вот в этот список

        std::string buffer;         // Временный буфер, через который мы будем набирать подстроку
        for (size_t i = 0; i < line.size(); ++i)    // Проходимся по строке от начала до конца
        {
            char c = line[i];               // Для удобства скопируем символ, который сейчас рассматривается
            if (isspace(c))                 // Если это пробельный символ (стандартная функция),
            {
                if (!buffer.empty())        // и если буфер не пустой, значит, только что закончилась какая-то подстрока
                    row.push_back(buffer);  // Так что добавляем буфер в список
                buffer.clear();             // И очищаем буфер, т.к. будем собирать новую подстроку теперь
            }
            else                            // Иначе (если символ непробельный)
            {
                buffer += c;                // Добавляем этот символ к буферу
            }
        }
        if (!buffer.empty()) row.push_back(buffer); // В буфере может что-то остаться, если строка не заканчивается
                                                    // пробелом. Если это так, то добавляем и остатки к списку

        MatrixRow matrixRow;                        // Создаём строку матрицы

        // Теперь проходимся по нашему списку подстрок
        // Данный синтаксис фора, кажется, уже в c++11 есть.
        // Это примерно похоже на конструкцию вида:
        // for (int i = 0; i < row.size(); ++i)
        // {
        //     const auto &str = row[i];
        //     ...
        // }
        for (const auto &str : row)
        {
            // Теперь нам нужно попытаться перевести подстроку в число
            // В стандартной библиотеке есть функция std::stoi, которая умеет это делать
            // Первым параметром она принимает строку, а вторым - указатель на size_t, куда положит
            // номер символа, на котором она остановилась (работает, как ok в наших функциях)
            std::size_t pos = 0;                    // Сюда функция положит позицию последнего символа
            int val = std::stoi(str, &pos);         // Пытаемся перевести в число
            if (pos != str.size())                  // Если она не смогла преобразовать строку целиком, то бракуем
            {                                       // Ругаемся в лог и возвращаем пустую матрицу
                std::cout << "Can't process row #" << rowcnt << " of input.";
                return IncidenceMatrix();
            }

            matrixRow.push_back(val);               // Если всё окей, то добавляем полученное число к строке матрицы
        }

        // Когда всю строку собрали, осталось только добавить её ко всей матрице
        result.push_back(matrixRow);

        std::getline(fin, line);    // И пытаемся прочитать следующую строку
    }

    *ok = true; // Если всё окей, то кладём сюда тру
    return result;  // И возвращаем результат
}

// Преобразование матрицы инцидентности к матрице смежности
AdjacencyMatrix fromIncidenceMatrix(const IncidenceMatrix &graph, bool *ok)
{
    std::size_t size = graph.size();    // Сразу запомним размер (кол-во вершин)
    if (size == 0)                      // Если вершин нету, то ругнёмся в консоль
    {
        std::cout << "Empty incidence matrix received. Seems to be broken." << std::endl;
        *ok = false;                    // И вернём пустую матрицу, пометив в "ок", что всё не ок
        return AdjacencyMatrix();
    }

    std::size_t edges = graph[0].size();    // Отдельно положим количество рёбер в графе
    AdjacencyMatrix adjMatrix(size, MatrixRow(size));   // Создадим матрицу смежности нужных размеров

    // И начинаем обрабатывать каждое ребро
    for (unsigned edge = 0; edge < edges; ++edge)
    {
        int start, end;     // Стартовая и конечная вершины
        start = end = -1;   // Сначала положим в них -1, т.е. пока ни одной не нашли

        // Переберём все вершины, чтобы понять, с кем ребро инцидентно
        for (unsigned node = 0; node < size; ++node)
        {
            Connection c = graph[node][edge];   // Проверяем, что стоит на текущем месте в матрице инцидентности
            // В матрице инцидентности будем считать, что 1 значит, что ребро начинается с данной вершины,
            // -1 - что ребро заканчивается в данной вершине,
            // а 2 - что ребро представляет собой цикл, т.е. и начинается и заканчивается в данной вершине
            if (c == 1)                 // Итак, если у нас 1
            {
                if (start == -1)        // И мы пока не успели найти начало ребра
                    start = node;       // То всё ок, запоминаем, откуда ребро начинается
                else                    // А если успели найти, то получается, что у нас слишком много начал
                {                       // Тогда ругаемся в лог и возвращаем пустую матрицу - у нас плохие данные
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row contains too much start nodes." << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            }
            else if (c == -1)           // Если у нас -1
            {
                if (end == -1)          // И мы пока не нашли конец ребра,
                    end = node;         // Запоминаем. Ну и далее аналогично, как и с началом ребра
                else
                {
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row contains too much end nodes." << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            } else if (c == 2)          // То же самое, если нашли 2
            {
                if (start == -1 && end == -1)   // Только проверяем, что мы до этого не успели найти ни начало,
                    start = end = node;         // ни конца
                else
                {
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row with a loop contains another start" << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            }
        }

        // Проверяем, что нашли и начало, и конец ребра
        if (start != -1 && end != -1)
            adjMatrix[start][end] = 1;  // и тогда ставим связь в матрице смежности между вершинами
        else if ((start != -1 && end == -1) || (start == -1 && end != -1))      // если нашли только начало или
        {                                                                       // только конец, то ребро тоже плохое
            std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;  // ругаемся и отваливаемся
            *ok = false;
            return AdjacencyMatrix();
        }   // Ну а если мы не нашли ни того, ни другого, то всё ок, наверное, просто какое-то пустое ребро
            // По идее это тоже ошибка, но она нам ничего не ломает, так что норм
    }
    *ok = true;         // Помечаем, что всё получилось
    return adjMatrix;   // И возвращаем результат
}

// Определение транзитивности графа
GraphTransitivity getGraphTransitivity(const AdjacencyMatrix & graph)
{
    size_t size = graph.size();                     // отдельно запомним кол-во вершин

    // Будем считать транзитивные и нетранзитивные пути
    // Транзитивным путём назовём три последовательно связанные вершины A, B, C,
    // Для которых выполняется условие существования пути из А в С.
    // Если условие не выполняется, то путь назовём нетранзитивным
    unsigned transitive_ways = 0;                   // Создаём счётчики
    unsigned nontransitive_ways = 0;

    for (unsigned v1 = 0; v1 < size; ++v1)          // Перебираем первую вершину
        for (unsigned v2 = 0; v2 < size; ++v2)      // Перебираем вторую вершину
        {
            if (!graph[v1][v2]) continue;           // Если не существует связи между первой и второй вершиной,
                                                    // то нам такое не надо, пропускаем
            for (unsigned v3 = 0; v3 < size; ++v3)  // Иначе перебираем третью вершину
            {
                if (!graph[v2][v3]) continue;       // Опять же, если нет связи из второй к третьей, то пропускаем

                if (graph[v1][v3]) transitive_ways++;   // Проверяем условие и увеличиваем нужный счётчик
                else nontransitive_ways++;
            }
        }

    if (nontransitive_ways == 0)                            // В итоге, если мы не нашли нетранзитивных путей,
        return GraphTransitivity::Transitive;               // то граф назовём транзитивным
    if (transitive_ways > 0 && nontransitive_ways > 0)      // Если нашли и такие, и такие, то частично транзитивным
        return GraphTransitivity::PartlyTransitive;
    return GraphTransitivity::NonTransitive;                // Иначе назовём граф нетранзитивным
}
