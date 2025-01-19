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
}

void update_bar()
{
    _progress += 1;
}

void print_bar()
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
    printf(" %d%%", (int)(((double)(_progress + 1)/_goal)*100));//_progress + 1 is a "cheat" to appear 100% when the progress finishes
    fflush(stdout);
}

void print_bar_s()
{
    if ((int)(_inc * _progress) != last)
    {
        print_bar();
    }
}
