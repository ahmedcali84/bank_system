#include <raylib.h>

int main(void)
{
	int Window_Height = 800;
	int Window_Width = 960;

	InitWindow(Window_Width , Window_Height , "Banking System Interface");
	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		ClearBackground(RAYWHITE);
		BeginDrawing();
		
		EndDrawing();
	}

	CloseWindow();

	return 0;
}