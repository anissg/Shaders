#include "Engine.h"
#include "Sprite.h"
#include <cassert>

Engine::Engine()
{
	this->m_pD3DObject = nullptr;
	this->m_pDevice = nullptr;
	this->m_pSprites = nullptr;
	this->m_pVertDeclPostProcess = nullptr;
	this->m_pTargetTexture = nullptr;
	this->m_pShader = nullptr;
}


Engine::~Engine()
{
}


void Engine::Init(HWND _hWnd)
{
	this->m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (this->m_pD3DObject == nullptr)
		assert("Can not create direct 3d object!");

	//create presentation param to create device
	D3DPRESENT_PARAMETERS presentParam;
	ZeroMemory(&presentParam, sizeof(presentParam));

	presentParam.BackBufferWidth = 800;
	presentParam.BackBufferHeight = 600;
	presentParam.hDeviceWindow = _hWnd;
	presentParam.Windowed = true;
	presentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.AutoDepthStencilFormat = D3DFMT_D16;
	presentParam.EnableAutoDepthStencil = true;

	//create device
	HRESULT res = this->m_pD3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &this->m_pDevice);

	if (res != S_OK)
		assert("Can not create the device!");

	//create shader
	res = D3DXCreateEffectFromFileA(this->m_pDevice, "plasma.fx", 0, 0, D3DXSHADER_DEBUG, 0, &this->m_pShader, 0);

	if (res != S_OK)
		assert("Can not create the shader!");

	this->m_pDevice->CreateTexture(	presentParam.BackBufferWidth,
									presentParam.BackBufferHeight,
									1,
									D3DUSAGE_RENDERTARGET,
									D3DFMT_A8R8G8B8,
									D3DPOOL_DEFAULT,
									&this->m_pTargetTexture,
									NULL);

	this->m_pDevice->CreateVertexDeclaration(Decl, &this->m_pVertDeclPostProcess);

	//init sprite vector
	this->m_pSprites = new std::vector<IDrawable*>();

};

void Engine::AddSprite(std::wstring file)
{
	Sprite* s = new Sprite(0.0f,0.0f,0.0f);
	s->Initialize(this->m_pDevice, file, 1000, 950);
	this->m_pSprites->push_back(s);
};

void Engine::Release(void)
{

};



void Engine::Render(float gameTime)
{
	//Render to texture
	IDirect3DSurface9* OldRT		= nullptr;
	IDirect3DSurface9* targTexSurf	= nullptr;

	m_pDevice->GetRenderTarget(0, &OldRT);
	this->m_pTargetTexture->GetSurfaceLevel(0, &targTexSurf);
	m_pDevice->SetRenderTarget(0, targTexSurf);

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
	m_pDevice->BeginScene();
	{
		for (std::vector<IDrawable*>::iterator i = this->m_pSprites->begin(); i < this->m_pSprites->end(); i++)
		{
			(*i)->Draw(0.0f);
		}	
	}
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);

	//Post precessing render
	m_pDevice->SetRenderTarget(0, OldRT);
	IDirect3DSurface9 *d3dsurf = nullptr;
	D3DSURFACE_DESC *m_pD3DSurfDesc = new D3DSURFACE_DESC();
	HRESULT Res = m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dsurf);
	d3dsurf->GetDesc(m_pD3DSurfDesc);

	m_pDevice->BeginScene();
	{
		PostProcessVERT quad[4] =
		{
			{ 0,					 0,							0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			{ m_pD3DSurfDesc->Width, 0,							0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0,					 m_pD3DSurfDesc->Height,	0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
			{ m_pD3DSurfDesc->Width, m_pD3DSurfDesc->Height,	0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }
		};

		m_pDevice->SetVertexDeclaration(this->m_pVertDeclPostProcess);
		m_pShader->SetTechnique("plasma_technique");
		m_pShader->SetTexture("ScreenTexture", this->m_pTargetTexture);
		m_pShader->SetFloat("time", gameTime);

		unsigned int nb_passes = 0;
		this->m_pShader->Begin(&nb_passes, 0);
		{
			for (unsigned int i = 0; i < nb_passes; i++)
			{
				this->m_pShader->BeginPass(i);
				{
					m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(PostProcessVERT));
				}
				this->m_pShader->EndPass();
			}
		}
		this->m_pShader->End();
	}


	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
};
