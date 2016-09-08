#include <iostream>
#include <cstdio>

#include "gsnizk.h"
#include "tests.h"
#include "benchmark.h"

using namespace std;
using namespace gsnizk;

#define PBC_PAIRING_FILE "pairing.param"

bool getPairing()
{
    char *paramData;
    FILE *paramFile = fopen(PBC_PAIRING_FILE, "r");
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
    initialize_pairings(count, paramData);
    delete[] paramData;
    return true;
}

int main() {
#ifdef USE_MIRACL
    initialize_pairings(0, 0);
#else
    if (!getPairing()) {
        cerr << ("Error: Could not read file \"" PBC_PAIRING_FILE
                 "\".\n") << endl;
        return 0;
    }
#endif
    testLibrary();
    benchmarkLibrary();
    terminate_pairings();
    cout << "Done." << endl;
    return 0;
}
