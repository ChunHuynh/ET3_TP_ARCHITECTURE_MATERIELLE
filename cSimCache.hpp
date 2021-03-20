
#ifndef __CSIMCACHE_HPP_
#define __CSIMCACHE_HPP_

#include <iostream>
#include <fstream>
#include <vector>

class SimCache
{
    unsigned long int access_count; // nombre d'accès mémoire
    unsigned long int miss_count; // nombre de défauts de cache

    std::vector< std::vector<unsigned long int> > BC; // tags
    std::vector< std::vector<unsigned long int> > LRU; // timestamps d'accès mémoire

    int total_size, line_size, num_lines_per_set;
    int num_sets; // number of sets in cache: derived from total_size, line_size, num_lines_per_set

    int N; // vector/matrix dimension: used only in logResult
    bool log_access; // output detailed log of memory accesses to stderr

    int cache(unsigned long int address);
    int lru(int set);

public:
    std::ofstream ofs; // file stream for results

    SimCache(const char * const filename, bool log_access)
        : ofs(filename), log_access(log_access) {};

    ~SimCache();

    void access(unsigned long int address);
    void reset(int p_total_size, int p_line_size, int p_num_lines_per_set, int p_N);

    void logMessage(const char * const msg);
    void logHeaderRow(bool Ncube = false);
    void logResult(bool Ncube = false);
};

#endif
