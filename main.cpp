// headers here ...
#include <iostream>
#include "raylib.h"
using namespace std;

int main() {

    string name;
    cout << "Welcome to our project! What is your name?" << endl;
    cin >> name;
    cout << "Hey, " << name << "... press enter to continue ..." << endl;

    /* raylib graphics + games library: open a window, size 1500x800 pixels */
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

    /* opens the graphics library window! */
    while (!WindowShouldClose()) {

        BeginDrawing();
        // Use a light background when showing the diagram, black for the cover photo
        ClearBackground(currentPhoto.id == diagramPhoto.id ? RAYWHITE : BLACK);

        // Reserve 140px at the top for text when showing the diagram
        float topMargin = (currentPhoto.id == diagramPhoto.id) ? 60.0f : 0.0f;
        float availableHeight = GetScreenHeight() - topMargin;

        float scaleX = (float)GetScreenWidth() / currentPhoto.width;
        float scaleY = availableHeight / currentPhoto.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY * 0.75f;

        float newWidth = currentPhoto.width * scale;
        float newHeight = currentPhoto.height * scale;

        float posX = (GetScreenWidth() - newWidth) / 2.0f;
        float posY = topMargin + (availableHeight - newHeight) / 2.0f;

        Rectangle source = { 0, 0, (float)currentPhoto.width, (float)currentPhoto.height };
        Rectangle dest = { posX, posY, newWidth, newHeight };

        DrawTexturePro(currentPhoto, source, dest, {0, 0}, 0.0f, WHITE);
        DrawText(message.c_str(), 40, 30, fontSize, textColor);


        if (IsKeyPressed(KEY_ENTER)) {
            currentPhoto = diagramPhoto;
            message = "Hey " + name + ", pick a compartment of the smartphone\nyou'd like to see explained more in depth";
            textColor = RED;
            fontSize = 40;
        }

        EndDrawing();
    }

    UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    CloseWindow();
    return 0;
}