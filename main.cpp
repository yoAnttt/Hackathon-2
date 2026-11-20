/*
Project: Hackathon #2
External Sources Cited:
* Raylib - used for window management, texture loading, 2D rendering,and keyboard input
* diagram.jpg image made using Google Gemini
* AI assistance - Google Gemini, used for proportional image-scaling and screen-centering calculations
*/
#include <iostream>
#include "raylib.h"
using namespace std;
#define NUM_FRAMES  3

// Custom button function using Raylib
bool Button(Rectangle bounds, const char* text) {

   // Check if the mouse is inside the button
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        
        // Draw lighter blue when hovering
        DrawRectangleRec(bounds, SKYBLUE);
        DrawText(text, bounds.x + 15, bounds.y + 15, 20, WHITE);

        // Check if the button is clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    else {
        // Draw normal blue when not hovering
        DrawRectangleRec(bounds, BLUE);
        DrawText(text, bounds.x + 15, bounds.y + 15, 20, WHITE);
    }
    return false;
}

int main() {
    // Cursor
    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    // Used your example as an outline for the basics of this program
    string name;
    cout << "Welcome to our project! What is your name?" << endl;
    cin >> name;
    cout << "Hey, " << name << "... press enter to continue ..." << endl;

    InitWindow(1500, 800, "Hackathon #2");
    // FPS = frame rate, 30 per second, caps the window rendering
    SetTargetFPS(30);

    // load a TEXTURE, an image that will fill a shape
    Texture2D smartphonePhoto = LoadTexture("smartphone-img.jpg");
    Texture2D diagramPhoto = LoadTexture("diagram.jpg");
    if (smartphonePhoto.id == 0 || diagramPhoto.id == 0) {
        CloseWindow();
        return -1;
    }

    // track which texture is currently displayed
    Texture2D currentPhoto = smartphonePhoto;

    // preparing text to add on top of image
    string message = "PRESS ENTER TO CONTINUE";
    Color textColor = YELLOW;
    int fontSize = 75;
    int textX = 80;
    int textY = 30;

    bool showButtons = false;

    // Main render loop controlled by raylib  
    while (!WindowShouldClose()) {

        // Cursor
        if (IsKeyPressed(KEY_H))
        {
            if (IsCursorHidden()) ShowCursor();
            else HideCursor();
        }

        ballPosition = GetMousePosition();

        // raylib drawing setup and background color constraints (RAYWHITE, BLACK)
        BeginDrawing();

        // Set background color and top margin depending on which image is showing
        Color bgColor;
        float topMargin;

        if (currentPhoto.id == diagramPhoto.id) {
            bgColor = RAYWHITE;
            topMargin = 60.0f;
        }
        else {
            bgColor = BLACK;
            topMargin = 0.0f;
        }

        ClearBackground(bgColor);

        // Aspect-ratio scaling and centering math
        // Used raylib's GetScreenHeight() and GetScreenWidth() to fit the image proportionally
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

        if (currentPhoto.id == diagramPhoto.id) {
            posX = posX - 100.0f;
            posY = posY - 30.0f;
        }

        // raylib rectangle struct defining the source texture bounds
        Rectangle source = { 0, 0, (float)currentPhoto.width, (float)currentPhoto.height };
        Rectangle dest = { posX, posY, newWidth, newHeight };

        // Render scaled texture and prompt text using raylib
        DrawTexturePro(currentPhoto, source, dest, {0, 0}, 0.0f, WHITE);
        DrawText(message.c_str(), textX, textY, fontSize, textColor);

        // Detect enter key press via raylib input functions (IsKeyPressed, KEY_ENTER)
        if (IsKeyPressed(KEY_ENTER)) {
            currentPhoto = diagramPhoto;
            message = "Hey " + name + ", pick a compartment of the smartphone\nyou'd like to see explained more in depth";
            textColor = RED; // raylib built-in RED color
            fontSize = 40;
            textX = 175;
            textY = 20;

            showButtons = true; // Buttons only show up after user continues
        }

        // Buttons
        if (showButtons == true){
            if (Button({25, 160, 235, 50}, "Logic Board")) {
                cout << "Logic Board opened!" << endl;
            }

            if (Button({25, 230, 235, 50}, "Power Compartment")) {
                cout << "Power Compartment opened!" << endl;
            }

            if (Button({25, 300, 235, 50}, "Camera Modules")) {
                cout << "Camera Modules opened!" << endl;
            }

            if (Button({25, 370, 235, 50}, "Audio and Haptics")) {
                cout << "Audio and Haptics opened!" << endl;
            }
        }

        // Draw the custom cursor ball on top of everything
        DrawCircleV(ballPosition, 15.0f, ballColor);

        // End frame rendering via raylib
        EndDrawing();
    }

    // Free loaded GPU textures and close window using raylib cleanup functions
    UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    CloseWindow();
    return 0;
}