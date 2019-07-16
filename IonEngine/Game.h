#pragma once
class Game {
public:
	Game();
	~Game();

	virtual void initialize() = 0;
	virtual void preUpdate() = 0;
	virtual void postUpdate() = 0;
	virtual void cleanUp() = 0;
};

