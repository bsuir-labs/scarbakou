#include <iostream>
#include <thread>
#include "example.h"

using namespace std;

typedef int                         Connection;
typedef std::vector<Connection>     MatrixRow;
typedef std::vector<MatrixRow>      IncidenceMatrix;
typedef std::vector<MatrixRow>      AdjacencyMatrix;


void saveTest(const T_adj_matr &matrix, const std::string &result);

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

        auto trans = check_transitivity_of_graph(adj_matr);
        std::string result;
        switch (trans) {
            case 0: result = "transitive"; break;
            case 1: result = "nontransitive"; break;
            case 2: result = "partly"; break;
            default: result = "undefined"; break;
        }

        std::cout   <<  std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }//for
}
