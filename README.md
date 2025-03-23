# lejeuquiressembleacachecachemaiscestunpaintball

This game is a multiplayer hunting experience! 🎯  

Press **Z** to move in the direction of your mouse. As the hunter, you can shoot, but you won’t be able to see your enemies directly. However, you can fire **paintballs** with the **left click**, marking opponents with paint. Once you spot them, use the **right click** to take them down with a stab.  

Sound plays a crucial role too—**3D audio footprints** help you track enemies by listening to their movements.  

To keep things fresh, the **map is procedurally generated**, ensuring endless replayability!

## How to play

When you launch the game, you can either simply try by going solo, or creating
a server and then wait for your friends to log in. (The port is 8080 by default.)

Inside the menus, to change of focused text input, press Tab.

Keys:

  Z - Advance (W on QWERTY keyboards)
  Left click - Send paintball (hunter only)
  Right click - Stab (hunter only)


## How to build

Simply create a `build` directory. Then,

`cd build && cmake ..`

Then go back to project root and launch the game with

`make -C build/ && ./build/paintball`
