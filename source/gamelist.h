#ifndef GAMELIST_H
#define GAMELIST_H

#include <stdint.h>
#include <stddef.h>

typedef struct GameList {
    uint64_t id;
    char country[3];
    char title[256];
} GameList;

extern GameList gamelist[];
extern size_t game_count;

#endif // GAMELIST_H
