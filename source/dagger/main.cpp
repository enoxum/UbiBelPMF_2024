
#include "core/engine.h"
#include "gameplay/pacman/pacman_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	return engine.Run<pacman::PacmanGame>();
}
