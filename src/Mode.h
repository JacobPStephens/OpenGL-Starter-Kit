#ifndef MODE_H
#define MODE_H

enum GhostMode {
    SCATTER,
    CHASE,
    FRIGHTENED,
    EATEN,
    HOME_IDLE,
    HOME_EXIT,
    HOME_ENTER
};

enum PlayerMode {
    NORMAL,
    HUNTING,
    EATING,
    DYING 
};

// inline allows the linker to be okay with multiple definitions
inline GhostMode switchMode(GhostMode m) {
    if (m == SCATTER) 
        return CHASE;
    return SCATTER;
}

inline const char* ModeToString(GhostMode m) {
    switch (m) {
        case SCATTER:
            return "SCATTER";
        break;
        case CHASE:
            return "CHASE";
        break;
        default:
            return "Non-Scatter/Chase";
        break;
    }
}

#endif