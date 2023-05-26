
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "libs/file.h"
#include "libs/colorUtil.h"
#include "libs/console.h"

#include "assets/cat.rgb555.txt"


// change this to 1 or 0
#define GENERATE_LUT 0

//CHAR_INFO aCHARINFO_LUT[32*32*32];

const int app_screen_width = 190;
const int app_screen_height = 60;
bufferConsole_t buffer;


int draw_image() {
    HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hnd == INVALID_HANDLE_VALUE) {
        printf("Window handle is invalid!\n");
        return EXIT_FAILURE;
    }
    SetConsoleTitle("Image test");

    CONSOLE_SCREEN_BUFFER_INFOEX info = {0};
    info.cbSize = sizeof(info);
    if (GetConsoleScreenBufferInfoEx(hnd, &info) == 0) {
        printf("GetConsoleScreenBufferInfoEx failed\n");
        return EXIT_FAILURE;
    }

    // screen max size
    COORD maxScreen = GetLargestConsoleWindowSize(hnd);
    if ((maxScreen.X < app_screen_width) || (maxScreen.Y < app_screen_height)) {
        printf("Screen is too small!\n");
        printf("Max supported: width(%i) height(%i)\n", maxScreen.X, maxScreen.Y);
        printf("Needed: width(%i) height(%i)\n", app_screen_width, app_screen_height);
        return EXIT_FAILURE;
    }
    console_set_size(hnd, app_screen_width, app_screen_height);
    console_set_canvasSize(hnd, app_screen_width-1, app_screen_height-1);

    /* buffer setup */
    memset(&buffer, 0, sizeof(buffer));
    if (0 != bufferConsole_new(&buffer, hnd, app_screen_width, app_screen_height)) {
        printf("Can't init the console\n");
        return EXIT_FAILURE;
    }

    // copy image
    for (int y = 0; y < app_screen_height; y++) {
        for (int x = 0; x < app_screen_width; x++) {
            uint32_t col = cat_rgb555[y*app_screen_width + x];
            buffer.pixels[y*app_screen_width + x] = color_RGB555toRGBA32(col, 255);
        }
    }

    bufferConsole_update(&buffer);
    //bufferConsole_swap(&buffer);
    //console_cls(buffer.hnd);
    return 0;
}


int main(void) {

    #if GENERATE_LUT == 1
    printf(" Bruteforce table generation. Please wait a few minutes..");
    printf("\n The file path is: \"./debug/debug.txt\"");
    charinfo_genPixTable(aCHARINFO_LUT);
    #else
    draw_image();
    #endif

    time_t endwait = time(NULL) + 10;
    while (time (NULL) < endwait) {
    }

    return EXIT_SUCCESS;
}

