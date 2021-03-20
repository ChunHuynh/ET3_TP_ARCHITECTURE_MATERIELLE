
#include "cSimCache.hpp"
#include <array>
#include <iostream>

void calc3(int N, SimCache &cache, double *x, double *y)
{
    double s = 0;

    for (int i = 0; i < N; ++i)
    {
        cache.access((unsigned long int) &x[i]);
        cache.access((unsigned long int) &y[i]);

        s = s + x[i]*y[i];
    }
}

void run3(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list)
{
    SimCache cache(out, detailed_log);

    // Memory allocation
    double *raw = new double[N + N]; // contiguous memory for x[N], y[N]

    double *x = raw;
    double *y = raw + N;

    // init x, y
    for (int i = 0; i < N; ++i)
    {
        x[i] = i + N - 1;
        y[i] = i - N + 1;
    }

    // write log file header
    cache.ofs << "exo 3 : PRODUIT SCALAIRE\n\n"
              << "&x[0] = " << (void *)(&x[0]) << '\n'
              << "&y[0] = " << (void *)(&y[0]) << '\n'
              << "sizeof(double) = " << sizeof(double) << "\n\n";
    cache.logHeaderRow();

    for (auto total_size : total_size_list)
        for (auto line_size : line_size_list)
            for (auto num_lines_per_set : num_lines_per_set_list)
            {
                // init cache
                cache.reset(total_size, line_size, num_lines_per_set, N);

                // run calculation
                calc3(N, cache, x, y);

                // write result
                cache.logResult();
            }

    delete [] raw;
}

