#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#include "gamelist.h"
#include "ticketman.h"
#include "iospatch.h"

#define ARROW " \x10"

#define resetscreen() printf("\x1b[2J")

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;


// larsenv/GlowWii/source/main.c @ 3655408
bool isDolphin(void)
{
    s32 checkDolphin;
    checkDolphin = IOS_Open("/dev/dolphin", IPC_OPEN_NONE);
    if (checkDolphin >= 0)
        return true;
    else
        return false;
}


void disclaimer()
{
	printf("\n\n\n\n\n\n\n\n");
	printf("This software lets you download titles from Wii Shop Channel after shutdown.\n");
	printf("You must install the WADs that came with this tool in order to use this.\n");
	// printf("This may or may not be a legal way to download titles.\n");
	printf("Installing too many titles can brick your system.\n\n");
	printf("The serpent told the Woman, \"You won't die. God knows that the\n");
	printf("moment you eat from that tree, you'll see what's really going on.\n");
	printf("You'll be just like God, knowing everything, ranging all the way\n");
	printf("from good to evil.\"\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tGenesis 3:4-5\n\n");
	printf("Press A to view the game list.");

	while (1)
	{
		WPAD_ScanPads();

		u32 pressed = WPAD_ButtonsDown(0);

		if (pressed & WPAD_BUTTON_A)
		{
			return;
		}
	}
}


void game_selectionmenu()
{
	int i, selection = 0, page = 0;

	bool sd = false;

MENU:
	while (true)
	{
		u32 pressed;

		WPAD_ScanPads();

		VIDEO_WaitVSync();

		resetscreen();

		pressed = WPAD_ButtonsDown(0);

		printf("\x1B[%d;%dH", 3, 0); // move console cursor to y/
		printf("Wii Like To Party\n");
		if (sd)
		{
			printf("Downloading to SD card (1) \t\t\t\t\t\t");
		}
		else
		{
			printf("Downloading to NAND (1)\t\t\t\t\t\t\t");
		}
		printf("Press HOME to exit the app.\n\n");

		for (i = page * 16; i < (page * 16) + 16; i++)
		{
			if (i < game_count)
			{
				printf("%s %s\n", ((selection == i) ? ARROW : "  "), gamelist[i].title);
			}
		}

		if (pressed == WPAD_BUTTON_UP)
		{
			if (selection > 0)
			{
				selection--;

				if (selection % 16 == 0)
				{
					page--;
				}
			}
			else
			{
				selection = game_count - 1;
				page = game_count / 16;
			}
		}

		if (pressed == WPAD_BUTTON_DOWN)
		{
			if (selection < game_count)
			{
				selection++;

				if (selection % 16 == 0)
				{
					page++;
				}
			}
			else
			{
				selection = 0;
				page = 0;
			}
		}

		if (pressed == WPAD_BUTTON_RIGHT)
		{
			if (selection < game_count && (page * 16) + 16 < game_count)
			{
				selection = (selection % 16) + (page * 16) + 16;
				page++;
			}
			else
			{
				selection = 0;
				page = 0;
			}
		}

		if (pressed == WPAD_BUTTON_LEFT)
		{
			if (selection > 0 && (page * 16) + 16 < game_count)
			{
				selection = (selection % 16) + (page * 16) - 16;
				page--;
			}
			else
			{
				selection = game_count - 1;
				page = game_count / 16;
			}
		}

		if (pressed == WPAD_BUTTON_1)
		{
			sd = !sd;
		}

		if (pressed == WPAD_BUTTON_A)
		{
			resetscreen();
			// int result =
			hard_ticket_to_nand(gamelist[selection].id);
			printf("\n\nPress A to open the Wii Shop Channel.\n");
			printf("Press B to go back.\n");
			while (1)
			{
				WPAD_ScanPads();

				u32 pressed = WPAD_ButtonsDown(0);

				if (pressed & WPAD_BUTTON_A)
				{
					// if (result == 0) // ok
					// {
					char title[61];
					sprintf(title, "/oss/serv/B_09.jsp?titleId=%016llx&country=%s%s", gamelist[selection].id, gamelist[selection].country, (sd) ? "&SD=Y" : "");
					WII_LaunchTitleWithArgs(0x0001000248414241LL, 0, title, NULL);
					// }
				}
				if (pressed & WPAD_BUTTON_B)
				{
					goto MENU;
				}
			}
		}

		if (pressed == WPAD_BUTTON_HOME)
		{
			printf("Exiting...");
			exit(0);
		}
	}
}


//---------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
	// SYS_STDIO_Report(true);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(false);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if (rmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();

	PAD_Init();
	WPAD_Init();

	if (AHBPROT_DISABLED && !isDolphin())
	{
		IOSPATCH_Apply();  // this gets past ISFS error -102
	}
	disclaimer();
	resetscreen();
	game_selectionmenu();

	exit(0);
}
