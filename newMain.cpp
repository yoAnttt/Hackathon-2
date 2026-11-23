#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Tempfun.h"

using namespace std;

bool Button(Rectangle bounds, const char* text) {
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

    return false;
}
// Custom button function with hover effect
int main(void) {
    // Cursor setup
    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    // Terminal greeting
    string name;
    cout << "Welcome to our project! We will be explaining how different compartments of a smartphone work." << endl;
    cout << "You usually don't think about this on a daily basis, but now you will have the opportunity to discover " << endl;
    cout << "how your cellphone truly works. To start off, what is your name?" << endl;
    cin >> name;

    const int screenWidth = 1500;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "2D Camera Smartphone");
    SetTargetFPS(60);

    // Load textures
    Texture2D smartphonePhoto = LoadTexture("smartphone-img.png");
    Texture2D diagramPhoto = LoadTexture("diagram.png");
    if (smartphonePhoto.id == 0 || diagramPhoto.id == 0) {
        CloseWindow();
        return -1;
    }


    // Track current image and header text
    Texture2D currentPhoto = smartphonePhoto;
    string message = "Hey " + name + " press ENTER to continue!";
    Color textColor = BLUE;
    int fontSize = 50;

    DiagramNode InterfaceMain;  
    InterfaceMain.SetCircle(false, false, false, false, false);
    InterfaceMain.SetLine(false, false, false, false);
    // Initialize 2D camera
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int zoomMode = 0;

     bool showButtons = false;

    BeginDrawing();
    ClearBackground(BLACK);
 
    while (!WindowShouldClose()) {
// ---------------------------------------------------------
        // 1. INPUT & CURSOR UPDATES
        // ---------------------------------------------------------
        if (IsKeyPressed(KEY_H)) {
            if (IsCursorHidden()) ShowCursor();
            else HideCursor();
        }

        ballPosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE;

        // Transition from opening photo to diagram when Enter is pressed
        if (IsKeyPressed(KEY_ENTER)) {
            currentPhoto = diagramPhoto;
            message = "Hey " + name + ", pick a compartment of the smartphone\nyou'd like to see explained more in depth";
            textColor = RED;
            fontSize = 40;
            showButtons = true;
        }

        // ---------------------------------------------------------
        // 2. CAMERA ZOOM & PAN LOGIC
        // ---------------------------------------------------------
        Vector2 renderScale = { (float)GetRenderWidth() / GetScreenWidth(), (float)GetRenderHeight() / GetScreenHeight() };

        if (IsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (IsKeyPressed(KEY_TWO)) zoomMode = 1;

        if (zoomMode == 0) {
            float wheel = GetMouseWheelMove();
            if (wheel != 0) {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = Vector2Multiply(GetMousePosition(), renderScale);
                camera.target = mouseWorldPos;

                float scale = 0.2f * wheel;
                camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
            }
        }
        else {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = Vector2Multiply(GetMousePosition(), renderScale);
                camera.target = mouseWorldPos;
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                float deltaX = GetMouseDelta().x;
                float scale = 0.005f * deltaX;
                camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }
        // ---------------------------------------------------------
        // 3. DRAWING
        // ---------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw image & prompt text (hidden once the interactive circles open)
        if (!InterfaceMain.ShowMainCircle()) {
            Rectangle source = { 0, 0, (float)currentPhoto.width, (float)currentPhoto.height };
            Rectangle dest;

            if (currentPhoto.id == smartphonePhoto.id) {
                // Stretch initial background photo across the entire screen
                dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            }
            else {
                // Keep proportional scaling for diagramPhoto
                float topMargin = 60.0f;
                float availableHeight = GetScreenHeight() - topMargin;
                float scaleX = (float)GetScreenWidth() / currentPhoto.width;
                float scaleY = availableHeight / currentPhoto.height;

                float scale;
                if (scaleX < scaleY) {
                    scale = scaleX;
                }
                else {
                    scale = scaleY * 0.75f;
                }

                float newWidth = currentPhoto.width * scale;
                float newHeight = currentPhoto.height * scale;
                float posX = (GetScreenWidth() - newWidth) / 2.0f;
                float posY = topMargin + (availableHeight - newHeight) / 2.0f;

                dest = { posX, posY, newWidth, newHeight };
            }

            DrawTexturePro(currentPhoto, source, dest, { 0, 0 }, 0.0f, WHITE);

            //Shrink initial font size only if it is wider than the screen
            // Used Gemini AI to help fix this issue of the text constantly falling off the screen
            if (currentPhoto.id == smartphonePhoto.id) {
                while (MeasureText(message.c_str(), fontSize) > GetScreenWidth() - 80 && fontSize > 10) {
                    fontSize -= 2;
                }
            }

            // Center the top message horizontally
            int textWidth = MeasureText(message.c_str(), fontSize);
            int centeredX = (GetScreenWidth() - textWidth) / 2;

            // Dark bar behind the opening text so it is easy to read
            if (currentPhoto.id == smartphonePhoto.id) {
                DrawRectangle(0, 20, GetScreenWidth(), 95, Fade(BLACK, 0.6f));
            }

            DrawText(message.c_str(), centeredX, 30, 40, textColor);
        }
        // Button opens interactive circles and disappears once clicked
        if (showButtons && !InterfaceMain.ShowMainCircle()) {
            if (Button({ 10, 10, 175, 50 }, "LEARN MORE..")) {
                InterfaceMain.ToggleMainCircle();
            }
        }
        
        if(InterfaceMain.ShowMainCircle() ) {
            BeginMode2D(camera);
            InterfaceMain.DrawCircle(400, 300, 50, RED);
            InterfaceMain.DrawCircleLines(400, 300, 70.0f, BLUE);
            InterfaceMain.ShowSubCircleAH();
            InterfaceMain.ShowSubCircleCM(); 
            InterfaceMain.ShowSubCircleLB(); 
            InterfaceMain.ShowSubCirclePC();

            if(Button({ 375, 320, 45, 25 }, "LEARN MORE..")) {
                InterfaceMain.ToggleLinePC();
                InterfaceMain.ToggleLineLB();
                InterfaceMain.ToggleLineCM();
                InterfaceMain.ToggleLineAH();
                InterfaceMain.ToggleSubCirclePC(); 
                InterfaceMain.ToggleSubCircleLB();
                InterfaceMain.ToggleSubCircleCM();
                InterfaceMain.ToggleSubCircleAH();
            }
            if (InterfaceMain.ShowAHLine() && InterfaceMain.ShowCMLine() 
                && InterfaceMain.ShowLBLine() && InterfaceMain.ShowPCLine()
                && InterfaceMain.ShowSubCircleAH() && InterfaceMain.ShowSubCircleCM() && 
                InterfaceMain.ShowSubCircleLB() && InterfaceMain.ShowSubCirclePC()) { 
                // Gemini Used to find the Outside point circle formula 
                InterfaceMain.DrawLineV({ 350, 250 }, { 280, 200 }, RAYWHITE); // PC Line 
                InterfaceMain.DrawLineV({ 450, 250 }, { 520, 200 }, RAYWHITE); // LB Line
                InterfaceMain.DrawLineV({ 350, 350 }, { 280, 400 }, RAYWHITE); // CM Line
                InterfaceMain.DrawLineV({ 450, 350 }, { 520, 400 }, RAYWHITE); // AH Line

                InterfaceMain.DrawCircle(280, 200, 30, PURPLE); // PC Circle
                InterfaceMain.DrawCircle(520, 200, 30, BLUE);   // LB Circle
                InterfaceMain.DrawCircle(280, 400, 30, VIOLET); // CM Circle 
                InterfaceMain.DrawCircle(520, 400, 30, PURPLE); // AH Circle

                // 1. Power Compartment Button (Near {280, 200})
                if (Button({ 245, 240, 70, 25 }, "PC Info")) {
                    
                }

                // 2. Logic Board Button (Near {520, 200})
                if (Button({ 485, 240, 70, 25 }, "LB Info")) {

                }

                // 3. Control Module Button (Near {280, 400})
                if (Button({ 245, 440, 70, 25 }, "CM Info")) {

                }

                // 4. Audio & Haptics Button (Near {520, 400})
                if (Button({ 485, 440, 70, 25 }, "AH Info")) {

                }
            }
        }
        EndMode2D();
        // Draw custom cursor on top of everything
        DrawCircleV(ballPosition, 15, ballColor);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}