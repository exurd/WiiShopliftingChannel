#ifndef TICKETMAN_H
#define TICKETMAN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "microtar.h"

int hard_ticket_to_nand(uint64_t value);

u32 patch_isfs(void);

#endif
