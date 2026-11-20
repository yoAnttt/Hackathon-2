#include <iostream>
#include "raylib.h"
#include "Tempfun.h"
#include "raymath.h"

using namespace std;

// for buttons 
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



//Used raylib cheatsheet line 8-14
float GetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger 
Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera);   // Get world space position for a 2d camera screen space position
Vector2 GetMousePosition(void);                         // Get mouse position XY  
bool IsKeyPressed(int key);                             // Check if key has been pressed once   
int GetRenderWidth(void);                                   // Get current render width (it considers HiDPI)
int GetRenderHeight(void);                                  // Get current render height (it considers HiDPI)
Vector2 GetMouseDelta(void);                            // Get mouse delta between frames
float Clamp(float value, float min, float max);   
float logf(float x);
float expf(float x);




int main(void) {

    // cursor
    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    string name;
    cout << "Welcome to our project! What is your name?" << endl;
    cin >> name;
    cout << "Hey, " << name << "... press enter to continue ..." << endl;



    const int screenWidth = 1920; // Used raylib example core 2d camera mouse zoom line 20 - 95
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "2d camera Smartphone");


    // load a TEXTURE, an image that will fill a shape
    Texture2D smartphonePhoto = LoadTexture("smartphone-img.png");
    Texture2D diagramPhoto = LoadTexture("diagram.png");
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





    // Create an instance of MainInterface
    MainInterface InterfaceMain;
    SubInterfaceCompPC InterfaceSubPC;
    SubInterfaceCompLB InterfaceSubLB;
    SubInterfaceCompCM InterfaceSubCM; 
    SubInterfaceCompAH InterfaceSubAH;
    // Initialize camera
    Camera2D camera = { 0 }; 
    camera.zoom = 1.0f;      

    int zoomMode = 0; 

    SetTargetFPS(60);  
    
    
    // BOLEANS TO MAKE THINGS APPEAR ONLY AFTER SOMETHING ELSE
    bool showCircles = false;
    bool showButtons = false;


    // Main game loop
    while (!WindowShouldClose()) {


        // CURSOR in while 
        //----------------------------------------------------------------------------------
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
        //----------------------------------------------------------------------------------


        // Calculate the render scale
        Vector2 renderScale = { (float)GetRenderWidth()/GetScreenWidth(), (float)GetRenderHeight()/GetScreenHeight() };

        if (zoomMode == 0)
        {
            float wheel = GetMouseWheelMove(); // Mouse wheel movement for X or Y, whichever is larger
            if (wheel != 0) 
            {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera); // Mouse position in world space before zooming
                camera.offset = Vector2Multiply(GetMousePosition(), renderScale); // Set the offset to where the mouse is, mouse is scaled to viewport space, but offset is in unscaled pixels

                // Set the target to match, so that the camera maps the world space point
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;

                // Zoom increment
                // Uses log scaling to provide consistent zoom speed
                float scale = 0.2f*wheel;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            }
        }
        else
        {
            // Zoom based on mouse right click
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is, mouse is scaled to viewport space, but offset is in unscaled pixels
                camera.offset = Vector2Multiply(GetMousePosition(), renderScale);

                // Set the target to match, so that the camera maps the world space point
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                // Zoom increment
                // Uses log scaling to provide consistent zoom speed
                float deltaX = GetMouseDelta().x;
                float scale = 0.005f*deltaX;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            }
        }


// Zoom mode handling by key press
        if (IsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (IsKeyPressed(KEY_TWO)) zoomMode = 1;

        // Camera movement handling with mouse drag
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }


        BeginDrawing();

        ClearBackground(RAYWHITE);

    ////////////////// buttons   ////////////////////////////////////////////////


// button opens cirles and closes once it circles
if (showButtons && !showCircles){


if (Button({10, 10, 150, 50}, "LEARN MORE..")) {
    showCircles = true;
}




////////////////////////////////////////////////////////////////////////////////////////////////



}



// IMAGES

 // ClearBackground(currentPhoto.id == diagramPhoto.id ? RAYWHITE : BLACK);


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


        // EDIT TO MAKE THE IMAGE DISSAPPEAR AFTER CLICKING BUTTON LEARN MORE
        if (!showCircles) {
    DrawTexturePro(currentPhoto, source, dest, {0, 0}, 0.0f, WHITE);
}



// to make compartment message 2nd window dissapear
if (!showCircles){
        DrawText(message.c_str(), 40, 30, fontSize, textColor);
}





        if (IsKeyPressed(KEY_ENTER)) {

            currentPhoto = diagramPhoto;


            
                
            message = "Hey " + name + ", pick a compartment of the smartphone\nyou'd like to see explained more in depth";
            textColor = RED;
            fontSize = 40;
        

showButtons = true; // for buttons only to show up after continue

        }















// SHOW CIRCLES AFTER LEARN MORE 
if (showCircles == true) { 

    BeginMode2D(camera);

        // Cricle for Power Compartment
        InterfaceSubPC.DrawCircleLines(280, 200, 40.0f, ORANGE);
        InterfaceSubPC.DrawLineV({400, 300}, {280, 200}, BLACK); // Draw a Line from center to sub-branch 
        InterfaceSubPC.DrawCircle(280, 200, 30, PURPLE);
        // Cricle for Logic Board
        InterfaceSubLB.DrawCircleLines(520, 200, 40.0f, LIGHTGRAY);
        InterfaceSubLB.DrawLineV({400, 300}, {520, 200}, BLACK);    
        InterfaceSubLB.DrawCircle(520, 200, 30, PINK);
        // Cricle for Control Module
        InterfaceSubCM.DrawCircleLines(280, 400, 40.0f, MAROON);
        InterfaceSubCM.DrawLineV({400, 300}, {280, 400}, BLACK);
        InterfaceSubCM.DrawCircle(280, 400, 30, LIME);
        // Cricle for Audio & Haptics
        InterfaceSubAH.DrawCircleLines(520, 400, 40.0f, DARKGREEN);
        InterfaceSubAH.DrawLineV({400, 300}, {520, 400}, BLACK);    
        InterfaceSubAH.DrawCircle(520, 400, 30, BLUE);
        // MaIn middle Circle 
        InterfaceMain.DrawCircle(400, 300, 50, RED); // Draw a filled circle
        InterfaceMain.DrawCircleLines(400, 300, 70.0f, BLUE); // Draw a circle outline 
        
        EndMode2D();


    } 


    // FOR CURSOR TO SHOW - DRAWING
    DrawCircleV(ballPosition, 15, ballColor);

    EndDrawing();
      




    }

    // Close the window and clean up resources
     UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    CloseWindow();
    return 0;
}