shake2skip
==========

Shake2Skip allows you to skip the current song by (you guessed it) shaking the phone. Tested on N950, should work on N9. 

[Here](http://www.youtube.com/watch?v=lwfXHgGAktc) is a short demo or shake2skip in action. Download the latest .deb for N9[50] from [here](https://github.com/scifi-guy/shake2skip/tree/master/downloads).


Some Notes
==========

- This application is installed as a daemon and is started automatically on boot (it takes a minute after boot in N950). No need launch the application manually. The application/daemon is only active when music is being played.

- If the music is already playing by the time application starts, the "shakes" are recognized from the next song or after pause/play. This is a known bug.

- In Harmattan, accelerometer works only when the display is on. As a result, shake is recognized  only when the display is on. An advantage of this is that songs are not changed when you go for a walk with the phone in your pocket. 

- Rename the file /opt/shake2skip/config/settings.ini.template to settings.ini and adjust the settings if necessary.

- Although I tried to avoid accidental triggers (when walking etc), there could be bugs. This is just an experimental release. Use at your own risk.
