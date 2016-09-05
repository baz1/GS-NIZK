#include <iostream>
#include <cstdio>

#include "gsnizk.h"
#include "tests.h"
#include "benchmark.h"

using namespace std;

bool getPairing()
{
    char *paramData;
    FILE *paramFile = fopen("pairing.param", "r");
    if (!paramFile)
        return false;
    fseek(paramFile, 0, SEEK_END);
    size_t count = ftell(paramFile);
    if (count <= 0)
        count = 2048;
    paramData = new char[count];
    fseek(paramFile, 0, SEEK_SET);
    count = fread(paramData, sizeof(char), count, paramFile);
    fclose(paramFile);
    if (!count)
    {
        delete[] paramData;
        return false;
    }
    pairings::initialize_pairings(count, paramData);
    delete[] paramData;
    return true;
}

int main() {
#ifdef USE_MIRACL
    pairings::initialize_pairings(0, 0);
#else
    if (!getPairing()) {
        cerr << "Error: Could not read file \"pairing.param\".\n" << endl;
        return;
    }
#endif
    //testLibrary();
    benchmarkLibrary();
    pairings::terminate_pairings();
    cout << "Done." << endl;
    return 0;
}
