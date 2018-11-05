#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>


struct Coord {
    public:
        // Origin: (0,0) === Upper-left corner
        int y; // row
        int x; // column

        Coord();
        Coord(int y, int x);

        Coord& operator+=(const Coord& coord) noexcept;
        Coord operator+(const Coord& coord) const noexcept;
};


struct Color {
    public:
        unsigned int fg;
        unsigned int bg;

        Color(unsigned int fg, unsigned int bg) noexcept;

        // TODO: replace with a more elegant solution
        static unsigned int BLACK;
        static unsigned int RED;
        static unsigned int GREEN;
        static unsigned int YELLOW;
        static unsigned int BLUE;
        static unsigned int PURPLE;
        static unsigned int CYAN;
        static unsigned int WHITE;
};


/* // TODO: rename */
/* class ColorFactory { */
/*     public: */
/*         static void setColor(const Color& color) noexcept; */
/*  */
/*     private: */
/*         static unsigned int m_NextAvailableIndex; */
/*         // [FG][BG] = ColorPair */
/*         static std::unordered_map<unsigned int, */
/*             std::unordered_map<unsigned int, unsigned int>> m_Colors; */
/* }; */


struct Tag {
    private:
        static const unsigned int m_EmptyTag;
        static unsigned int m_NextAvailableIndex;

    public:
        const unsigned int value;

        unsigned int operator()() const noexcept;
        bool isEmpty() const noexcept;

        static Tag createEmpty() noexcept;
        static Tag createNew() noexcept;

    private:
        Tag(unsigned int value) noexcept;
};


#endif
