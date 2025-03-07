#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "gamelist.h"

#define ARROW " \x10"

#define resetscreen() printf("\x1b[2J")

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;


void disclaimer()
{
	printf("This software lets you download titles from Wii Shop Channel after shutdown.\n");
	printf("You must install the WADs that came with this tool in order to use this.\n");
	printf("This may or may not be a legal way to download titles.\n");
	printf("There isn't really a way to get these titles legally, so who cares\n\n");
	printf("No one was doxed with this software.\n");
	printf("Nor was the accusation of doxing on the Rii Shop project true.\n");
	printf("I know that my reputation in the Wii community is tarnished.\n");
	printf("My stay in the Wii community has been a journey\n\n");
	printf("Press A to go back");

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
		printf("Wii Shoplift Channel v1.2 - By Larsenv\n");
		if (sd)
		{
			printf("Press 1 to download to NAND\t\t\t\t\t\t");
		}
		else
		{
			printf("Press 1 to download to SD\t\t\t\t\t\t\t");
		}
		printf("Press 2 to view a disclaimer\n\n");

		if (pressed == WPAD_BUTTON_2)
		{
			disclaimer();
			resetscreen();
			goto MENU;
		}

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
			char title[61];
			sprintf(title, "/oss/serv/B_09.jsp?titleId=%016llx&country=%s%s", gamelist[selection].id, gamelist[selection].country, (sd) ? "&SD=Y" : "");
			WII_LaunchTitleWithArgs(0x0001000248414241LL, 0, title, NULL);
		}

		if (pressed == WPAD_BUTTON_HOME)
		{
			printf("exiting");
			return;
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

	game_selectionmenu();

	return 0;
}
