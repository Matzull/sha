#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int _width;
int _goal;
int _progress;
float _inc;
int last;

void init_bar(int sections)
{
    struct winsize ws;
    if (ioctl(1, TIOCGWINSZ, &ws) != -1)
    {
        _width = ws.ws_col - 8; // 5 + [ > ]
    }
    _goal = sections;
    _progress = 0;
    _inc = _width / (double)sections;
    printf("Bar init with _inc: %f %d / %d\n", _inc, _width, sections);
}

void update_bar()
{
    _progress += 1;
}

void print_bar()
{
    if ((int)(_inc * _progress) != last)
    {
        putchar('\r');
        putchar('[');
        last = (int)(_inc * _progress);
        for (int i = 0; i < (int)(_inc * _progress); i++)
        {
            putchar('=');
        }
        putchar('>');
        for (int i = 0; i < (int)(_inc * (_goal - _progress)); i++)
        {
            putchar(' ');
        }
        putchar(']');
        fflush(stdout);
    }
}