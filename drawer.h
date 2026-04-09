#ifndef DRAW_H
#define DRAW_H

struct Color{
    float r;float g;float b;
    Color() : r(0),g(0),b(0){}
    Color(float red, float green, float blue):r(red),g(green),b(blue){}
    
};
Color lerp_color(Color A, Color B, float para);
#endif
