#include "physics.h"

Object::Object(std::vector<float> position,std::vector<float> velocity,float radius, float mass){
    this->position = position;
    this->velocity = velocity;
    this->radius   = radius;
    this->mass     = mass;
}
Object::Object(std::vector<float> position,std::vector<float> velocity,float radius, float mass, Color color){
    this->position = position;
    this->velocity = velocity;
    this->radius   = radius;
    this->mass     = mass;
    this->color    = color;
}
// Accelerates this object
void Object::accelerate(float x, float y, float z){
    
    this->velocity[0] += x;
    this->velocity[1] += y;
    this->velocity[2] += z;
}
// Updates the position of this object using its velocity
void Object::updatePos(){
    this->position[0] += this->velocity[0];
    this->position[1] += this->velocity[1];
    this->position[2] += this->velocity[2];
}
// Draws this object at a resolution
void Object::DrawCircle(int resolution){
    // 1. Clear the matrix for this object
    glPushMatrix();

    // 2. Move the "Camera" or the Object
    // We move -10.0f on Z to put the object 10 units in front of the lens
    glTranslatef(this->position[0], this->position[1], this->position[2]); 

    // 3. Set the color
    glColor3f(this->color.r, this->color.g, this->color.b);

    // 4. Draw the Sphere using GLU
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, this->radius, 32, 32); // radius, slices, stacks
    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Checks if this object is colliding with another object
void Object::CollisionCheck(Object* object){
    std::vector<float> relative = subtract(object->position,this->position);        // Relative position of other object

    float distance = length(relative);                                              // Distance between this object and other object     
    if(distance<object->radius+this->radius){                                       // If distance less than combined radius
        
        std::vector<float> normal_relative = normalize(relative);                   // Normalize the relative position vector

        std::vector relative_velocity = subtract(object->velocity,this->velocity);  // Find relative velocity of other object

        float relVel = dot(normal_relative,relative_velocity);                      // Measures how fast the object is approaching
                                                                                    // this object
        if(relVel < 0){                                                             // If its approaching us
            float invMassA = 1.0f / this->mass;
            float invMassB = 1.0f / object->mass;
            float j = -(1 + 0.9) * relVel / (invMassA + invMassB);
            std::vector<float> impulseVec = scale(normal_relative, j);            // Momentum exchange stuff
                                          
            this->velocity   = add(this->velocity,   scale(impulseVec,-invMassA)); // Bounce this object off
            object->velocity = add(object->velocity, scale(impulseVec,invMassB)); // Bounce other object off
        }
        float overlap    = 0.5f*(object->radius+this->radius - distance);           // Finds overlapping between the objects
        this->position   = add(  this->position,scale(normal_relative,-overlap));   // Move this object away from other object
        object->position = add(object->position,scale(normal_relative, overlap));   // Move other object away from this object
    }
}


double length(std::vector<float> vector){
    float mag = 0;
    for(int i=0;i<vector.size();i++){
        mag += vector[i]*vector[i];
    }
    return sqrt(mag);
}
std::vector<float> normalize(std::vector<float> vector){
    float mag = length(vector);
    std::vector<float> newvector = vector;
    for(int i=0;i<newvector.size();i++){
        newvector[i]/=mag;
    }
    return newvector;
}
std::vector<float> scale(std::vector<float> vector, float k){
    std::vector<float> newvector = vector;
    for(int i=0;i<newvector.size();i++){
        newvector[i]*=k;
    }
    return newvector;
}
float dot(std::vector<float> A, std::vector<float> B){
    float mag = 0;
    for(int i=0;i<A.size();i++){
        mag += A[i]*B[i];
    }
    return mag;
}
std::vector<float> add(std::vector<float> A, std::vector<float> B){
    std::vector<float> newvector = A;
    for(int i=0;i<newvector.size();i++){
        newvector[i]=A[i]+B[i];
    }
    return newvector;
}
std::vector<float> subtract(std::vector<float> A, std::vector<float> B){
    std::vector<float> newvector = A;
    for(int i=0;i<newvector.size();i++){
        newvector[i]=A[i]-B[i];
    }
    return newvector;
}