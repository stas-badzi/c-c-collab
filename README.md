# c-c-collab
[Home page](https://stasbadzi.w.staszic.waw.pl)

# Terminal Emulators

## Windows

### __Conhost ✅__
- Windows default terminal

### __Mintty ✅__
- Cygwin's, MSYS2's & Git Bash's default terminal

### __Windows Terminal ✅⚠️__
- Default terminal for newer Windows versions
- ⚠️ For best preformance `⚙️Settings`->`🖌️Appearance`->`Use active terminal titile as application title` must be on

### __ConEmu ✅__
- Might not work in future versions if the `-cmd` argument is removed
  - if that happens, please open an issue (it's shouldn't be hard to fix, but I need to know the issue exists)

### __Cmder ✅⚠️__
- Mouse only works you launch a tab with `bash\mintty`

### __Hyper ✅⚠️__
- ⚠️ Popup windows use the default terminal
  - (because Hyper doesn't have a way to launch the terminal with a command)

### __MobaXterm ❌__
- Windows console api just doesn't work -> no way to fix it (I think)
  - if you know how to go around that, please open an issue

### __FireCMD ❌__
- Windows console api doesn't work and neither xterm control sequences
    - i don't know if there's a way to fix it

# Linux (the code for popups isn't there yet, but these were tested on another program)

### __Gnome Terminal ✅__
- Default terminal for Gnome environment

### __Konsole ✅__
- Default terminal for KDE environment

### __Xfce Terminal ✅__

- Default terminal for XFCE environment
### __Mate Terminal ✅__
- Default terminal for MATE environment

### __Xterm (UXterm) ✅__

### __And More__  ✅ (most should just work out of the box)

#

## Cross-Platform

### __Tabby Terminal ✅✅❌⚠️__
- Tested on windows ✅
- MacOS should work, but not tested ✅⚠️
- Linux shouldn't work (Window's also didn't work, but it's fixed) ❌⚠️
  
### __Warp ❌❌✅⚠️__
- Windows ❌
  - Mouse only works when the terminal is in alternate buffer mode
  - To be implemented (maybe)

- Linux ❌
  - Weird ways of launching the terminal
  - (Todo)

- MacOS ✅⚠️
  - Not tested (probably won't be until I get my hackickintosh working with a grafix card)


# Tecnical
## windows key on linux
- my laptop: left -> 125 right -> non-existent(I think 126), submit an issue if you have it as a different keycode, than I might add testing for hardware

# Download
[onedrive](https://1drv.ms/u/c/2c1a62828b4f65ed/EfSOz37OnQlGnHiaBQPvhUYB_A3Bu39x9FoMp-q-yTzDrg?e=fh4xBW)
