
#include "cSimCache.hpp"
#include <array>
#include <iostream>

void calc6(int N, SimCache &cache, double **x, double **y, double **z, double **yt)
{
    double s = 0;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            cache.access((unsigned long int) &y[i][j]);
            cache.access((unsigned long int) &yt[j][i]);

            yt[j][i]=y[i][j];
        }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            s = 0;

            for (int k = 0; k < N; ++k)
            {
                cache.access((unsigned long int) &x[i][k]);
                cache.access((unsigned long int) &yt[j][k]);

                s = s + x[i][k]*yt[j][k];
            }

            cache.access((unsigned long int) &z[i][j]);
            z[i][j] = s;
        }
}

void run6(int N, const char * const out, bool detailed_log,
          const std::array<int, 3> &total_size_list,
          const std::array<int, 3> &line_size_list,
          const std::array<int, 3> &num_lines_per_set_list)
{
    SimCache cache(out, detailed_log);

    // Memory allocation
    double *raw = new double[N*N + N*N + N*N + N*N]; // contiguous memory for x[N][N], y[N][N], z[N][N], yt[N][N]

    double **x = new double *[N]; // pointers to lines of x
    for (int i = 0; i < N; ++i)
        x[i] = raw + i*N;

    double **y = new double *[N]; // pointers to lines of y
    for (int i = 0; i < N; ++i)
        y[i] = raw + N*N + i*N;

    double **z = new double *[N]; // pointers to lines of z
    for (int i = 0; i < N; ++i)
        z[i] = raw + N*N + N*N + i*N;

    double **yt = new double *[N]; // pointers to lines of yt
    for (int i = 0; i < N; ++i)
        yt[i] = raw + N*N + N*N + N*N + i*N;

    // init x, y
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            x[i][j] = i + j;
            y[i][j] = i - j;
        }

    // write log file header
    cache.ofs << "exo 6 : IJKT\n\n"
              << "&x[0][0]  = " << (void *)(&x[0][0]) << '\n'
              << "&y[0][0]  = " << (void *)(&y[0][0]) << '\n'
              << "&z[0][0]  = " << (void *)(&z[0][0]) << '\n'
              << "&yt[0][0] = " << (void *)(&yt[0][0]) << '\n'
              << "sizeof(double) = " << sizeof(double) << "\n\n";
    cache.logHeaderRow(true);

    for (auto total_size : total_size_list)
        for (auto line_size : line_size_list)
            for (auto num_lines_per_set : num_lines_per_set_list)
            {
                // init cache
                cache.reset(total_size, line_size, num_lines_per_set, N);

                // run calculation
                calc6(N, cache, x, y, z, yt);

                // write result
                cache.logResult(true);
            }

    delete [] raw;
    delete [] x;
    delete [] y;
    delete [] z;
    delete [] yt;
}
