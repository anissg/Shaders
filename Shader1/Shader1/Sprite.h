#pragma once

#include "IDrawable.h"
#include "IUpdatable.h"
#include <string>
#include <cassert>
#include <d3dx9.h>


class Sprite :public IDrawable, IUpdatable
{
public:
	Sprite();
	Sprite(float x, float y, float z);
	~Sprite();

	//Sprite Functions
	bool Initialize(LPDIRECT3DDEVICE9 device, std::wstring file, int width, int height);

	virtual void Update(float gameTime);
	virtual void Draw(float gameTime);
private:
	//Pointers
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	//Attributes
	D3DXVECTOR3 position;
	D3DCOLOR color;
};

