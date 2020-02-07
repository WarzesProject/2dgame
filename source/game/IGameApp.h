#pragma once

class __declspec(novtable) IGameApp
{
public:
	virtual ~IGameApp();

	virtual void OnInit() = 0;

protected:

};