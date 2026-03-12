// Draws a circle with the specified position and resolution
#include "drawer.h"
#include <cmath>

Color lerp_color(Color A, Color B, float para){
    Color newcolor = Color();
    newcolor.r = B.r*para + A.r*(1-para);
    newcolor.g = B.g*para + A.g*(1-para);
    newcolor.b = B.b*para + A.b*(1-para);
    return newcolor;
}