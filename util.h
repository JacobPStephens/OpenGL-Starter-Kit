#pragma once
class util {
    public:
        static const unsigned int scrWidth = 900;
        static const unsigned int scrHeight = 900;
        static int getSign(float number) {
            if (number > 0)
                return 1;
            if (number < 0)
                return -1;
            return 0;
        }
};