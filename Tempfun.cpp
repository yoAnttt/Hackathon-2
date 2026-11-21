#include "Tempfun.h"

void DiagramNode::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}

void DiagramNode::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}

void DiagramNode::DrawLineV(Vector2 startPos, Vector2 endPos, Color color) const {
    ::DrawLineV(startPos, endPos, color);
}