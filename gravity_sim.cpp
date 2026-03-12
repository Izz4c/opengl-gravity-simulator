#include <iostream>
#include <GLFW/glfw3.h>
#include "drawer.h"
#include "physics.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f
#define GRAV_CONST 21.0f
GLFWwindow* StartGLFW(){
    if(!glfwInit()){
        std::cerr<<"Failed to initialize GLFW, PANIC!" << std::endl;
        return nullptr;
    
    }
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"gravity sim", NULL,NULL);
    
    if (!window){
        std::cerr<<"Failed to create GLFW window, PANIC!" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return window;
}


int main(){
    GLFWwindow* window = StartGLFW();
    std::vector<Object> objs = {
        Object(std::vector<float>{400.0f, 500.0f,0}, std::vector<float>{2.1f,0.0f ,0   },25.0f,8.0f ,Color(1,0,0)),
        Object(std::vector<float>{400.0f, 300.0f,0}, std::vector<float>{0.0f,0.0f ,0   },35.0f,30.0f,Color(1,1,0)),
	    Object(std::vector<float>{600.0f, 300.0f,0}, std::vector<float>{0.0f,-2.1f,0.0f},20.0f,4.0f,Color(0,0,1))
    };

    int fbWidth, fbHeight;
    
    while(!glfwWindowShouldClose(window)){
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        int screenWidth = fbWidth;
        int screenHeight = fbHeight;
        glClear(GL_COLOR_BUFFER_BIT);
        for(int i=0;i<objs.size();i++){
            if(i==0){
                printf("Position X : %f\nPosition Y : %f",objs[i].position[0],objs[i].position[1]);

            }
            if(objs[i].position[0]<objs[i].radius || objs[i].position[0]+objs[i].radius>screenWidth){
                objs[i].velocity[0]*=-0.95f;
                objs[i].position[0] = (objs[i].position[0]<objs[i].radius) ? objs[i].radius : screenWidth-objs[i].radius;
            }
            if(objs[i].position[1]<objs[i].radius || objs[i].position[1]+objs[i].radius>screenHeight){
                objs[i].velocity[1]*=-0.95f;
                objs[i].position[1] = (objs[i].position[1]<objs[i].radius) ? objs[i].radius : screenHeight-objs[i].radius;
            }
            for(int j=0;j<objs.size();j++){
                if(i==j) continue;
                objs[i].CollisionCheck(&objs[j]);
                std::vector<float> relativePos = subtract(objs[i].position,objs[j].position);
                if(i==0&&j==1){
                    printf("\nDistance from central body : %f",length(relativePos));
                }
                std::vector<float> grav = scale(normalize(relativePos),-GRAV_CONST/pow(length(relativePos),2));
                objs[i].accelerate(grav[0]*objs[j].mass,grav[1]*objs[j].mass,0);
                objs[j].accelerate(-grav[0]*objs[i].mass,-grav[1]*objs[i].mass,0); // Newton's third law (Opposite reaction thing)
            }

            //objs[i].accelerate(0.0f,-9.81/20.0);
            objs[i].updatePos();
            objs[i].DrawCircle(8);
            if(i==0){
                printf("\033[2A\r");
            }
        }
        glEnd();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }
    glfwTerminate();
}
