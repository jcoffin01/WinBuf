#pragma once
#include <ios>
#include <ostream>
#include <windows.h>

//! A C++ streambuf that writes directly to a Windows console
class WinBuf : public std::streambuf
{
    HANDLE h;

public:
    //! Create a WinBuf from an Windows handle
    //! @param h handle to a Windows console
    WinBuf(HANDLE h) : h(h) {}
    WinBuf(WinBuf const &) = delete;
    WinBuf &operator=(WinBuf const &) = delete;

    //! Return the handle to which this buffer is attached
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

//! A C++ ostream that  writes via the preceding WinBuf
class WinStream : public virtual std::ostream
{
    WinBuf buf;

public:
    //! Create stream for a Windows console, defaulting to standard output
    WinStream(HANDLE dest = GetStdHandle(STD_OUTPUT_HANDLE))
        : buf(dest), std::ostream(&buf)
    {
    }

    //! return a pointer to the underlying WinBuf
    WinBuf *rdbuf() { return &buf; }
};

//! Provide the ability to set attributes (text colors)
class SetAttr
{
    WORD attr;

public:
    //! Save user's attribute for when this SetAttr object is written out
    SetAttr(WORD attr) : attr(attr) {}

    //! Support writing the SetAttr object to a WinStream
    //! @param w a WinStream object to write to
    //! @param c An attribute to set
    friend WinStream &operator<<(WinStream &w, SetAttr const &c)
    {
        WinBuf *buf = w.rdbuf();
        auto h = buf->handle();
        SetConsoleTextAttribute(h, c.attr);
        return w;
    }

    //! support combining attributes
    //! @param r the attribute to combine with this one
    SetAttr operator|(SetAttr const &r)
    {
        return SetAttr(attr | r.attr);
    }
};

//! Support setting the position for succeeding output
class gotoxy
{
    COORD coord;

public:
    //! Save position for when object is written to stream
    gotoxy(SHORT x, SHORT y) : coord{.X = x, .Y = y} {}

    //! support writing gotoxy object to stream
    friend WinStream &operator<<(WinStream &w, gotoxy const &pos)
    {
        WinBuf *buf = w.rdbuf();
        auto h = buf->handle();
        SetConsoleCursorPosition(h, pos.coord);
        return w;
    }
};

//! Clear the "screen"
class cls
{
    char ch;

public:
    //! Creat screen clearing object
    //! @param ch character to use to fill screen
    cls(char ch = ' ') : ch(ch) {}

    //! Support writing to a stream
    //! @param os the WinStream to write to
    //! @param c the cls object to write
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

//! Provide some convenience instances of the SetAttr object
//! to (marginally) ease setting colors.
extern SetAttr red;
extern SetAttr green;
extern SetAttr blue;
extern SetAttr intense;

extern SetAttr red_back;
extern SetAttr blue_back;
extern SetAttr green_back;
extern SetAttr intense_back;
