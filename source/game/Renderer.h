#pragma once

class  __declspec(novtable) Renderer
{
public:
	virtual ~Renderer() = default;

	virtual void Resize() = 0;

	virtual void Draw() = 0;
};