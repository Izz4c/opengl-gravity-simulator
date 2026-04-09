#include <iostream>

#include <GL/glew.h>
#include "physics.h"

#include "drawer.h"

#define SCREEN_WIDTH 1200.0f
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
    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to initialize GLEW, PANIC!" << std::endl;
        return nullptr;
    }
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        45.0,                          // FOV angle
        SCREEN_WIDTH / SCREEN_HEIGHT,  // Aspect ratio
        0.1,                           // Near clip
        1000.0                         // Far clip
    );

    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}


int main(){
    GLFWwindow* window = StartGLFW();
    std::vector<Object> objs = {                                                              // All the objects in scene
        Object(std::vector<float>{0, 0,-30.0f},   std::vector<float>{-0.2f,0.0f,0.0f },          50.0f,20.0f ,Color(1,0.5f,0)),
        Object(std::vector<float>{0, 200,-30.0f}, std::vector<float>{2.0f,0.0f,0.0f },          20.0f,2.0f ,Color(0.5f,1,0)),
        Object(std::vector<float>{-70.71f, 70.71f,-30.0f}, std::vector<float>{0.0f,2.0f,3.0f },10.0f,0.01f ,Color(0,1,1)),
        Object(std::vector<float>{-100.0f, 0.0f,-30.0f}, std::vector<float>{-1.0f,0.0f,-3.0f },10.0f,0.01f ,Color(0,1,1)),
        Object(std::vector<float>{0.0f, 100.0f,-30.0f}, std::vector<float>{0.0f,-1.0f,-3.0f },10.0f,0.01f ,Color(0,1,1)),
        Object(std::vector<float>{70.71f, -70.71f,-30.0f}, std::vector<float>{0.0f,0.0f,3.0f },10.0f,0.01f ,Color(0,1,1)),
        Object(std::vector<float>{100.0f, 0.0f,-30.0f}, std::vector<float>{0.0f,2.0f,-3.0f },10.0f,0.01f ,Color(0,1,1))
    };
    float camX = 0.0f, camY = -50.0f, camZ = 500.0f;    // Camera Movement (keyboard) control variables

    float yaw = -60.0f, pitch = 0.0f;                   
    float lastX = 400, lastY = 300, x_Offset, y_Offset; // Mouse control variables
    float dirX,dirY,dirZ;          // Direction the camera's facing
    int fbWidth, fbHeight;         // frame buffer dimensions
    float speed = 3.0f;            // camera movement speed
    float sensitivity = 0.05f;     // mouse sensitivty
    while(!glfwWindowShouldClose(window)){
        
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        
        int screenWidth = fbWidth;
        int screenHeight = fbHeight;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Keyboard WASD controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camX += dirX * speed;
            camY += dirY * speed;
            camZ += dirZ * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camX -= dirX * speed;
            camY -= dirY * speed;
            camZ -= dirZ * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camX -= (dirZ) * speed; 
            camZ += (dirX) * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camX += (dirZ) * speed;
            camZ -= (dirX) * speed;
        }

        // Mouse controls
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        x_Offset = (mouseX - lastX) * sensitivity;
        y_Offset = (lastY - mouseY) * sensitivity; // Reversed: y ranges bottom to top
        lastX = mouseX;
        lastY = mouseY;
        yaw   += x_Offset;
        pitch += y_Offset;

        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f; // Clamping b/w -90 and +90

        dirX = cos(yaw * M_PI/180.0f) * cos(pitch * M_PI/180.0f);  // Converting polar (pitch&yaw) coordinates to cartesian (X,Y,Z)
        dirY = sin(pitch * M_PI/180.0f);
        dirZ = sin(yaw * M_PI/180.0f) * cos(pitch * M_PI/180.0f);

        gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, 0.0f, 1.0f, 0.0f); // Transform camera

        // Physics and Graphics
        for(int i=0;i<objs.size();i++){
            if(i==0){   // Watching position of first object (change i==object number)
                printf("Position X : %f\nPosition Y : %f",objs[i].position[0],objs[i].position[1]); 

            }
            for(int j=0;j<objs.size();j++){
                if(i==j) continue;
                objs[i].CollisionCheck(&objs[j]);
                std::vector<float> relativePos = subtract(objs[i].position,objs[j].position);
                if(i==0&&j==1){
                    printf("\nDistance from central body : %f",length(relativePos));
                }
                std::vector<float> grav = scale(normalize(relativePos),-GRAV_CONST/pow(length(relativePos),2));
                objs[i].accelerate(grav[0]*objs[j].mass,grav[1]*objs[j].mass,grav[2]*objs[j].mass);
                objs[j].accelerate(-grav[0]*objs[i].mass,-grav[1]*objs[i].mass,-grav[2]*objs[i].mass); // Newton's third law (Opposite reaction thing)
            }

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
