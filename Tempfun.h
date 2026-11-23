#pragma once
#ifndef TEMPFUN_H
#define TEMPFUN_H

#include "raylib.h"

class DiagramNode {
    private: 
        bool DisplayMainCircle;
        bool DisplaySubCirclePC;
        bool DisplaySubCircleLB;
        bool DisplaySubCircleCM;
        bool DisplaySubCircleAH;
        bool PCDisplayLine; 
        bool LBDisplayLine;
        bool CMDisplayLine;
        bool AHDisplayLine; 
        bool DisplayButtons;

    public:
        DiagramNode();

        // Circle
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;
        void SetCircle(bool MCircle, bool CirclePC, bool CircleLB, bool CircleCM, bool CircleAH);       
        bool ShowMainCircle() const;
        bool ShowSubCirclePC() const;
        bool ShowSubCircleLB() const;
        bool ShowSubCircleCM() const;
        bool ShowSubCircleAH() const;
        void ToggleMainCircle(); 
        void ToggleSubCirclePC();
        void ToggleSubCircleLB(); 
        void ToggleSubCircleCM();
        void ToggleSubCircleAH();

        // Line
        void DrawLineV(Vector2 startPos, Vector2 endPos, Color color) const;
        void SetLine(bool PCLine, bool LBLine, bool CMLine, bool AHLine); 
        bool ShowPCLine() const;
        bool ShowLBLine() const;
        bool ShowCMLine() const;
        bool ShowAHLine() const; 
        void ToggleLinePC();
        void ToggleLineLB(); 
        void ToggleLineCM();
        void ToggleLineAH();
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;
        bool Button(Rectangle bounds, const char* text) const;        
        void SetB(bool SButton);
        void ToggleB();
};

#endif // TEMPFUN_H