#include <raylib.h>
#include <imgui.h>
#include "library/rlImGui.h"
#include "config.h"


int main() {
    InitWindow(Game::ScreenWidth, Game::ScreenHeight, Game::PROJECT_NAME);
    SetTargetFPS(60);


    Texture2D myTexture = LoadTexture("assets/graphics/testimage.png");

    rlImGuiSetup(true);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(WHITE);

        rlImGuiBegin();

        ImGui::Begin("Pannello di Controllo");
        ImGui::Text("Se vedi questo, rlImGui funziona!");
        static float colore[3] = { 1.0f, 0.0f, 0.0f };
        ImGui::ColorEdit3("Colore Sfondo", colore);

        if (ImGui::Button("Reset Finestra")) {

        }
        ImGui::End();

        rlImGuiEnd();

        DrawText("Hello World", 20, 20, 20, RED);
        DrawTexture(myTexture, 10, 100, WHITE);

        EndDrawing();
    }

    UnloadTexture(myTexture);

    CloseWindow();

    return 1;
}
