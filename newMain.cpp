#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Tempfun.h"

using namespace std;

// Custom button function with hover effect
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

// Same button logic, adjusted for 2D camera zoom/pan
bool WorldButton(Rectangle bounds, const char* text, Camera2D camera) {
    Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);

    if (CheckCollisionPointRec(worldMouse, bounds)) {
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

    // DiagramNode class instances from Tempfun.h
    DiagramNode InterfaceMain;
    DiagramNode InterfaceSubPC;
    DiagramNode InterfaceSubLB;
    DiagramNode InterfaceSubCM;
    DiagramNode InterfaceSubAH;

    // Initialize 2D camera
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int zoomMode = 0;

    bool showButtons = false;

    while (!WindowShouldClose()) {
        // ---------------------------------------------------------
        // 1. INPUT & CURSOR UPDATES
        // ---------------------------------------------------------
        if (IsKeyPressed(KEY_H)) {
            if (IsCursorHidden()) ShowCursor();
            else HideCursor();
        }

        ballPosition = GetMousePosition();

        // Transition from opening photo to diagram when Enter is pressed
        if (IsKeyPressed(KEY_ENTER) && currentPhoto.id == smartphonePhoto.id) {
            currentPhoto = diagramPhoto;
            message = "Hey " + name + ", click 'Learn More' to see\nthe compartments explained in depth.";
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
        if (!InterfaceMain.ShowCircle()) {
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

            // Shrink initial font size only if it is wider than the screen
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

            DrawText(message.c_str(), centeredX, 30, fontSize, textColor);
        }

        // Button opens interactive circles and disappears once clicked
        if (showButtons && !InterfaceMain.ShowCircle()) {
            if (Button({ 10, 10, 175, 50 }, "LEARN MORE..")) {
                InterfaceMain.ToggleCircle();
            }
        }

        if (InterfaceMain.ShowCircle()) {
            BeginMode2D(camera);

            DrawCircle(400, 300, 50, RED);
            DrawCircleLines(400, 300, 70.0f, BLUE);

            if (WorldButton({ 375, 320, 45, 25 }, "LEARN MORE..", camera)) {
                InterfaceSubPC.ToggleCircle();
                InterfaceSubLB.ToggleCircle();
                InterfaceSubCM.ToggleCircle();
                InterfaceSubAH.ToggleCircle();
            }

            if (InterfaceSubPC.ShowCircle() && InterfaceSubLB.ShowCircle()
                && InterfaceSubCM.ShowCircle() && InterfaceSubAH.ShowCircle()) {

                DrawLineV({ 350, 250 }, { 280, 200 }, RAYWHITE); // LB Line
                DrawLineV({ 450, 250 }, { 520, 200 }, RAYWHITE); // CM Line
                DrawLineV({ 350, 350 }, { 280, 400 }, RAYWHITE); // PC Line
                DrawLineV({ 450, 350 }, { 520, 400 }, RAYWHITE); // AH Line

                DrawCircle(280, 200, 30, PURPLE); // LB Circle
                DrawCircle(520, 200, 30, BLUE);   // CM Circle
                DrawCircle(280, 400, 30, VIOLET); // PC Circle
                DrawCircle(520, 400, 30, PURPLE); // AH Circle

                // 1. Power Compartment Button (Near {280, 200})
                if (WorldButton({ 245, 240, 70, 25 }, "LB Info", camera)) {
                }

                // 2. Logic Board Button (Near {520, 200})
                if (WorldButton({ 485, 240, 70, 25 }, "CM Info", camera)) {
                }

                // 3. Control Module Button (Near {280, 400})
                if (WorldButton({ 245, 440, 70, 25 }, "PC Info", camera)) {
                }

                // 4. Audio & Haptics Button (Near {520, 400})
                if (WorldButton({ 485, 440, 70, 25 }, "AH Info", camera)) {
                }
            }

            EndMode2D();
        }

        // Show BACK button whenever past the opening smartphone photo
        if (currentPhoto.id == diagramPhoto.id || InterfaceMain.ShowCircle()) {
            if (Button({ 10, 70, 120, 45 }, "BACK") || IsKeyPressed(KEY_BACKSPACE)) {

                // Step A: Sub-circles are open -> collapse back to the main red circle
                if (InterfaceSubPC.ShowCircle()) {
                    InterfaceSubPC.SetCircle(false);
                    InterfaceSubLB.SetCircle(false);
                    InterfaceSubCM.SetCircle(false);
                    InterfaceSubAH.SetCircle(false);
                }
                // Step B: Only main red circle is open -> go back to diagramPhoto
                else if (InterfaceMain.ShowCircle()) {
                    InterfaceMain.SetCircle(false);
                    camera.zoom = 1.0f;
                    camera.target = { 0.0f, 0.0f };
                    camera.offset = { 0.0f, 0.0f };
                }
                // Step C: On diagramPhoto -> go back to opening smartphonePhoto
                else if (currentPhoto.id == diagramPhoto.id) {
                    currentPhoto = smartphonePhoto;
                    message = "Hey " + name + " press ENTER to continue!";
                    textColor = BLUE;
                    fontSize = 50;
                    showButtons = false;
                }
            }
        }

        // Draw custom cursor on top of everything
        DrawCircleV(ballPosition, 15, ballColor);
        EndDrawing();
    }

    UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    CloseWindow();
    return 0;
}