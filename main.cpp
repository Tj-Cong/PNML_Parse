#include <iostream>
#include "Petri_Net.h"
using namespace std;

int main() {
    char filename[] = "model.pnml";
    Petri ptnet;
    ptnet.readPNML(filename);
    ptnet.printPlace();
    ptnet.printTransition();
    return 0;
}