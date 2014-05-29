#pragma once

#include "IDrawable.h"
#include "IUpdatable.h"
#include <Windows.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dtypes.h>

struct PostProcessVERT
{
	float x, y, z, rhw;
	float tu, tv;       // Texcoord for post-process source
	float tu2, tv2;     // Texcoord for the original scene
};

const D3DVERTEXELEMENT9 Decl[4] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	D3DDECL_END()
};

class Engine
{
public:
	Engine();
	~Engine();

	void Init(HWND _hWnd);
	void Release(void);

	void AddSprite(std::wstring file);

	void Render(float gameTime);

private:
	IDirect3D9 *m_pD3DObject;
	IDirect3DDevice9 *m_pDevice;
	IDirect3DVertexDeclaration9	*m_pVertDeclPostProcess;
	IDirect3DTexture9* m_pTargetTexture;
	ID3DXEffect *m_pShader;

	std::vector<IDrawable*> *m_pSprites;
};

