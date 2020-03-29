winbuf.lib: winbuf.obj
    lib -out:WinBuf.lib winbuf.obj

WinBuf.obj: WinBuf.hpp WinBuf.cpp
    cl -c WinBuf.cpp

clean:
    -del *.obj
	-del test_winbuf.exe

test: test_winbuf.obj winbuf.lib
    cl test_winbuf.obj winbuf.lib
	test_winbuf
