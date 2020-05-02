#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

typedef int                         Connection;
typedef std::vector<Connection>     MatrixRow;
typedef std::vector<MatrixRow>      IncidenceMatrix;
typedef std::vector<MatrixRow>      AdjacencyMatrix;

enum class GraphTransitivity
{
    Transitive          = 0,
    NonTransitive       = 1,
    PartlyTransitive    = 2
};

IncidenceMatrix readGraphFromFile(std::ifstream &fin, bool* ok);
AdjacencyMatrix fromIncidenceMatrix(const IncidenceMatrix &graph, bool *ok);
GraphTransitivity getGraphTransitivity(const AdjacencyMatrix &graph);

int main()
{
    const unsigned kTestCaseNumber = 50;

    for (unsigned test = 1; test <= kTestCaseNumber; ++test)
    {
        std::string inputFileName = "input-" + std::to_string(test) + ".txt";
        std::string outputFileName = "output-" + std::to_string(test) + ".txt";

        std::ifstream fin;
        std::ofstream fout;

        fin.open(inputFileName);
        fout.open(outputFileName);

        if (!fin.is_open() || !fout.is_open())
        {
            std::cout << "Can't open files for test #" << test << std::endl;
            std::cout << "Skipping test." << std::endl;
            continue;
        }

        bool ok = true;
        IncidenceMatrix graph_in = readGraphFromFile(fin, &ok);
        if (!ok)
        {
            std::cout << "Test #" << test << " contains broken input data.\n";
            std::cout << "Skipping test." << std::endl;
            continue;
        }

        AdjacencyMatrix graph_adj = fromIncidenceMatrix(graph_in, &ok);
        if (!ok)
        {
            std::cout << "Test #" << test << " contains broken input data.\n";
            std::cout << "Skipping test." << std::endl;
            continue;
        }

        auto result = getGraphTransitivity(graph_adj);
        std::string output;

        switch (result) {
            case GraphTransitivity::Transitive:         output = "transitive"; break;
            case GraphTransitivity::NonTransitive:      output = "nontransitive"; break;
            case GraphTransitivity::PartlyTransitive:   output = "partly"; break;
            default: output = "undefined";
        }

        fout << output << std::endl;
    }

    return 0;
}

IncidenceMatrix readGraphFromFile(std::ifstream &fin, bool* ok)
{
    IncidenceMatrix result;

    std::string line;
    std::getline(fin, line);
    unsigned rowcnt = 0;
    while (!fin.eof())
    {
        ++rowcnt;
        std::list<std::string> row;

        std::string buffer;
        for (size_t i = 0; i < line.size(); ++i)
        {
            char c = line[i];
            if (isspace(c))
            {
                if (!buffer.empty())
                    row.push_back(buffer);
                buffer.clear();
            }
            else
            {
                buffer += c;
            }
        }
        if (!buffer.empty()) row.push_back(buffer);

        MatrixRow matrixRow;

        for (const auto &str : row)
        {
            std::size_t pos = 0;
            int val = std::stoi(str, &pos);
            if (pos != str.size())
            {
                std::cout << "Can't process row #" << rowcnt << " of input.";
                return IncidenceMatrix();
            }

            matrixRow.push_back(val);
        }

        result.push_back(matrixRow);

        std::getline(fin, line);
    }

    *ok = true;
    return result;
}


AdjacencyMatrix fromIncidenceMatrix(const IncidenceMatrix &graph, bool *ok)
{
    std::size_t size = graph.size();
    if (size == 0)
    {
        std::cout << "Empty incidence matrix received. Seems to be broken." << std::endl;
        *ok = false;
        return AdjacencyMatrix();
    }

    std::size_t edges = graph[0].size();
    AdjacencyMatrix adjMatrix(size, MatrixRow(size));

    for (unsigned edge = 0; edge < edges; ++edge)
    {
        int start, end;
        start = end = -1;

        for (unsigned node = 0; node < size; ++node)
        {
            Connection c = graph[node][edge];
            if (c == 1)
            {
                if (start == -1)
                    start = node;
                else
                {
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row contains too much start nodes." << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            }
            else if (c == -1)
            {
                if (end == -1)
                    end = node;
                else
                {
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row contains too much end nodes." << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            } else if (c == 2)
            {
                if (start == -1 && end == -1)
                    start = end = node;
                else
                {
                    std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
                    std::cout << "The row with a loop contains another start" << std::endl;
                    *ok = false;
                    return AdjacencyMatrix();
                }
            }
        }

        if (start != -1 && end != -1)
            adjMatrix[start][end] = 1;
        else if ((start != -1 && end == -1) || (start == -1 && end != -1))
        {
            std::cout << "Invalid matrix (edge #" << edge << ")" << std::endl;
            *ok = false;
            return AdjacencyMatrix();
        }
    }
    *ok = true;
    return adjMatrix;
}

GraphTransitivity getGraphTransitivity(const AdjacencyMatrix & graph)
{
    size_t size = graph.size();
    unsigned transitive_ways = 0;
    unsigned nontransitive_ways = 0;

    for (unsigned v1 = 0; v1 < size; ++v1)
        for (unsigned v2 = 0; v2 < size; ++v2)
        {
            if (!graph[v1][v2]) continue;

            for (unsigned v3 = 0; v3 < size; ++v3)
            {
                if (!graph[v2][v3]) continue;

                // there's a way v1->v2 and v2->v3
                if (graph[v1][v3]) transitive_ways++;
                else nontransitive_ways++;
            }
        }

    if (nontransitive_ways == 0)
        return GraphTransitivity::Transitive;
    if (transitive_ways > 0 && nontransitive_ways > 0)
        return GraphTransitivity::PartlyTransitive;
    return GraphTransitivity::NonTransitive;
}
