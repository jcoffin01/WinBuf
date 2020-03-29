# WinBuf/WinStream

## Intro

This project contains a small C++ stream buffer class to write
directly to a Windows console. Along with the normal stream capabilities, it supports clearing the screen by writing `cls()` to the stream, writing to a particular screen position by writing `gotoxy(x, y)` to the stream, and setting the foreground/background color by writing color names to the stream. Names supported are:

- red
- green
- blue
- intense
- red_back
- green_back
- blue_back
- intenst_back

These can be bit-wise `OR`ed together, so you can get medium grey on a blue background with code on this general order:

```cpp
auto color = red | green | blue | blue_back;

WinStream ws;

ws << color << "this is some text\n";
```

Since `|` has lower precedence than `<<`, if you want to generate a color "inline", you need to enclose any `OR`ed values in parentheses:

```cpp
ws << (red | green | blue | blue_back ) << "Some text\n";
```

## Building

To build with Microsoft tools:

```
mkdir build
cd build
cmake .. -G "NMake Makefiles"
NMake
```

To build with MinGW:

```
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="<path to mingw>\make"
make
```

That should build a WinBuf library (WinBuf.lib or WinBuf.a, as applicable) and a TestStatic.exe, which should run a small test, clearing the screen to grey text on a blue background, and writing a little bit of test in a couple different colors.

The file test_Winbuf.cpp contains a quick demonstration of how to use the code. It's what builds to the testStatic.exe
