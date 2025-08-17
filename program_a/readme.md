### Catch the SIGINT signal to make your process meow when Ctrl-C is pressed

Compile with:

```
gcc -o program_a program_a.c play_audio.c -lSDL2 -lSDL2_mixer
```

You might need to install sdl2.

```
sudo apt-get install libsdl2-dev libsdl2-mixer-dev
```
