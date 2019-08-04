#include "finger.h"

Finger::Finger(int port, bool hold, FingerType type) : _port(port), _hold(hold), _type(type) {}

Finger::~Finger() {}

int Finger::getPort() {
    return _port;
}

void Finger::setHold(bool hold) {
    _hold = hold;
}

bool Finger::getHold() {
    return _hold;
}

FingerType Finger::getType() {
    return _type;
}
