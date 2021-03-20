
#include <iostream>
#include <cstdio>
#include <array>
#include "cSimCache.hpp" // cache simulator

// Parse arguments.
// Defined in parseArgs.cpp
void parseArgs(int &N, int &exo, char *&out, bool &detailed_log, int argc, char *argv[]);

// Run computations.
// Defined in exo{3..7}.cpp
void run3(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list);
void run4(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list);
void run5(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list);
void run6(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list);
void run7(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list);

int main(int argc, char *argv[])
{
    int N, exo;
    char *out;
    bool detailed_log;

    parseArgs(N, exo, out, detailed_log, argc, argv); // allocates out filename

    // different cache sizes to simulate
    std::array<int, 3> total_size_list {4096, 8192, 16384};
    // different line sizes to simulate
    std::array<int, 3> line_size_list {16, 32, 64};
    // different numbers of lines per set to simulate
    std::array<int, 3> num_lines_per_set_list {1, 2, 4};

    void (*run[8])(int, const char * const, bool,
                   const std::array<int, 3> &,
                   const std::array<int, 3> &,
                   const std::array<int, 3> &)
        = {nullptr, nullptr, nullptr, run3, run4, run5, run6, run7};

    run[exo](N, out, detailed_log,
             total_size_list,
             line_size_list,
             num_lines_per_set_list);

    free(out);
}

