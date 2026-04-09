#ifndef PHYSICS_H
#define PHYSICS_H
#include <vector>
#include <GLFW/glfw3.h>
#include <cmath>
#include "drawer.h"
#include <GL/glu.h>
class Object{
    public:
    std::vector<float> position;
    std::vector<float> velocity;
    float mass;
    float radius;
    Color color;
    Object(std::vector<float> position,std::vector<float> velocity,float radius, float mass);
    Object(std::vector<float> position,std::vector<float> velocity,float radius, float mass, Color color);
    void accelerate(float x, float y, float z);
    void updatePos();
    void DrawCircle(int resolution);
    void CollisionCheck(Object* object);
};
double length(std::vector<float> vector);
float dot(std::vector<float> A, std::vector<float> B);
std::vector<float> normalize(std::vector<float> vector);
std::vector<float> scale(std::vector<float> vector, float k);
std::vector<float> add(std::vector<float> A, std::vector<float> B);
std::vector<float> subtract(std::vector<float> A, std::vector<float> B);

#endif