#pragma once
#ifndef TEMPFUN_H
#define TEMPFUN_H
#include "raylib.h"

class MainInterface { // MainInterface class definition
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;         // Draw a color-filled circle: Used Raylib cheatsheet line 6-8
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;    // Draw circle outline
};
class SubInterfaceCompPC { // Sub-Interface Componet Power Compartment class definition
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;         
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;    
        void DrawLineV(Vector2 startMainToPC, Vector2 endMainToPC, Color color) const;               // Draw a line (using gl lines)
};
class SubInterfaceCompLB { // Sub-Interface Componet Logic Board class definition
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;         
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;    
        void DrawLineV(Vector2 startMainToLB, Vector2 endMainToLB, Color color) const;                
};
class SubInterfaceCompCM { // Sub-Interface Componet Control Module class definition
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;         
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;    
        void DrawLineV(Vector2 startMainToCM, Vector2 endMainToCM, Color color) const;               
};
class SubInterfaceCompAH { // Sub-Interface Componet Audio and Haptic
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;       
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;    
        void DrawLineV(Vector2 startMainToAH, Vector2 endMainToAH, Color color) const;                
};
#endif // TEMPFUN_H_ 