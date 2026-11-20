#include "Tempfun.h"

void MainInterface::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}
void MainInterface::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}
void SubInterfaceCompPC::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}
void SubInterfaceCompPC::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}
void SubInterfaceCompPC::DrawLineV(Vector2 startMainToPC, Vector2 endMainToPC, Color color) const {
    ::DrawLineV(startMainToPC, endMainToPC, color);
}
void SubInterfaceCompLB::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}
void SubInterfaceCompLB::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}
void SubInterfaceCompLB::DrawLineV(Vector2 startMainToLB, Vector2 endMainToLB, Color color) const {
    ::DrawLineV(startMainToLB, endMainToLB, color);
}
void SubInterfaceCompCM::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}
void SubInterfaceCompCM::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}
void SubInterfaceCompCM::DrawLineV(Vector2 startMainToCM, Vector2 endMainToCM, Color color) const {
    ::DrawLineV(startMainToCM, endMainToCM, color);
}
void SubInterfaceCompAH::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}
void SubInterfaceCompAH::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}
void SubInterfaceCompAH::DrawLineV(Vector2 startMainToAH, Vector2 endMainToAH, Color color) const {
    ::DrawLineV(startMainToAH, endMainToAH, color);
}