#include "Drawer.h"
#include <unordered_map>
#include <iostream>
#include <ncurses.h>
#include "Log.h"


class Renderer : public Drawer {
    public:
        Renderer();
        virtual ~Renderer();
        void goTo(unsigned int y, unsigned int x) override;
        void putstr(const std::string& string) override;
        void putch(char c) override;
        void putsch(SpecialChar specialChar) override;
        void setAttribute(Attribute attribute, bool on) override;
        void update() override;
        void init() override;
        void enableColors() override;
        void clear() override;
        void setColor(const Color& color) override;
    private:
        unsigned int m_NextAvailableColorIndex = 1;
        // [FG][BG] = ColorPair
        std::unordered_map<unsigned int,
            std::unordered_map<unsigned int, unsigned int>> m_Colors;
    private:
        void cleanup() override;
};
