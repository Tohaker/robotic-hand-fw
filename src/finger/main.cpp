#include "finger.h"
#include <iostream>

int main() {
    Finger finger(0, false, THUMB);
    std::cout << finger.getPort();
}