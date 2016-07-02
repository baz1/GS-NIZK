#include <iostream>

#include "pairings.h"

using namespace std;
using namespace pairings;

int main() {
    initialize_pairings(0, 0);
    cout << "Hello world" << endl;
    terminate_pairings();
    return 0;
}
