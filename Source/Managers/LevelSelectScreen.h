#pragma once

#include <pugixml.hpp>
#include "../DXTKGui/Controls/GUIFactory.h"
//#include "../DXTKGui/BaseGraphics/screen.h"
//#include "../DXTKGui/BaseGraphics/PrimitiveShapes.h"
//#include "../DXTKGui/Controls/TextLabel.h"


struct LevelSelection {

	Vector2 position;
	
	unique_ptr<Sprite> previewPic;
	unique_ptr<RectangleFrame> picFrame;
	unique_ptr<TextLabel> label;
	string levelXMLFile;

	unique_ptr<HitArea> hitArea;


	LevelSelection(const Vector2& position, pugi::xml_node levelNode);

	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);

	void setPosition(const Vector2& newPosition);
};

class LevelSelectScreen : public Screen {
public:
	LevelSelectScreen();
	~LevelSelectScreen();

	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController * mouse) override;
	virtual void setGameManager(GameManager * game) override;

	virtual void update(double deltaTime, KeyboardController * keys, MouseController * mouse) override;
	virtual void draw(SpriteBatch * batch) override;

	virtual void pause() override;


private:
	GameManager* game;
	unique_ptr<xml_document> levelManifest;

	vector<unique_ptr<LevelSelection>> levelSelections;
};