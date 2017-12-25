# EXEPeek

This is my hobby project, where I initially planned to create a complete software that could parse Windows's PE file structure. Made using C++, while using Windows API for UI and other related functionality.

My goal was to create a PE structure viewer, that more or less same as [PEview](http://wjradburn.com/software/). But because of lack of time, and I currently focusing on other interests, so this is kinda abandoned project.

_**Please keep in mind that this project is half-finished, and at current rate is very unstable!**_

## Checklist

Here is the list of things that I had in mind when developing this project:

1) Create a complete PE structure viewer (far from complete)
2) Using Windows API (for UI and related tasks) without relying much on other libraries
3) Create a [freestanding executable](http://nullprogram.com/blog/2016/01/31/). This is to ensure that this program can run everywhere, without depending on too many dependencies.
4) Supporting another executable structure such as `ELF` or `Mach-O` format.

## Program Design

In order to make the project more cleaner, I've splitted the components into their appropriate folder. For example, all UI elements are reside in `gui` folder. All code are in C++, with `Object-Oriented programming` design.

1) `gui` - contains all code related to UI
2) `misc` - contains all miscellaneous code (such as opening file, data structure, etc)
3) `structures` - contains code to parse PE structure

## How to build?

Contrary to the famous opinion that when you're developing Windows program you must use Windows OS to do so.

This is not the case for me, as I'm relying 100% on open-source toolkits such as MinGW-w64 and Wine when developing this software while using Arch Linux as my main OS.

### Build guide (Arch Linux)

`$ pacman -S make cmake mingw-w64-gcc mingw-w64-headers wine-staging`

Once you installed all required things, go to main EXEPeek directory and run `./build.sh`, which will automagically build the software for you.

The compiled executable will be inside `build-win/`. You can use Wine in order to run the program.

## Screenshot

![image](https://user-images.githubusercontent.com/2675341/34339403-924c13aa-e9ae-11e7-8d82-0d75da08892a.png)

## What I've Learned?

This is my first time developing a serious project using Windows API's UI toolkit. This has been kinda on my checklist of things that I must learn.

Honestly, IMHO, developing a program that uses Windows API UI toolkit is a pain in the ass. You better get off using something like Qt or wxWidgets. YES, you can use Windows API UI toolkit when developing a simple program. But, please, don't ever try to use it when developing a large project.

The reason why I said *Windows API UI* bad is because developing UI in Windows API is hard, I bet you will spend 60% of your time figuring how to layout and setup UI things instead of spending time on the main thing, which is your program functionality.