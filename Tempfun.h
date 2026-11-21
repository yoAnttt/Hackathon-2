#pragma once
#ifndef TEMPFUN_H
#define TEMPFUN_H

#include "raylib.h"

class DiagramNode {
    public:
        void DrawCircle(int centerX, int centerY, float radius, Color color) const;
        void DrawCircleLines(int centerX, int centerY, float radius, Color color) const;
        void DrawLineV(Vector2 startPos, Vector2 endPos, Color color) const;
};

#endif // TEMPFUN_H