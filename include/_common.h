#ifndef _COMMON_H
#define _COMMON_H

#include<iostream>
#include<windows.h>
#include<string>

#include<gl/gl.h>
#include<GL/glut.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<unordered_map>
#include <vector>

#include<fstream>

#define PI 3.14159

using namespace std;

typedef struct{
    float x,y;
    } vec2;

struct vec3{
    float x,y,z,len;
    vec3(){
        x=y=z=0;

    }
    vec3(const vec3& newVec){
        x = newVec.x;
        y = newVec.y;
        z = newVec.z;
    }
    vec3 (float xNew, float yNew, float zNew){
        x = xNew;
        y = yNew;
        z = zNew;
    }
    void setVec(vec3 newVec){
        x = newVec.x;
        y = newVec.y;
        z = newVec.z;
    }
    float normalize(){
        float lenSquare = x*x + y*y + z*z;
        if(lenSquare > 0.001f) {
            float invertLength = 1.0f / sqrtf(lenSquare);
            x *= invertLength;
            y *= invertLength;
            z *= invertLength;
            return lenSquare;
        } else return 0;
    }
    vec3 operator+(const vec3& other) const {return vec3(x+other.x, y+other.y,z+other.z);}
    vec3 operator-(const vec3& other) const {return vec3(x-other.x, y-other.y,z-other.z);}
    vec3& operator+=(const vec3& o) {x += o.x; y += o.y; z += o.z; return *this;}
    vec3& operator-=(const vec3& o) {x -= o.x; y -= o.y; z -= o.z; return *this;}
    vec3 operator*(const vec3& other) const {
        return vec3(x*other.x, y*other.y,z*other.z);
    }
    vec3 operator*(const float& mult) const {
        return vec3(x*mult, y*mult,z*mult);
    }
};

typedef struct{
    float r;
    float g;
    float b;
    float a;
} col4;
enum Scene {
    MAIN,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    CREDITS,
    HELP,
    QUIT
};
enum ItemType {
    GADGET,
    GIZMO,
    WEAPON,
    TRAINING
};
enum CAPSULE_STATE {
    DESPAWNED,
    SPAWNED,
    FALLING,
    ONGROUND,
    COLLECTED
};

#endif // _COMMON_H
