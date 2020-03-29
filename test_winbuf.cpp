#include "winbuf.hpp"

int main()
{
    WinStream w;

    auto color = red | green | blue | blue_back;

    w << color << cls() << gotoxy(10, 4) << "This is a string\n";
    for (int i = 0; i < 10; i++)
        w << "Line: " << i << "\n";

    w << (green | blue_back);

    for (int i = 0; i < 10; i++)
        w << "Line: " << i + 10 << "\n";

    w << gotoxy(20, 10) << "Stuck in the middle with you!\n";
}
