#pragma once
#include <ios>
#include <ostream>
#include <windows.h>

class WinBuf : public std::streambuf
{
    HANDLE h;

public:
    WinBuf(HANDLE h) : h(h) {}
    WinBuf(WinBuf const &) = delete;
    WinBuf &operator=(WinBuf const &) = delete;

    HANDLE handle() const { return h; }

protected:
    virtual int_type overflow(int_type c) override
    {
        if (c != EOF)
        {
            DWORD written;
            WriteConsole(h, &c, 1, &written, nullptr);
        }
        return c;
    }

    virtual std::streamsize xsputn(char_type const *s, std::streamsize count) override
    {
        DWORD written;
        WriteConsole(h, s, DWORD(count), &written, nullptr);
        return written;
    }
};

class WinStream : public virtual std::ostream
{
    WinBuf buf;

public:
    WinStream(HANDLE dest = GetStdHandle(STD_OUTPUT_HANDLE))
        : buf(dest), std::ostream(&buf)
    {
    }

    WinBuf *rdbuf() { return &buf; }
};

class SetAttr
{
    WORD attr;

public:
    SetAttr(WORD attr) : attr(attr) {}

    friend WinStream &operator<<(WinStream &w, SetAttr const &c)
    {
        WinBuf *buf = w.rdbuf();
        auto h = buf->handle();
        SetConsoleTextAttribute(h, c.attr);
        return w;
    }

    SetAttr operator|(SetAttr const &r)
    {
        return SetAttr(attr | r.attr);
    }
};

class gotoxy
{
    COORD coord;

public:
    gotoxy(SHORT x, SHORT y) : coord{.X = x, .Y = y} {}

    friend WinStream &operator<<(WinStream &w, gotoxy const &pos)
    {
        WinBuf *buf = w.rdbuf();
        auto h = buf->handle();
        SetConsoleCursorPosition(h, pos.coord);
        return w;
    }
};

class cls
{
    char ch;

public:
    cls(char ch = ' ') : ch(ch) {}

    friend WinStream &operator<<(WinStream &os, cls const &c)
    {
        COORD tl = {0, 0};
        CONSOLE_SCREEN_BUFFER_INFO s;
        WinBuf *w = os.rdbuf();
        HANDLE console = w->handle();

        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacter(console, c.ch, cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
        return os;
    }
};

extern SetAttr red;
extern SetAttr green;
extern SetAttr blue;
extern SetAttr intense;

extern SetAttr red_back;
extern SetAttr blue_back;
extern SetAttr green_back;
extern SetAttr intense_back;
