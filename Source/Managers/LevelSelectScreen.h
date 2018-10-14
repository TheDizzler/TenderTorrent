#pragma once


class LevelSelection : public Selectable {
public:

	string levelXMLFile;

	LevelSelection(GUIFactory* factory, MouseController* mouseController,
		const Vector2& position, pugi::xml_node levelNode);
	virtual ~LevelSelection();

	virtual void reloadGraphicsAsset() override;

	virtual bool updateSelect(double deltaTime) override;
	virtual bool update(double deltaTime) override;
	void draw(SpriteBatch* batch);

	virtual void setFont(const pugi::char_t* font = "Default Font") override;
	virtual void setText(wstring text) override;
	virtual const Vector2 XM_CALLCONV measureString() const override;
	virtual const Vector2& getPosition() const override;
	virtual const int getWidth() const override;
	virtual const int getHeight() const override;
	const Vector2& getSize();


	virtual bool clicked() override;
	virtual bool pressed() override;
	virtual bool hovering() override;

	class OnClickListener {
	public:
		virtual void onClick(LevelSelection* level) = 0;
		virtual void onPress(LevelSelection* level) = 0;
		virtual void onHover(LevelSelection* level) = 0;
		virtual void resetState(LevelSelection* level) = 0;
	};


	void setOnClickListener(OnClickListener* iOnC);


	virtual void onClick() override;
	virtual void onPress() override;
	virtual void onHover() override;
	virtual void resetState() override;

	virtual void setToUnpressedState();
	virtual void setToHoverState();
	virtual void setToSelectedState();



private:

	typedef void (OnClickListener::*OnClickFunction) (LevelSelection*);
	OnClickListener* onClickListener = NULL;
	OnClickFunction onClickFunction;
	OnClickFunction onHoverFunction;
	OnClickFunction onPressFunction;
	OnClickFunction onResetFunction;

	Sprite previewPic;
	unique_ptr<RectangleFrame> picFrame;
	unique_ptr<TextLabel> label;
	//unique_ptr<HitArea> hitArea;

	Vector4 frameColor;
	Vector4 labelColor;

	Vector4 hoverFrameColor = Vector4(0, 0, 0, .5f);
	Vector4 hoverLabelColor = Vector4(0, 0, 0, .8f);
};




class MenuManager;
class LevelSelectScreen : public Screen {
public:
	LevelSelectScreen(MenuManager* menuManager);
	virtual ~LevelSelectScreen();

	virtual void setGameManager(GameManager* game) override;
	virtual bool initialize(ComPtr<ID3D11Device> device) override;
	void reloadGraphicsAssets();

	virtual void update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;

	virtual void pause() override;
	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

	void loadLevel(string levelXMLFile);

	MenuManager* menuManager;
private:
	GameManager* game;
	unique_ptr<xml_document> levelManifest;

	//vector<unique_ptr<LevelSelection>> levelSelections;

	unique_ptr<TextLabel> titleLabel;
	SelectorManager selectorManager;
};


class OnClickLevelSelect : public LevelSelection::OnClickListener {
public:

	OnClickLevelSelect(LevelSelectScreen* screen) : levelSelectScreen(screen) {
	}
	virtual void onClick(LevelSelection* level) {
		levelSelectScreen->loadLevel(level->levelXMLFile);
	};
	virtual void onPress(LevelSelection* level) {
	};
	virtual void onHover(LevelSelection* level) {
	};
	virtual void resetState(LevelSelection* level) {
	};

private:
	LevelSelectScreen* levelSelectScreen;

};
