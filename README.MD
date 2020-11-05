# SwitchPixelGameEngine
**SwitchPixelGameEngine** is a PixelGameEngine for Nintendo switch. 

An easy-to-use PixelGameEngine for Nintendo Switch homebrew game

Curious about what's next? See my to-do list on [Trello](https://trello.com/b/B6yaX5u3/switchpixelgameengine)

# Example 

- **You can find some Demo [from here](https://github.com/tiansongyu/Example_SwitchPixelGameEngine)**

## Incorporating into your Project
### 1. Install packages through (dkp-)pacman :

```
devkit-env
devkitA64
libnx
switch-freetype
```

### 2. Add the header file

You need the one file - **SwitchPixelGameEngine.h** - included in your project!

### 3. Edit Makefile
**You'll need the following packages installed through (dkp-)pacman in order for Aether to compile:**

Add the following lines in **your** Makefile after `LIBS`  respectively.
```
CFLAGS	+=  `freetype-config --cflags`
LIBS    +=  `freetype-config --libs`
```

### 4. Done

Provides a fast, richly featured, cross platform pixel drawing and user interface framework for
 * The development of games
 * Visualisation of algorithms
 * Prototyping and experimentation
 * Education

## Features

- Runs on Switch. It is easy to port! 

- SwitchPixelGameEngine is actively maintained and developed!

# Documentation (WIP)
Please see https://github.com/tiansongyu/SwitchPixelGameEngine/wiki

# TODO: New Features 

- 添加声音播放(add audio function)
- 添加多线程   (add thread function)
- 整理函数位置结构 (clean the funcion code)
- RGBA分开存储 (clean the RGBA code)
- 更换字体颜色和大小 (add changing the font size and color)



# Credits
- Thanks [Javidx9](https://github.com/OneLoneCoder) who inspired me to achieve it and his [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) !

- WinterMute, fincs and [devkitPro](https://devkitpro.org/) contributors for devkitARM, devkitA64 and [dkp-pacman](https://github.com/devkitPro/pacman/releases).

- [Yellows8](https://github.com/yellows8) and all the mantainers for [switch-examples](https://github.com/switchbrew/switch-examples).

