#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#include "background_img.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

enum GAME_STATE
{
	MENU,
	GAME,
	BREATH,
	IMAGE
};

GAME_STATE game_state = MENU;

const int WIDTH = 850;
const int HEIGHT = 450;

Image background_image;
Texture2D background_tex;

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Mental Health Game");

	background_image.data = BACKGROUND_IMG_DATA;
	background_image.format = BACKGROUND_IMG_FORMAT;
	background_image.width = BACKGROUND_IMG_WIDTH;
	background_image.height = BACKGROUND_IMG_HEIGHT;
	background_image.mipmaps = 1;

	background_tex = LoadTextureFromImage(background_image);
	
	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

struct SnowFlake
{
	Vector2 pos;
	float speed;
};

std::vector<SnowFlake> snowflakes;
int snowflakes_delay = 0;

std::vector<Vector2> bubbles;
int bubbles_delay = 0;

int breath_ball_radius = 10;
bool breath = true;

void UpdateDrawFrame()
{

	BeginDrawing();
	
	if (game_state == MENU)
	{
		ClearBackground(WHITE);
		DrawText("Pracitices for healing of mental health issues", 
			WIDTH/2-MeasureText("Pracitices for healing of mental health issues", 20)/2,
			30,
			20,
			BLACK
		);

		if (GuiButton({WIDTH/2+50, HEIGHT/2-50/2, 100, 50}, "Relaxing Imagery")) { game_state = IMAGE; };
		if (GuiButton({WIDTH/2-150, HEIGHT/2-50/2, 100, 50}, "Fun Little Game")) { game_state = GAME; };
	}
	else if (game_state == IMAGE)
	{
		ClearBackground(BLACK);
		DrawTexture(background_tex, 0, 0, WHITE);

		if (GuiButton({30, 30, 100, 40}, "Home")) { game_state = MENU; }

		snowflakes_delay += 1;

		snowflakes.push_back({Vector2{(float)GetRandomValue(0, WIDTH), 0}, (float)GetRandomValue(1, 3)});

		for (int i = 0; i < snowflakes.size(); i++)
		{
			DrawCircleV(snowflakes[i].pos, 2, WHITE);
			snowflakes[i].pos.y += snowflakes[i].speed;

			if (snowflakes[i].pos.y < 0) snowflakes.erase(snowflakes.begin() + i);
		}
	}
	else if (game_state == GAME)
	{
		ClearBackground(BLUE);

		if (GuiButton({30, 30, 100, 40}, "Home")) { game_state = MENU; }
		DrawText("Click the light blue bubbles to pop them.", 200, 20, 20, BLACK);

		bubbles_delay += 1;

		if (bubbles_delay >= GetFPS())
		{
			bubbles_delay = 0;
			bubbles.push_back(Vector2{(float)GetRandomValue(25, WIDTH-25), -25});
		}

		for (int i = 0; i < bubbles.size(); i++)
		{
			bubbles[i].y += 3;
			DrawCircleV(bubbles[i], 25, SKYBLUE);

			if (CheckCollisionPointCircle(GetMousePosition(), bubbles[i], 25) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				bubbles.erase(bubbles.begin() + i);
			}
		}
	}

	EndDrawing();
}