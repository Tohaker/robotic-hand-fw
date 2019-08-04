#include "finger_type.h"

class Finger {
    public:
        Finger(int port, bool hold, FingerType type);
        ~Finger();
        
        int getPort();
        void setHold(bool hold);
        bool getHold();
        FingerType getType();
    private:
        int _port;
        bool _hold;
        enum FingerType _type;
};