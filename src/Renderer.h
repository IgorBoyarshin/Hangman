#include "Drawer.h"
#include <unordered_map>
#include <ncurses.h>


class Renderer : public Drawer {
    public:
        Renderer(unsigned int height, unsigned int width);

        // TODO: change into more abstract ones
        void _move(unsigned int y, unsigned int x) override;
        void _addstr(const std::string& string) override;
        void _flushinp() override;
        void _addch(int c) override;
        void _attron(int attr) override;
        void _attroff(int attr) override;
        void _refresh() override;
        void _endwin() override;
        void _keypad() override;
        void _initscr() override;
        void _noecho() override;
        void _start_color() override;
        void _erase() override;

        void setColor(const Color& color) override;

    public:
        /* static const int _ACS_ULCORNER; */
        /* static const int _ACS_URCORNER; */
        /* static const int _ACS_LLCORNER; */
        /* static const int _ACS_LRCORNER; */
        /* static const int _ACS_HLINE; */
        /* static const int _ACS_VLINE; */
        /* static const int _A_BOLD; */
        /* static const int _A_UNDERLINE; */
        /* static const int _KEY_DOWN; */
        /* static const int _KEY_UP; */
        /* static const int _KEY_LEFT; */
        /* static const int _KEY_RIGHT; */
        /* static const int _KEY_DC; */

    private:
        unsigned int m_NextAvailableColorIndex = 1;
        // [FG][BG] = ColorPair
        std::unordered_map<unsigned int,
            std::unordered_map<unsigned int, unsigned int>> m_Colors;

        const unsigned int m_Height;
        const unsigned int m_Width;
};
