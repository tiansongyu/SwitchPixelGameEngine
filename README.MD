# SwitchPixelGameEngine
**SwitchPixelGameEngine** is a PixelGameEngine for Nintendo switch. 

An easy-to-use PixelGameEngine for Nintendo Switch homebrew game

Curious about what's next? See my to-do list on [Trello](https://trello.com/b/B6yaX5u3/switchpixelgameengine)

# How to use it? 

- **Pleaase view the [Wiki](https://github.com/tiansongyu/SwitchPixelGameEngine/wiki)(WIP) to know the Detail**
- **You can find some Demo [from here](https://github.com/tiansongyu/Example_SwitchPixelGameEngine)(WIP)**


## Incorporating into your Project
### 1. Install packages through (dkp-)pacman :

```
switch-freetype
switch-libpng
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

# Features 

- 基本图形函数(basic graphic function)
- 文字显示(display strings)
- 帧率显示(display FPS)
- 按键控制(keyboard input)
- 触屏控制(touch input)
- 更改像素块大小(change the size of pixel)
- 更改分辨率(chage the image resolution)
- 支持透明度(transparency support)

# Credits
- Thanks [Javidx9](https://github.com/OneLoneCoder) who inspired me to achieve it and his [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) !

- WinterMute, fincs and [devkitPro](https://devkitpro.org/) contributors for devkitARM, devkitA64 and [dkp-pacman](https://github.com/devkitPro/pacman/releases).

- [Yellows8](https://github.com/yellows8) and all the mantainers for [switch-examples](https://github.com/switchbrew/switch-examples).

