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

    // Draw button
    DrawRectangleRec(bounds, BLUE);

    // Display text
    DrawText(text, bounds.x + 10, bounds.y + 10, 20, WHITE);

    // Check if the mouse is inside the button
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {

        // Check if the button is clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE;

        // raylib drawing setup and background color constraints (RAYWHITE, BLACK)
        BeginDrawing();

        ClearBackground(currentPhoto.id == diagramPhoto.id ? RAYWHITE : BLACK);

        // Aspect-ratio scaling and centering math generated with AI assistance (Google Gemini)
        // Used raylib's GetScreenHeight() and GetScreenWidth() to fit the image proportionally
        float topMargin = (currentPhoto.id == diagramPhoto.id) ? 60.0f : 0.0f;
        float availableHeight = GetScreenHeight() - topMargin;

        float scaleX = (float)GetScreenWidth() / currentPhoto.width;
        float scaleY = availableHeight / currentPhoto.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY * 0.75f;
 
        float newWidth = currentPhoto.width * scale;
        float newHeight = currentPhoto.height * scale;

        float posX = (GetScreenWidth() - newWidth) / 2.0f;
        float posY = topMargin + (availableHeight - newHeight) / 2.0f;

        // raylib rectangle struct defining the source texture bounds
        Rectangle source = { 0, 0, (float)currentPhoto.width, (float)currentPhoto.height };
        Rectangle dest = { posX, posY, newWidth, newHeight };

        // Render scaled texture and prompt text using raylib
        DrawTexturePro(currentPhoto, source, dest, {0, 0}, 0.0f, WHITE);
        DrawText(message.c_str(), 40, 30, fontSize, textColor);

        // Detect enter key press via raylib input functions (IsKeyPressed, KEY_ENTER)
        if (IsKeyPressed(KEY_ENTER)) {
            currentPhoto = diagramPhoto;
            message = "Hey " + name + ", pick a compartment of the smartphone\nyou'd like to see explained more in depth";
            textColor = RED; // raylib built-in RED color
            fontSize = 40;

            showButtons = true; // Buttons only show up after user continues
        }

        // Buttons
        if (showButtons == true){
            if (Button({25, 120, 235, 50}, "Logic Board")) {
                cout << "Logic Board opened!" << endl;
            }

            if (Button({25, 190, 235, 50}, "Power Compartment")) {
                cout << "Power Compartment opened!" << endl;
            }

            if (Button({25, 260, 235, 50}, "Camera Modules")) {
                cout << "Camera Modules opened!" << endl;
            }

            if (Button({25, 330, 235, 50}, "Audio and Haptics")) {
                cout << "Audio and Haptics opened!" << endl;
            }
        }

        // End frame rendering via raylib
        EndDrawing();
    }

    // Free loaded GPU textures and close window using raylib cleanup functions
    UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    CloseWindow();
    return 0;
}