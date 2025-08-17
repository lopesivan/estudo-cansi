#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "play_audio.h"

void handle_sigint(int sig) {
    play_audio("meow.mp3");
}

int main() {
    // Initialize SDL and SDL_mixer
    if (init_mixer() != 0) return 1;
    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);

    const char* message = "Subscribe";
    printf("Process ID: %d\n", getpid());
    // Message printing loop
    while (1) {
        printf("\r                 ");
        printf("\r%s", message);
        fflush(stdout);
        sleep(1);
        for (int i = 0; i < 3; i++) {
            printf(".");
            fflush(stdout);
            sleep(1);
        }
    }
    // Clean up SDL and SDL_mixer before exiting
    clean_mixer();
    return 0;
}