#include <iostream>
#include <Engine.h>

#include "TestScene.h"
#include "TestGame.h"
using namespace IonEngine;


int main(int argc, char** args) {

	Engine::initialize(new TestGame());
	Engine::scene = new TestScene();
	Engine::scene->load();

	Engine::loop();

	Engine::destroy();

	return 0;
}
