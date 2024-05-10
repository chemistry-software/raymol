#include "raylib.h"
#include "sdfparse.c"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
void drawAtom(Atom atom);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Molecule ethanol = parseSDF("./resources/ethanol.sdf");
    normalizeCoordinates(&ethanol, screenWidth, screenHeight);

    printf("Number of atoms: %d\n", ethanol.num_atoms);
    for (int i = 0; i < ethanol.num_atoms; i++) {
        printf("Atom %d: %s (%.4f, %.4f, %.4f)\n", i+1, ethanol.atoms[i].atom_type, ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z);
    }

    InitWindow(screenWidth, screenHeight, "LOL dongs");

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // update stuff ++

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(0, 0, screenWidth, screenHeight, ORANGE);
            DrawText("MOLTEST ayyyooo", 20, 20, 40, DARKGREEN);
            for (int i = 0; i < ethanol.num_atoms; i++) {
                DrawText(TextFormat("Atom %d: %s (%.4f, %.4f, %.4f)", i+1, ethanol.atoms[i].atom_type, ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z), 20, 80 + i * 20, 20, DARKGRAY);
            }

            for (int i = 0; i < ethanol.num_atoms; i++) {
                drawAtom(ethanol.atoms[i]);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawAtom(Atom atom)
{
   DrawSphere((Vector3){atom.x, atom.y, atom.z}, 0.2, RED);
} 
