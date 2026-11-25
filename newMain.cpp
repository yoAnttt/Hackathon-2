#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Tempfun.h"

using namespace std;

// Custom screen-space button with hover effect.
// Uses Raylib's collision detection (CheckCollisionPointRec) and mouse input (IsMouseButtonPressed).
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

// World-space button for elements rendered inside BeginMode2D(camera).
// Written with AI assistance (Google Gemini): uses Raylib's GetScreenToWorld2D() so button clicks
// still align accurately when the user zooms or pans the 2D camera.
bool WorldButton(Rectangle bounds, const char* text, Camera2D camera) {
    Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);

    if (CheckCollisionPointRec(worldMouse, bounds)) {
        DrawText(text, bounds.x + 5, bounds.y + 2, 2, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    else {
        DrawRectangleRec(bounds, BLUE);
        DrawText(text, bounds.x + 5, bounds.y + 2, 2, WHITE);
    }

    return false;
}

int main(void) {
    // Cursor setup (adapted from Raylib input/cursor examples)
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

    // Initialize Raylib window and lock frame rate to 60 FPS.
    InitWindow(screenWidth, screenHeight, "2D Camera Smartphone");
    SetTargetFPS(60);

    // Load image assets into GPU memory using Raylib's LoadTexture() function.
    Texture2D smartphonePhoto = LoadTexture("smartphone-img.png");
    Texture2D diagramPhoto = LoadTexture("diagram.png");

    // Load the 3D CPU model and create a render texture for it.
    Model cpu = LoadModel("cpu.glb");
    RenderTexture2D cpuRender = LoadRenderTexture(300, 250);

    // Set up the 3D camera.
    Camera3D cpuCamera = { 0 };
    cpuCamera.position = { 4.0f, 3.0f, 4.0f };
    cpuCamera.target = { 0.0f, 0.0f, 0.0f };
    cpuCamera.up = { 0.0f, 1.0f, 0.0f };
    cpuCamera.fovy = 45.0f;
    cpuCamera.projection = CAMERA_PERSPECTIVE;

    if (smartphonePhoto.id == 0 || diagramPhoto.id == 0) {
        CloseWindow();
        return -1;
    }

    // Track current image and header text styling.
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

    // Initialize Raylib's Camera2D struct (adapted from Raylib's "2D Camera Mouse Zoom" example)
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int zoomMode = 0;

    bool showButtons = false;

    // Controls which compartment information panel is displayed.
    // 0 = None, 1 = PC, 2 = LB, 3 = CM, 4 = AH.
    int activeInfo = 0;

    // Main Raylib render loop (runs once per frame until ESC or window close)
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
        // 2. CAMERA ZOOM & PAN LOGIC (Adapted from Raylib 2D Camera Example)
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

        // Click and drag with left mouse button to pan across the 2D camera view
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        // Render the CPU when the Logic Board panel is open
        // (Google searches plus AI help with rendering step)
        if (activeInfo == 2) {
            BeginTextureMode(cpuRender);
            ClearBackground(DARKGRAY);
            BeginMode3D(cpuCamera);

            // Rotate the CPU continuously around the Y-axis.
            DrawModelEx(cpu, { 0, 0, 0 }, { 0, 1, 0 }, GetTime() * 30.0f, { 1, 1, 1 }, WHITE);

            EndMode3D();
            EndTextureMode();
        }

        // ---------------------------------------------------------
        // 3. DRAWING (Raylib 2D Rendering)
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
                // Scale diagramPhoto proportionally while leaving top margin for header text
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

            // Used AI to help prevent long greeting text from overflowing off-screen.
            if (currentPhoto.id == smartphonePhoto.id) {
                while (MeasureText(message.c_str(), fontSize) > GetScreenWidth() - 80 && fontSize > 10) {
                    fontSize -= 2;
                }
            }

            // Center the top message horizontally using Raylib's MeasureText() function.
            int textWidth = MeasureText(message.c_str(), fontSize);
            int centeredX = (GetScreenWidth() - textWidth) / 2;

            // Dark bar behind the opening text so it is easy to read.
            if (currentPhoto.id == smartphonePhoto.id) {
                DrawRectangle(0, 20, GetScreenWidth(), 95, Fade(BLACK, 0.6f));
            }

            DrawText(message.c_str(), centeredX, 30, fontSize, textColor);
        }

        // Button opens interactive circles and disappears once clicked.
        if (showButtons && !InterfaceMain.ShowCircle()) {
            if (Button({ 10, 10, 175, 50 }, "LEARN MORE..")) {
                InterfaceMain.ToggleCircle();
            }
        }

        // Interactive node diagram rendered inside Raylib's 2D camera space
        if (InterfaceMain.ShowCircle()) {
            BeginMode2D(camera);

            DrawCircle(400, 300, 50, RED);
            DrawCircleLines(400, 300, 70.0f, BLUE);

            // Toggle visibility of the 4 outer compartment nodes
            if (WorldButton({ 375, 320, 45, 25 }, "LEARN \nMORE..", camera)) {
                InterfaceSubPC.ToggleCircle();
                InterfaceSubLB.ToggleCircle();
                InterfaceSubCM.ToggleCircle();
                InterfaceSubAH.ToggleCircle();
            }

            // Draw connecting lines, compartment nodes, and info buttons when expanded
            if (InterfaceSubPC.ShowCircle() && InterfaceSubLB.ShowCircle()
                && InterfaceSubCM.ShowCircle() && InterfaceSubAH.ShowCircle()) {

                // Connecting lines calculated with AI assistance using outer-point circle coordinates
                DrawLineV({ 350, 250 }, { 280, 200 }, RAYWHITE); // PC Line
                DrawLineV({ 450, 250 }, { 520, 200 }, RAYWHITE); // LB Line
                DrawLineV({ 350, 350 }, { 280, 400 }, RAYWHITE); // CM Line
                DrawLineV({ 450, 350 }, { 520, 400 }, RAYWHITE); // AH Line

                // Outer compartment circles
                DrawCircle(280, 200, 30, PURPLE); // PC Circle
                DrawCircle(520, 200, 30, BLUE);   // LB Circle
                DrawCircle(280, 400, 30, GREEN);  // CM Circle
                DrawCircle(520, 400, 30, ORANGE); // AH Circle

                // 1. Power Compartment Button (Near {280, 200})
                if (WorldButton({ 245, 240, 48, 15 }, "PC Info", camera)) {
                    activeInfo = 1;
                }

                // 2. Logic Board Button (Near {520, 200})
                if (WorldButton({ 485, 240, 48, 15 }, "LB Info", camera)) {
                    activeInfo = 2;
                }

                // 3. Control Module Button (Near {280, 400})
                if (WorldButton({ 245, 440, 48, 15 }, "CM Info", camera)) {
                    activeInfo = 3;
                }

                // 4. Audio & Haptics Button (Near {520, 400})
                if (WorldButton({ 485, 440, 48, 15 }, "AH Info", camera)) {
                    activeInfo = 4;
                }
            }

            // End 2D camera transformation so UI stays fixed to the screen
            EndMode2D();
        }

        // ---------------------------------------------------------
        // 4. COMPARTMENT EXPLANATION PANEL (AI-assisted with Raylib)
        // ---------------------------------------------------------
        if (activeInfo != 0) {
            // Draw the information panel
            DrawRectangle(450, 50, 1000, 700, DARKGRAY);

            // Display information based on the selected button
            if (activeInfo == 1) {
                DrawText("Power Compartment\nThe Heart of Your Smartphone", 500, 90, 35, GREEN);
                DrawText("Ever wondered what keeps your phone alive?\n\n"
                         "The lithium-ion battery stores chemical energy\n"
                         "and converts it into electrical energy.\n\n"
                         "The Power Management IC regulates voltage and\n"
                         "distributes power to different components.\n\n"
                         "Charging circuits monitor temperature and help\n"
                         "protect the battery from overheating.\n\n"
                         "Fun Fact: Your phone adjusts its power consumption\n"
                         "depending on the tasks you are performing.",
                         500, 180, 20, WHITE);
            }
            else if (activeInfo == 2) {
                DrawText("Logic Board\nThe Brain Behind the Screen", 500, 90, 35, GREEN);
                DrawText("Every tap triggers millions of tiny operations.\n\n"
                         "The CPU executes instructions, while the GPU\n"
                         "renders graphics, animations, and games.\n\n"
                         "RAM temporarily stores active applications,\n"
                         "while flash memory saves your personal data.\n\n"
                         "Billions of microscopic transistors work together\n"
                         "to process information at incredible speeds.\n\n"
                         "Fun Fact: Modern smartphone processors can perform\n"
                         "billions of operations every second.",
                         500, 180, 20, WHITE);

                // Larger 3D renderer
                DrawTexturePro(cpuRender.texture,
                               { 0, 0, 300, -250 },
                               { 1080, 250, 260, 200 },
                               { 0, 0 }, 0.0f, WHITE);
            }
            else if (activeInfo == 3) {
                DrawText("Camera Module\nFrom Photons to Memories", 500, 90, 35, GREEN);
                DrawText("Every tap triggers millions of tiny operations.\n\n"
                         "The CPU executes instructions, while the GPU\n"
                         "renders graphics, animations, and games.\n\n"
                         "RAM temporarily stores active applications,\n"
                         "while flash memory saves your personal data.\n\n"
                         "Billions of microscopic transistors work together\n"
                         "to process information at incredible speeds.\n\n"
                         "Fun Fact: Modern smartphone processors can perform\n"
                         "billions of operations every second.",
                         500, 180, 20, WHITE);
            }
            else if (activeInfo == 4) {
                DrawText("Audio & Haptics\nTechnology You Can Hear and Feel", 500, 90, 35, GREEN);
                DrawText("How does your phone turn light into a photograph?\n\n"
                         "Multiple tiny lenses focus incoming light onto\n"
                         "a CMOS image sensor containing millions of pixels.\n\n"
                         "The sensor converts light into electrical signals,\n"
                         "which an image processor turns into digital images.\n\n"
                         "Optical image stabilization uses tiny moving parts\n"
                         "to compensate for shaky hands.\n\n"
                         "Fun Fact: Your phone can combine multiple exposures\n"
                         "to produce a single, more detailed photograph.",
                         500, 180, 20, WHITE);
            }

            // Close the information panel
            if (Button({ 1250, 530, 120, 45 }, "CLOSE")) {
                activeInfo = 0;
            }
        }

        // ---------------------------------------------------------
        // 5. MULTI-STEP BACK NAVIGATION (AI-assisted)
        // ---------------------------------------------------------
        // Rendered outside BeginMode2D so it stays pinned in the top-left corner.
        if (currentPhoto.id == diagramPhoto.id || InterfaceMain.ShowCircle()) {
            if (Button({ 10, 70, 120, 45 }, "BACK") || IsKeyPressed(KEY_BACKSPACE)) {

                // Step A: Close the information panel before collapsing the circles
                if (activeInfo != 0) {
                    activeInfo = 0;
                }
                // Step B: Sub-circles are open -> collapse back to the main red circle
                else if (InterfaceSubPC.ShowCircle()) {
                    InterfaceSubPC.SetCircle(false);
                    InterfaceSubLB.SetCircle(false);
                    InterfaceSubCM.SetCircle(false);
                    InterfaceSubAH.SetCircle(false);
                }
                // Step C: Only main red circle is open -> go back to diagramPhoto
                else if (InterfaceMain.ShowCircle()) {
                    InterfaceMain.SetCircle(false);
                    camera.zoom = 1.0f;
                    camera.target = { 0.0f, 0.0f };
                    camera.offset = { 0.0f, 0.0f };
                }
                // Step D: On diagramPhoto -> go back to opening smartphonePhoto
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

    // Close Raylib window cleanly and release 2D/3D resources
    UnloadTexture(smartphonePhoto);
    UnloadTexture(diagramPhoto);
    UnloadModel(cpu);
    UnloadRenderTexture(cpuRender);
    CloseWindow();

    return 0;
}