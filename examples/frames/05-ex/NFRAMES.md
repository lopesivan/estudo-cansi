
```{bash}
$ ffprobe -count_frames -show_entries stream=nb_frames -v quiet -of csv=print_section=0 arquivo.mp4
1149
1982

$ ffprobe -count_frames -show_entries stream=nb_frames -v quiet   arquivo.mp4
[STREAM]
nb_frames=1149
[/STREAM]
[STREAM]
nb_frames=1982
[/STREAM]
```

