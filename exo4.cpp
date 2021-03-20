
#include "cSimCache.hpp"
#include <array>
#include <iostream>

void calc4(int N, SimCache &cache, double **x, double *y, double *z)
{
    double s = 0;

    for (int i = 0; i < N; ++i)
    {
        s = 0;

        for (int k = 0; k < N; ++k)
        {
            cache.access((unsigned long int) &x[i][k]);
            cache.access((unsigned long int) &y[k]);

            s = s + x[i][k]*y[k];
        }

        cache.access((unsigned long int) &z[i]);
        z[i] = s;
    }
}

void run4(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list)
{
    SimCache cache(out, detailed_log);

    // Memory allocation
    double *raw = new double[N*N + N + N]; // contiguous memory for x[N][N], y[N], z[N]

    double **x = new double *[N]; // pointers to lines of x
    for (int i = 0; i < N; ++i)
        x[i] = raw + i*N;

    double *y = raw + N*N;
    double *z = raw + N*N + N;

    // init x, y
    for (int i = 0; i < N; ++i)
    {
        y[i] = i;
        for (int j = 0; j < N; ++j)
            x[i][j] = i + j;
    }

    // write log file header
    cache.ofs << "exo 4 : PRODUIT MATRICE VECTEUR\n\n"
              << "&x[0][0] = " << (void *)(&x[0][0]) << '\n'
              << "&y[0]    = " << (void *)(&y[0]) << '\n'
              << "&z[0]    = " << (void *)(&z[0]) << '\n'
              << "sizeof(double) = " << sizeof(double) << "\n\n";
    cache.logHeaderRow();

    for (auto total_size : total_size_list)
        for (auto line_size : line_size_list)
            for (auto num_lines_per_set : num_lines_per_set_list)
            {
                // init cache
                cache.reset(total_size, line_size, num_lines_per_set, N);

                // run calculation
                calc4(N, cache, x, y, z);

                // write result
                cache.logResult();
            }

    delete [] raw;
    delete [] x;
}
