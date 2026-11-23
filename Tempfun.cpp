#include "Tempfun.h"

DiagramNode::DiagramNode() { // Used Gemini to debug the layout
    DisplayMainCircle = false; 
    DisplaySubCirclePC = false;
    DisplaySubCircleLB = false;
    DisplaySubCircleCM = false;
    DisplaySubCircleAH = false;
    PCDisplayLine = false;
    LBDisplayLine = false;
    CMDisplayLine = false;
    AHDisplayLine = false;
    DisplayButtons = false;
    DisplayButtons = false;
}

// Circle Display
void DiagramNode::DrawCircle(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircle(centerX, centerY, radius, color);
}

bool DiagramNode::ShowMainCircle() const {
    return DisplayMainCircle;
} 

bool DiagramNode::ShowSubCirclePC() const {
    return DisplaySubCirclePC;
}

bool DiagramNode::ShowSubCircleLB() const {
    return DisplaySubCircleLB;
}

bool DiagramNode::ShowSubCircleCM() const { 
    return DisplaySubCircleCM; 
}

bool DiagramNode::ShowSubCircleAH() const {
    return DisplaySubCircleAH; 
}

void DiagramNode::SetCircle(bool MCircle, bool CirclePC, bool CircleLB, bool CircleCM, bool CircleAH) {
    DisplayMainCircle = MCircle;
    DisplaySubCirclePC = CirclePC;
    DisplaySubCircleLB = CircleLB;
    DisplaySubCircleCM = CircleCM;
    DisplaySubCircleAH = CircleAH;
}

void DiagramNode::ToggleMainCircle() {
    DisplayMainCircle = !DisplayMainCircle;
}

void DiagramNode::ToggleSubCirclePC() {
    DisplaySubCirclePC = !DisplaySubCirclePC;
}

void DiagramNode::ToggleSubCircleLB() {
    DisplaySubCircleLB = !DisplaySubCircleLB;
}

void DiagramNode::ToggleSubCircleCM() {
    DisplaySubCircleCM = !DisplaySubCircleCM;
}

void DiagramNode::ToggleSubCircleAH() {
    DisplaySubCircleAH = !DisplaySubCircleAH;
}

// Line Display
void DiagramNode::DrawLineV(Vector2 startPos, Vector2 endPos, Color color) const {
    ::DrawLineV(startPos, endPos, color);
}

bool DiagramNode::ShowPCLine() const {
    return PCDisplayLine;
}

bool DiagramNode::ShowLBLine() const {
    return LBDisplayLine;
}

bool DiagramNode::ShowCMLine() const {
    return CMDisplayLine;
}
bool DiagramNode::ShowAHLine() const {
    return AHDisplayLine; 
}

void DiagramNode::SetLine(bool PCLine, bool LBLine, bool CMLine, bool AHLine) {
    PCDisplayLine = PCLine;
    LBDisplayLine = LBLine;
    CMDisplayLine = CMLine;
    AHDisplayLine = AHLine;
} 

void DiagramNode::ToggleLinePC() { 
    PCDisplayLine = !PCDisplayLine;
}

void DiagramNode::ToggleLineLB() {
    LBDisplayLine = !LBDisplayLine;
}

void DiagramNode::ToggleLineCM() {
    CMDisplayLine = !CMDisplayLine;
}

void DiagramNode::ToggleLineAH() {
    AHDisplayLine = !AHDisplayLine;
}

void SetLine(bool PCDisplayLine, bool LBDisplayLine, bool CMDisplayLine, bool AHDisplayLine);

void DiagramNode::DrawCircleLines(int centerX, int centerY, float radius, Color color) const {
    ::DrawCircleLines(centerX, centerY, radius, color);
}

bool DiagramNode::Button(Rectangle bounds, const char* text) const {
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {

        DrawRectangleRec(bounds, SKYBLUE);
        DrawText(text, bounds.x + 15, bounds.y + 15, 20, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    else {
        DrawRectangleRec(bounds, BLUE);
        DrawText(text, bounds.x + 15, bounds.y + 15, 20, WHITE);
    }
    return DisplayButtons;
}

void DiagramNode::SetB(bool SButton) {
    DisplayButtons = SButton;
}

void DiagramNode::ToggleB(){
    DisplayButtons = !DisplayButtons;
}