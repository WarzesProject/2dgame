#pragma once

#include "oldIGameScreen.h"

class MainMenuScreen : public oldIGameScreen
{
public:
	MainMenuScreen();
	~MainMenuScreen();

	virtual void Build() override;
	virtual void Destroy() override;

	virtual void OnEntry() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual int GetNextScreenIndex() const override;
	virtual int GetPreviousScreenIndex() const override;
private:
};