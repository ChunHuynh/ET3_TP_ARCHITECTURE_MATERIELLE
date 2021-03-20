
#include "cSimCache.hpp"
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>

SimCache::~SimCache()
{
    ofs.close();
}

/* Returns the index of least recently used line in the set. */
int SimCache::lru(int set)
{
    unsigned long int min_timestamp = LRU[set][0];
    int lru_line = 0;

    for (int line = 1; line < num_lines_per_set; ++line)
        if (LRU[set][line] < min_timestamp)
        {
            min_timestamp = LRU[set][line];
            lru_line = line;
        }

    return lru_line;
}

/* Simulates a cache lookup for address. Manages LRU data and line replacement.
Returns 1 if cache miss, otherwise 0. */
int SimCache::cache(unsigned long int address)
{
    if (log_access)
        std::cerr << std::hex << "Access address=" << address;

    address = address / (unsigned long int)line_size; // get rid of offset

    unsigned long int tag = address / (unsigned long int)num_sets;
    int set = address % num_sets;

    if (log_access)
        std::cerr << std::hex << ", set=" << set << ", tag=" << tag << ':';

    for (int line = 0; line < num_lines_per_set; ++line)
        if (BC[set][line] == tag) // found in cache
        {
            LRU[set][line] = access_count;

            if (log_access)
                std::cerr << " hit\n";

            return 0;
        }

    int lru_line = lru(set);

    if (log_access)
        std::cerr << std::hex << " miss, replacing line " << lru_line
                  << ", old tag " << BC[set][lru_line] << '\n';

    BC[set][lru_line] = tag;
    LRU[set][lru_line] = access_count;

    return 1;
}

/* acces cache pour l'adresse z */
void SimCache::access(unsigned long int address)
{
    ++access_count;
    miss_count += cache(address);
}

/* RAZ des memoires etiquettes et LRU */
void SimCache::reset(int p_total_size, int p_line_size, int p_num_lines_per_set, int p_N)
{
    total_size = p_total_size;
    line_size = p_line_size;
    num_lines_per_set = p_num_lines_per_set;
    num_sets = total_size / (line_size * num_lines_per_set);
    N = p_N;

    if (log_access)
        std::cerr << std::dec << "\n*** Resetting cache to: total_size=" << total_size
                  << " o, line_size=" << line_size
                  << " o, " << num_lines_per_set << "-way associative, "
                  << "num_sets=" << num_sets << "\n\n";

    BC.resize(num_sets);
    for (auto &e : BC) e.resize(num_lines_per_set);

    LRU.resize(num_sets);
    for (auto &e : LRU) e.resize(num_lines_per_set);

    access_count = 0L;
    miss_count = 0L;

    for (int set = 0; set < num_sets; ++set)
        for (int line = 0; line < num_lines_per_set; ++line)
        {
            LRU[set][line] = 0L;
            BC[set][line] = 0L;
        }
}

void SimCache::logMessage(const char * const msg)
{
    ofs << msg;
}

void SimCache::logHeaderRow(bool Ncube)
{
    std::ostringstream tmp;

    tmp << std::left
        << std::setw(15) << "TOTAL_SIZE"
        << std::setw(15) << "LINE_SIZE"
        << std::setw(15) << "LINES_PER_SET"
        << std::setw(15) << "NUM_SETS"
        << std::setw(15) << "N"
        << std::setw(15) << "Miss rate";

    if (Ncube)
        tmp << std::setw(15) << "Miss/(N*N*N)";

    ofs << tmp.str() << "\n\n";
}

void SimCache::logResult(bool Ncube)
{
    std::ostringstream tmp;

    tmp << std::left << std::setw(15) << total_size
        << std::setw(15) << line_size
        << std::setw(15) << num_lines_per_set
        << std::setw(15) << num_sets
        << std::setw(15) << N
        << std::setw(15) << (float)miss_count/(float)access_count;

    if (Ncube)
        tmp << std::setw(15) << (float)miss_count/(N*N*N);

    ofs << tmp.str() << '\n';
}

