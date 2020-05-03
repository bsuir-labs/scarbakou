#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
/////////////////////////////////////////////////////////////////////////////////////////
typedef std::string                 T_str;

typedef int                         T_vertex;
typedef std::vector < T_vertex  >   T_adj_row;
typedef std::vector < T_adj_row >   T_adj_matr;
/////////////////////////////////////////////////////////////////////////////////////////
struct  T_generate_int
{
    //-----------------------------------------------------------------------------------
    int     left_adj_val_boud_;
    int     right_adj_val_boud_;
    //-----------------------------------------------------------------------------------
    T_generate_int
        (
            int     left_adj_val_boud,
            int     right_adj_val_boud
        )
        :
        left_adj_val_boud_      ( left_adj_val_boud     ),
        right_adj_val_boud_     ( right_adj_val_boud    )
    {}
    //-----------------------------------------------------------------------------------
    int  operator() ()
    {
        int  res    =   get_rand_val();

        if( res != 0 )
        {
            res     =   get_rand_val();
        }
        return  res;
    }
    //-----------------------------------------------------------------------------------
    int  get_rand_val()
    {
        return  rand() % ( right_adj_val_boud_ - left_adj_val_boud_ + 1 ) + left_adj_val_boud_;
    }
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
struct  T_generate_rand_adj_row
{
    //-----------------------------------------------------------------------------------
    int     left_adj_val_boud_;
    int     right_adj_val_boud_;
    int     vertices_total_;
    //-----------------------------------------------------------------------------------
    T_generate_rand_adj_row
        (
            int     left_adj_val_boud,
            int     right_adj_val_boud,
            int     vertices_total
        )//IRF
        :
        left_adj_val_boud_      ( left_adj_val_boud     ),
        right_adj_val_boud_     ( right_adj_val_boud    ),
        vertices_total_         ( vertices_total        )
    {}
    //-----------------------------------------------------------------------------------
    T_adj_row   operator()  ()//IRF
    {

        T_adj_row   adj_row;

        std::generate_n
            (
                std::back_inserter( adj_row ),
                vertices_total_,
                T_generate_int
                    (
                        left_adj_val_boud_,
                        right_adj_val_boud_
                    )//-//
            );

        return  adj_row;
    }
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
void  generate_rand_adj_matr
    (
        int             left_adj_val_boud,
        int             right_adj_val_boud,
        int             vertices_total,
        T_adj_matr  &   adj_matr
    )//IRF
{
    std::generate_n
        (
            std::back_inserter( adj_matr ),
            vertices_total,
            T_generate_rand_adj_row
                (
                    left_adj_val_boud,
                    right_adj_val_boud,
                    vertices_total
                )//-//
        );
}
/////////////////////////////////////////////////////////////////////////////////////////
void  print_adj_row( T_adj_row  const    &   adj_row )//I
{
    std::copy
        (
            adj_row.begin   (),
            adj_row.end     (),
            std::ostream_iterator<int>(std::cout, "\t")
        );

    std::cout   <<  std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////
void  print_adj_matr( T_adj_matr  const    &   adj_matr )//IRF
{
    std::cout   <<  "Adj matrix."
                <<  std::endl;

    std::for_each
        (
            adj_matr.begin  (),
            adj_matr.end    (),
            print_adj_row//-//
        );
}
/////////////////////////////////////////////////////////////////////////////////////////
T_str   get_path_mess
    (
        T_vertex    A,
        T_vertex    B,
        T_vertex    C
    )
{
    T_str   const   DELIM   =   "->";
    std::ostringstream  sout;
    sout    <<  "way "
            <<  A + 1
            <<  DELIM
            <<  B + 1
            <<  DELIM
            <<  C + 1;

    return  sout.str();
}
/////////////////////////////////////////////////////////////////////////////////////////

int check_transitivity_of_graph(T_adj_matr & adj_matr)
{
    int     vertices_total      =   adj_matr.size();

    T_str   trans_mes;
    T_str   not_trans_mes;

    for( T_vertex   v1   =   0; v1 < vertices_total; ++v1 )
    {
        for( T_vertex v2 = 0; v2 < vertices_total; ++v2 )
        {
            if( adj_matr[v1][v2] ==  0 )
            {
                continue;
            }

            for( T_vertex  v3 = 0; v3 < vertices_total; ++v3 )
            {
                if( adj_matr[v2][v3] == 0 )
                {
                    continue;
                }
                if  (
                        adj_matr[v1][v3] != 0
                    )
                {
                    if  (
                            trans_mes.empty()
                        )
                    {
                        trans_mes       =   get_path_mess( v1, v2, v3 );
                    }
                }
                else
                {
                    if  (
                            not_trans_mes.empty()
                        )
                    {
                        not_trans_mes   =   get_path_mess( v1, v2, v3 );
                    }
                }//else
            }//for
        }//for
    }//for

    if  (
                trans_mes       .empty()
            &&  not_trans_mes   .empty()
        )
    {
        return 0;
    }
    else
    {
        if  (
                    !trans_mes       .empty()
                &&  !not_trans_mes   .empty()
            )
        {
            return 2;
        }
        else if (
                    trans_mes.empty()
                )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }//else
}

#endif // EXAMPLE_H
