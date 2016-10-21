#ifndef RECT_HPP
#define RECT_HPP

template <typename T>
class Rect {
public:
    Rect() = default;
    Rect(T left, T top, T width, T height)
        : left { left }
        , top  { top }
        , width { width }
        , height { height }
    
    T left = 0;
    T top = 0;
    T width = 0;
    T height = 0;
};

#endif