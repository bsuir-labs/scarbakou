#include <iostream>
#include <thread>
#include <fstream>
#include "example.h"

using namespace std;

typedef int                         Connection;
typedef std::vector<Connection>     MatrixRow;
typedef std::vector<MatrixRow>      IncidenceMatrix;
typedef std::vector<MatrixRow>      AdjacencyMatrix;

IncidenceMatrix fromAdjacency(const AdjacencyMatrix &matrix);
void saveTest(const T_adj_matr &matrix, const std::string &result, const unsigned test_case_number);

struct Config
{
	int TEST_CASES_TO_GENERATE = 30;
	int VERTICES_TOTAL = 3;

	Config(int argc, char* argv[])
	{
		if (argc < 2) return;
		this->TEST_CASES_TO_GENERATE = atoi(argv[1]);

		if (argc < 3) return;
		this->VERTICES_TOTAL = atoi(argv[2]);
	}
};

int  main(int argc, char* argv[])
{
	const Config config(argc, argv);

    int     const   LEFT_ADJ_VAL_BOUND      =   0;
    int     const   RIGHT_ADJ_VAL_BOUND     =   1;
	int     const   VERTICES_TOTAL = config.VERTICES_TOTAL;
	int	const test_cases_to_generate = config.TEST_CASES_TO_GENERATE;

    srand(unsigned(time(0)));

	for (int test = 1; test <= test_cases_to_generate; ++test)
	{
		T_adj_matr matr;
		generate_rand_adj_matr(LEFT_ADJ_VAL_BOUND,
		                       RIGHT_ADJ_VAL_BOUND,
					           VERTICES_TOTAL,
							   matr);
		
		auto trans = check_transitivity_of_graph(matr);
		std::string result;
		switch (trans) {
			case 0: result = "transitive"; break;
			case 1: result = "nontransitive"; break;
			case 2: result = "partly"; break;
			default: result = "undefined"; break;
		}

		saveTest(matr, result, test);
		std::cout << "Test #" << test << "; Result: " << result << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

IncidenceMatrix fromAdjacency(const AdjacencyMatrix &matrix)
{
	size_t size = matrix.size();
	std::vector<std::vector<int>> edges;
	for (unsigned v1 = 0; v1 < size; ++v1)
		for (unsigned v2 = 0; v2 < size; ++v2)
		{
			if (!matrix[v1][v2]) continue;

			std::vector<int> edge(size);
			edge[v1] = 1;
			edge[v2] = -1;
			edges.emplace_back(std::move(edge));
		}

	IncidenceMatrix graph(size, MatrixRow(edges.size()));
	for (unsigned edge_n = 0; edge_n < edges.size(); ++edge_n)
		for (unsigned node_n = 0; node_n < size; ++node_n)
			graph[node_n][edge_n] = edges[edge_n][node_n];
	return graph;
}

void saveTest(const T_adj_matr &matrix, const std::string &result, const unsigned test_case_number)
{
	auto incidence = fromAdjacency(matrix);
	
	std::ofstream fout_in;
	std::ofstream fout_out;

	static const std::string PREFIX_INPUT = "in-";
	static const std::string PREFIX_OUTPUT = "out-";
	static const std::string POSTFIX_COMM = ".txt";

	std::string input_filename = PREFIX_INPUT + std::to_string(test_case_number) + POSTFIX_COMM;
	std::string output_filename = PREFIX_OUTPUT + std::to_string(test_case_number) + POSTFIX_COMM;

	fout_in.open(input_filename);
	fout_out.open(output_filename);

	if (!fout_in.is_open() || !fout_out.is_open())
	{
		std::cerr << "Can't open files for saving the test #" << test_case_number << std::endl;
		std::cerr << "This test case will be ignored.\n" << std::endl;
		return;
	}

	// writing input
	for (unsigned node = 0; node < incidence.size(); ++node)
	{
		for (unsigned edge = 0; edge < incidence[node].size(); ++edge)
			fout_in << incidence[node][edge] << " ";
		fout_in << "\n";
	}
	fout_in.close();

	fout_out << result << std::endl;
}