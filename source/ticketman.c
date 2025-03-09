#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <fat.h>

#include <gccore.h>
#include <ogc/ios.h>
#include <ogc/isfs.h>
#include <ogc/system.h>

#include "microtar.h"


void to_lower_case(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}


// void print_hex(const char *data, size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         printf("%02X ", (unsigned char)data[i]);
//         if ((i + 1) % 16 == 0) {
//             printf("\n");
//         }
//     }
//     printf("\n");
// }


int hard_ticket_to_nand(uint64_t value) {
    // https://www.imdb.com/title/tt0093146
    uint32_t hex = (uint32_t)(value & 0xFFFFFFFF);

    char tid[16];
    char filename[21];

    sprintf(tid, "%08X", hex);
    to_lower_case(tid);
    snprintf(filename, sizeof(filename), "%s.tik", tid);

    mtar_t tar;
    mtar_header_t h;
    char *ticket;

    printf("Opening SD Card and ISFS...\n");
    if (!fatInitDefault()) {
        printf("Failed to initialize FAT filesystem.\n");
        return -1;
    }
    if (ISFS_Initialize() != ISFS_OK) {
        printf("Failed to initialize ISFS.\n");
        return -1;
    }

    // open tickets.tar
    printf("Finding ticket in TAR file...\n");
    if (mtar_open(&tar, "sd:/tickets.tar", "r") != MTAR_ESUCCESS &&
        mtar_open(&tar, "sd:/apps/WiiLikeToParty/tickets.tar", "r") != MTAR_ESUCCESS) {
        printf("Failed to open the tar archive.\n");
        return -1;
    }

    // get specified tik from tar
    if (mtar_find(&tar, filename, &h) != MTAR_ESUCCESS) {
        printf("File '%s.tik' not found in the archive.\n", filename);
        mtar_close(&tar);
        return -1;
    }

    // allocate memory
    printf("Reading ticket...\n");
    ticket = calloc(1, h.size + 1);
    if (ticket == NULL) {
        printf("Memory allocation failed.\n");
        mtar_close(&tar);
        return -1;
    }

    // read ticket
    if (mtar_read_data(&tar, ticket, h.size) != MTAR_ESUCCESS) {
        printf("Failed to read data from the tar file.\n");
        free(ticket);
        mtar_close(&tar);
        return -1;
    }

    char nand_path[64];
    snprintf(nand_path, sizeof(nand_path), "/ticket/00010001/%s", filename);

    printf("Writing ticket...\n");
    s32 fd = ISFS_Open(nand_path, ISFS_OPEN_WRITE);
    if (fd < 0) {
        if (fd == -106) {  // ISFS_ENOENT (-106)
            s32 createRet = ISFS_CreateFile(nand_path, 0, 3, 3, 0);
            if (createRet != ISFS_OK) {
                printf("L-135: Failed to create ticket file: %d\n", createRet);
                free(ticket);
                mtar_close(&tar);
                ISFS_Deinitialize();
                return -1;
            }

            fd = ISFS_Open(nand_path, ISFS_OPEN_WRITE);
            if (fd < 0) {
                printf("L-144: Failed to open ticket file for writing: %d\n", fd);
                free(ticket);
                mtar_close(&tar);
                ISFS_Deinitialize();
                return -1;
            }
        } else {
            printf("L-151: Failed to open ticket file for writing: %d\n", fd);
            free(ticket);
            mtar_close(&tar);
            ISFS_Deinitialize();
            return -1;
        }
    }
    s32 writeRet = ISFS_Write(fd, ticket, h.size);
    if (writeRet != h.size) {
        printf("L-160: Failed to write ticket data: expected %d, wrote %d\n", h.size, writeRet);
        ISFS_Close(fd);
        free(ticket);
        mtar_close(&tar);
        ISFS_Deinitialize();
        return -1;
    }

    printf("Ticket has been saved to NAND...!\n");

    ISFS_Close(fd);
    free(ticket);
    mtar_close(&tar);
    return 0;
}
