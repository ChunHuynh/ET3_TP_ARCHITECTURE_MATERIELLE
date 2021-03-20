
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

void myexit(const char * const message)
{
    std::cerr << message << '\n';
    exit(-1);
}

char* getCmdOption(char **begin, char **end, const std::string &option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

void parseArgs(int &N, int &exo, char *&out, bool &detailed_log, int argc, char *argv[])
{
    N = 0;
    exo = 0;
    out = nullptr;
    detailed_log = false;

    char *option;

    if ((option = getCmdOption(argv, argv+argc, "-N")) != nullptr)
    {
        N = atoi(option);
    }
    if (N == 0 || N < 0)
    {
        myexit("bad -N");
    }

    if ((option = getCmdOption(argv, argv+argc, "-exo")) != nullptr)
    {
        exo = atoi(option);
    }
    if (exo == 0 || exo < 3 || exo > 7)
    {
        myexit("bad -exo");
    }

    if ((option = getCmdOption(argv, argv+argc, "-out")) != nullptr)
    {
        out = strdup(option);
    }
    if (out == nullptr)
    {
        myexit("bad -out");
    }

    if (cmdOptionExists(argv, argv+argc, "-log"))
    {
        detailed_log = true;
    }
}

