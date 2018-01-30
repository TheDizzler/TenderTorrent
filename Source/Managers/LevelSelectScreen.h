#pragma once


class LevelSelection {
public:

	LevelSelection(const Vector2& position, pugi::xml_node levelNode);
	virtual ~LevelSelection();

	void reloadGraphicsAssets();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	const Vector2& getSize();

	bool isHover = false;
	bool isPressed = false;
	bool isClicked = false;

	class OnClickListener {
	public:
		virtual void onClick(LevelSelection* level) = 0;
	};


	void setOnClickListener(OnClickListener* iOnC) {
		if (onClickListener != NULL)
			delete onClickListener;
		onClickFunction = &OnClickListener::onClick;
		onClickListener = iOnC;
	}

	void onClick() {
		if (onClickListener != NULL) {
			isClicked = isPressed = false;
			(onClickListener->*onClickFunction)(this);
		}
	}


	void setOnHoverListener(OnClickListener* iOnC) {
		if (!onHoverListener != NULL)
			delete onHoverListener;
		onHoverFunction = &OnClickListener::onClick;
		onHoverListener = iOnC;

	}

	void onHover() {
		if (onHoverListener != NULL) {
			(onHoverListener->*onHoverFunction)(this);
		}
	}

	unique_ptr<Sprite> previewPic;
	unique_ptr<RectangleFrame> picFrame;
	unique_ptr<TextLabel> label;
	string levelXMLFile;
	unique_ptr<HitArea> hitArea;

	typedef void (OnClickListener::*OnClickFunction) (LevelSelection*);
	OnClickFunction onClickFunction;
	OnClickListener* onClickListener = NULL;
	OnClickFunction onHoverFunction;
	OnClickListener* onHoverListener = NULL;

	virtual void setToUnpressedState();
	virtual void setToHoverState();
	virtual void setToSelectedState();

	Vector4 frameColor;
	Vector4 labelColor;

	Vector4 hoverFrameColor = Vector4(0, 0, 0, .5);
	Vector4 hoverLabelColor = Vector4(0, 0, 0, .8);
};




class MenuManager;
class LevelSelectScreen : public Screen {
public:
	LevelSelectScreen(MenuManager* menuManager);
	virtual ~LevelSelectScreen();

	virtual void setGameManager(GameManager * game) override;
	virtual bool initialize(ComPtr<ID3D11Device> device) override;
	void reloadGraphicsAssets();

	virtual void update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;
	//virtual void safedraw(SpriteBatch* batch) override;

	virtual void pause() override;
	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

	void loadLevel(string levelXMLFile);

	MenuManager* menuManager;
private:
	GameManager* game;
	unique_ptr<xml_document> levelManifest;

	vector<unique_ptr<LevelSelection>> levelSelections;

	unique_ptr<TextLabel> titleLabel;
};


class OnClickLevelSelect : public LevelSelection::OnClickListener {
public:

	OnClickLevelSelect(LevelSelectScreen* screen) : levelSelectScreen(screen) {
	}
	virtual void onClick(LevelSelection* level) {
		levelSelectScreen->loadLevel(level->levelXMLFile);

	};

private:
	LevelSelectScreen* levelSelectScreen;

};
