#include "example.h"
#include <vector>

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



//IncidenceMatrix readGraph(const std::string &filename);
//AdjacencyMatrix fromIncidence(const IncidenceMatrix &matrix);
//IncidenceMatrix fromAdjacency(const AdjacencyMatrix &matrix);
//GraphTransitivity getGraphTransitivity(const IncidenceMatrix &graph);
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


using namespace std;

int  main()//IRF
{
    int     const   LEFT_ADJ_VAL_BOUND      =   0;
    int     const   RIGHT_ADJ_VAL_BOUND     =   1;
    int     const   VERTICES_TOTAL          =   3;

    srand(unsigned(time(0)));
    std::locale::global(std::locale(""));

    for(;;)
    {
        T_adj_matr  adj_matr;

        generate_rand_adj_matr
            (
                LEFT_ADJ_VAL_BOUND,
                RIGHT_ADJ_VAL_BOUND,
                VERTICES_TOTAL,
                adj_matr
            );//-//

        std::cout   <<  "\n\n\n\n\n\n";
        print_adj_matr( adj_matr );
        std::cout   <<  std::endl;
//        check_transitivity_of_graph_and_print_result( adj_matr );//-//

        auto res = getGraphTransitivity(adj_matr);
        std::string output;
        switch (res) {
            case GraphTransitivity::Transitive:       output = "Transitive!"; break;
            case GraphTransitivity::NonTransitive:    output = "Non transitive!"; break;
            case GraphTransitivity::PartlyTransitive: output = "Partly transitive"; break;
        }

        std::cout << output << " ";

//        check_transitivity_of_graph_and_print_result(adj_matr);
        auto trans = check_transitivity_of_graph(adj_matr);
        if (static_cast<int>(res) == trans)
        {
            std::cout << "Ok!";
        } else {
            std::cout << "False! Another checker got: ";
            check_transitivity_of_graph_and_print_result(adj_matr);
            break;
        }

        std::cout   <<  std::endl;
        system( "clear" );
    }//for
}
