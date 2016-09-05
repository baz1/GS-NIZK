#include <iostream>
#include <fstream>
#include <cstdlib>

#include "gsnizk.h"
#include "tests.h"

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
    ASSERT(getPairing());
#endif
    testLibrary();
    pairings::terminate_pairings();
    return 0;
}
