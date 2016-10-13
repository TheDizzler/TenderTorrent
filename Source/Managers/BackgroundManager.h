#pragma once

#include "../GameObjects/PlayerShip.h"
#include "../GameObjects/Graphics/Background.h"


using namespace std;

/** This manager seems unnecessary. */
class BackgroundManager {
public:
	BackgroundManager();
	~BackgroundManager();

	//bool initialize(ID3D11Device* device);
	bool loadLevel(ComPtr<ID3D11Device> device, const wchar_t* file);
	void clear();

	//void startUpdate(double deltaTime);
	void update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);


	vector<BackgroundLayer*> getLayers();

private:

	unique_ptr<Background> bg;
};