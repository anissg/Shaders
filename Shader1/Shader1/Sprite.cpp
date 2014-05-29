#include "Sprite.h"


Sprite::Sprite()
{
	//Default the sprite to position (0, 0, 0)
	position.x = 0;
	position.y = 0;
	position.z = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);
}

Sprite::Sprite(float x, float y, float z)
{
	//Default Z to 0
	position.x = x;
	position.y = y;
	position.z = z;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);
}

bool Sprite::Initialize(LPDIRECT3DDEVICE9 device, std::wstring file, int width, int height)
{
	//Same functionality as D3DXCreateTextureFromFile EXCEPT width and height are manually entered
	if (!SUCCEEDED(D3DXCreateTextureFromFileEx(device, file.c_str(),
		width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, NULL, NULL, &tex)))
	{
		assert("Can not create the Texture.");
		return false;
	}

	//Attempt to create the sprite
	if (!SUCCEEDED(D3DXCreateSprite(device, &sprite)))
	{
		assert("Can not create the Sprite.");
		return false;
	}

	return true;
}

void Sprite::Draw(float gameTime)
{
	if (sprite && tex)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTSAVESTATE);

		sprite->Draw(tex, NULL, NULL, &position, color);

		sprite->End();
	}
}

void Sprite::Update(float gameTime)
{
	//Update 
}

Sprite::~Sprite()
{
	if (sprite)
	{
		sprite->Release();
		sprite = nullptr;
	}

	if (tex)
	{
		tex->Release();
		tex = nullptr;
	}
}
