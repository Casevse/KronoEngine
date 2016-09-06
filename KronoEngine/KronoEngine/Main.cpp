#include <Config.h>
#include <Renderer.h>

int main(void) {
	Renderer renderer;

	renderer.Init(Config::width, Config::height, Config::fullscreen, "KronoEngine");

	while (renderer.IsRunning()) {
		renderer.BeginFrame();
		renderer.Draw();
	}

	renderer.Close();

	return 0;
}