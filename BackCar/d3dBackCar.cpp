#include"d3dUtility.h"
#include "Camera.h"
#include "ObjLoader.h"

ID3D11Device *device = NULL;  //D3D11�豸�ӿ�
IDXGISwapChain *swapChain = NULL;  //�������ӿ�
ID3D11DeviceContext *immediateContext = NULL;
ID3D11RenderTargetView *renderTargetView = NULL;  //��ȾĿ����ͼ

//Effect���ȫ��ָ��
ID3D11InputLayout *vertexLayout;
ID3DX11Effect *effect;
ID3DX11EffectTechnique *technique;

//������������ϵ����
XMMATRIX world; //��������任�ľ���
XMMATRIX view;  //���ڹ۲�任�ľ���
XMMATRIX projection;  //����ͶӰ�任�ľ���

ID3D11BlendState *blendStateAlpha;        //���״̬
ID3D11RasterizerState* noCullRS;          //��������״̬

ID3D11DepthStencilView* depthStencilView;  //���ģ����ͼ

ID3D11ShaderResourceView *texture;  //��·���������
ID3D11ShaderResourceView *textureGrass;  //�㳡���������
ID3D11ShaderResourceView *textureCar;  //�������������
ID3D11ShaderResourceView *textureHeart1; //����ֵ���ڵ��������
ID3D11ShaderResourceView *textureHeart2; //����ֵʧȥ���������
ID3D11ShaderResourceView *texturehouse1Front;  //���ӵ��������
ID3D11ShaderResourceView *texturehouse1Right;
ID3D11ShaderResourceView *texturehouse1Left;
ID3D11ShaderResourceView *texturehouse1Top;
ID3D11ShaderResourceView *texturehouse1Behand;
ID3D11ShaderResourceView *textureNumber;//���ֵ��������
ID3D11ShaderResourceView *textureRoad;
ID3D11ShaderResourceView *textureRoad3;
ID3D11ShaderResourceView *textureTime0;//��ʱ����
ID3D11ShaderResourceView *textureTime1;
ID3D11ShaderResourceView *textureTime2;
ID3D11ShaderResourceView *textureTime3;
ID3D11ShaderResourceView *textureTime4;
ID3D11ShaderResourceView *textureTime5;
ID3D11ShaderResourceView *textureTime6;
ID3D11ShaderResourceView *textureTime7;
ID3D11ShaderResourceView *textureTime8;
ID3D11ShaderResourceView *textureTime9;
ID3D11ShaderResourceView *textureBackground1;
ID3D11ShaderResourceView *textureBackground2;
ID3D11ShaderResourceView *textureBackground3;

ID3D11ShaderResourceView *textureLight;

ID3D11ShaderResourceView *textureWin;
ID3D11ShaderResourceView *textureGameOver;//��Ϸ�������

ID3D11ShaderResourceView *texturebusFront;  //�������������
ID3D11ShaderResourceView *texturebusRight;
ID3D11ShaderResourceView *texturebusLeft;
ID3D11ShaderResourceView *texturebusTop;
ID3D11ShaderResourceView *texturebusBehand;

ID3D11ShaderResourceView *texturebus2Front;  //����2���������
ID3D11ShaderResourceView *texturebus2Right;
ID3D11ShaderResourceView *texturebus2Left;
ID3D11ShaderResourceView *texturebus2Top;
ID3D11ShaderResourceView *texturebus2Behand;

ID3D11ShaderResourceView *texturebus3Front;  //����3���������
ID3D11ShaderResourceView *texturebus3Right;
ID3D11ShaderResourceView *texturebus3Left;
ID3D11ShaderResourceView *texturebus3Top;
ID3D11ShaderResourceView *texturebus3Behand;

//����һ������ṹ����������������ͷ�����
struct Vertex
{
	XMFLOAT3 Pos;  //����
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;  //��������
};

ObjLoader* objLoaderBus;
Camera* camera = new Camera();    //���������
XMVECTOR Eye;                     //�ӵ�λ��

bool Setup()
{
	//****��һ��:��.fx�ļ�����ID3DEffect����****//
	HRESULT hr = S_OK;  //����HRESULT�Ķ������ڼ�¼���������Ƿ�ɹ�
	ID3DBlob *pTechBlob = NULL;  //����ID3DBlob�Ķ������ڴ�Ŵ��ļ���ȡ����Ϣ
	hr = D3DX11CompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx�ļ�����ʧ��", L"Error", MB_OK);
		return hr;
	}

	//����D3DX11CreateEffectFromMemory����ID3DEffect����
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,
		device,
		&effect
		);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Effectʧ��", L"Error", MB_OK);
		return hr;
	}

	//����D3DX11CreateShaderResourceViewFromFile��BOX.BMP�ļ���������

	D3DX11CreateShaderResourceViewFromFile(device, L"road.jpg", NULL, NULL, &textureRoad, NULL);
	
	D3DX11CreateShaderResourceViewFromFile(device, L"road3.jpg", NULL, NULL, &textureRoad3, NULL);

	D3DX11CreateShaderResourceViewFromFile(device,L"road2.jpg",NULL,NULL,&texture,NULL);

	D3DX11CreateShaderResourceViewFromFile(device,L"BOX.bmp",NULL,NULL,&textureCar,NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"grass.jpg", NULL, NULL, &textureGrass, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1front.jpg", NULL, NULL, &texturehouse1Front, NULL);
	
	D3DX11CreateShaderResourceViewFromFile(device, L"house1right.jpg", NULL, NULL, &texturehouse1Right, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1left.jpg", NULL, NULL, &texturehouse1Left, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1top.jpg", NULL, NULL, &texturehouse1Top, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1behand.jpg", NULL, NULL, &texturehouse1Behand, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"��1.jpg", NULL, NULL, &textureHeart1, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"��2.jpg", NULL, NULL, &textureHeart2, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"��������.jpg", NULL, NULL, &textureNumber, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time0.jpg", NULL, NULL, &textureTime0, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time1.jpg", NULL, NULL, &textureTime1, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time2.jpg", NULL, NULL, &textureTime2, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time3.jpg", NULL, NULL, &textureTime3, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time4.jpg", NULL, NULL, &textureTime4, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time5.jpg", NULL, NULL, &textureTime5, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time6.jpg", NULL, NULL, &textureTime6, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time7.jpg", NULL, NULL, &textureTime7, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time8.jpg", NULL, NULL, &textureTime8, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time9.jpg", NULL, NULL, &textureTime9, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"busfront.png", NULL, NULL, &texturebusFront, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"busright.png", NULL, NULL, &texturebusRight, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"busleft.png", NULL, NULL, &texturebusLeft, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bustop.png", NULL, NULL, &texturebusTop, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"busbehand.png", NULL, NULL, &texturebusBehand, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus2front.png", NULL, NULL, &texturebus2Front, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus2right.png", NULL, NULL, &texturebus2Right, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus2left.png", NULL, NULL, &texturebus2Left, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus2top.png", NULL, NULL, &texturebus2Top, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus2behand.png", NULL, NULL, &texturebus2Behand, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus3front.png", NULL, NULL, &texturebus3Front, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus3right.png", NULL, NULL, &texturebus3Right, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus3left.png", NULL, NULL, &texturebus3Left, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus3top.png", NULL, NULL, &texturebus3Top, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"bus3behand.png", NULL, NULL, &texturebus3Behand, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"background1.jpg", NULL, NULL, &textureBackground1, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"background1.jpg", NULL, NULL, &textureBackground2, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"background1.jpg", NULL, NULL, &textureBackground3, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"light.jpg", NULL, NULL, &textureLight, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"youwin.png", NULL, NULL, &textureWin, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"GAMEOVER.png", NULL, NULL, &textureGameOver, NULL);

	//*************�������������벼��****************************************************
	//��GetTechniqueByName��ȡID3DX11EffectTechnique�Ķ���
	technique = effect->GetTechniqueByName("TexTech");

	//D3DX11_PASS_DESC�ṹ��������һ��Effect Pass
	D3DX11_PASS_DESC PassDesc;
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//�������������벼��
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//layoutԪ�ظ���
	UINT numElements = ARRAYSIZE(layout);

	//����CreateInputLayout�������벼��
	hr = device->CreateInputLayout
		(
		layout,
		numElements,
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&vertexLayout
		);
	//�������ɵ����벼�ֵ�ִ����������
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Input Layoutʧ��", L"Error", MB_OK);
		return hr;
	}

	//*************�������������벼��****************************************************

	//*************���Ĳ��������㻺��****************************************************
static float typr = 2.0f;
static float typgrass = 2.0f;
static float xhouse1 =10.0f;
static float zhouse1 =10.0f;
static float xbus = 10.0f;
static float zbus = 10.0f;
static float typghouse = 1.0f;
static float tyWidth = 9.0f;
static float tyHeight = 1.0f;
static float typtime = 0.1f;
static float xtime = 3.0f;
static float ytime = 5.0f;
static float ztime = 1.0f;
static float tyHeart = 8.0f;
static float typback1 = 2.0f;
static float xlight = 17.0f;
static float zlight = 13.0f;
static float xHeart = 2.0f;

	Vertex vertices[] =
	{
		

		{ XMFLOAT3(1.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//8
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		//19
		{ XMFLOAT3(6.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		//
		{ XMFLOAT3(6.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//25
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
//28


		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//44
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(0.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//66*6

		//grass
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 15.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 15.0f) },
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 15.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },

		
		//house1
		//front
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse +  zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse +  zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//behand
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//left
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//right
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//top
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//car 36
		//car  36
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 3.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		//heart front
		{ XMFLOAT3(10.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(11.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(11.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(11.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(12.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(12.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(12.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(13.0f + xHeart, 5.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f + xHeart, 4.0f + tyHeart, 28.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },


		////heart behind
		//{ XMFLOAT3(10.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(10.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(10.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(13.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(13.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(13.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		// ��·���䣺

		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },




		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		

		//ͣ����
		{ XMFLOAT3(1.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//540
		//���1
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		
		//���2
		{ XMFLOAT3(8.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//552

		//��ʱ
		{ XMFLOAT3(0.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//bus
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//
		/*{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },*/
		//behand
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//
		/*{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },*/
		//left
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//
		/*{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },*/
		//right
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//
		/*{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },*/
		//top
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//background 1
		{ XMFLOAT3(0.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//2
		{ XMFLOAT3(0.0f*typback1, 0.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typback1, 7.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typback1, 7.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//3
		{ XMFLOAT3(14.0f*typback1, 0.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typback1, 0.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typback1, 0.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typback1, 7.0f*typback1, 14.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typback1, 7.0f*typback1, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//bus2
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		
		//behand
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//left
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		
		//right
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		
		//top
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//bus3
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//behand
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//left
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//right
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 0.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//top
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 0.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typghouse + xbus, 4.0f*typghouse, 3.0f*typghouse + zbus), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//dengzhu 1
		//front
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//behand		+xlight	2		  +zlight
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//left			+xlight	2		  +zlight
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//right			+xlight	2		  +zlight
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 0.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//top			+xlight	2		  +zlight
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.1f + xlight, 3.0f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//606+30

		//dengzhu 2
		//front
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//behand			2		
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//left				2		
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//right				2		
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//top				2		
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.9f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//down
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.1f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//light rgy
		{ XMFLOAT3(2.5f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 3.1f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.4f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.4f + xlight, 2.8f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.5f + xlight, 3.1f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.4f + xlight, 3.1f, 0.0f + zlight), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//ͣ��λ
		{ XMFLOAT3(5.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(5.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//result
		{ XMFLOAT3(5.0f*typr, 4.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 4.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 3.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 3.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 4.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 3.0f*typr, 0.0f*typback1), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

	};
	UINT vertexCount = ARRAYSIZE(vertices);

	//�������㻺��
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;  //������36������
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//����һ���������ڳ�ʼ������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	//����һ��D3D11Buffer������Ϊ���㻺��
	ID3D11Buffer *vertexBuffer;
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����VertexBufferʧ��", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//���ö��㻺��
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//ָ��ͼԪ����
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************����obj��������ͼ���ļ�***************************************
	/*objLoaderBus = new ObjLoader(effect);
	objLoaderBus->Load(device, "media/", "file.obj", ObjLoader::Left_Hand_System);*/

	Eye = XMVectorSet(8.0f, 3.0f, -5.0f, 0.0f);//���λ��

	camera->SetEye(Eye);   //�����ӵ�λ��

}

void Cleanup()
{
	//�ͷ�ȫ��ָ��
	if (renderTargetView)renderTargetView->Release();
	if (immediateContext)immediateContext->Release();
	if (swapChain)swapChain->Release();
	if (device)device->Release();

	if (vertexLayout)vertexLayout->Release();
	if (effect)effect->Release();
	if (texture)texture->Release();
	if (textureCar)textureCar->Release();
}

bool Display(float timeDelta  )
{
	if (device)
	{
		float ClearColor[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		//��ָ��������ӣ�һ�㲻����������������������ָ��Ϊʹ��blend factor
		float BlendFactor[] = { 0, 0, 0, 0 }; 

		ID3D11ShaderResourceView *textureT[] = { textureTime0, textureTime1, textureTime2,
			textureTime3, textureTime4, textureTime5, textureTime6, textureTime7,
			textureTime8, textureTime9 };
		/*static float angle = XM_PI;
		static float height = 5.0f;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 2.0f*timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 2.0f*timeDelta;*/
		/*if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f*timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f*timeDelta;*/

		//if (height < 0.0f) height = 0.f;   //���ƾ�ͷ��Զ����
		//if (height > 12.0f) height = 12.f;     //���ƾ�ͷ�������

		//****�����������ñ任����ϵ****
		//****���ø�������ϵ****//
		//��ʼ���������
		//world = XMMatrixIdentity();

		////��ʼ���۲����
		//XMVECTOR Eye = XMVectorSet(8.0f, 5.0f, -3.0f, 0.0f);//���λ��
		//XMVECTOR At = XMVectorSet(8.0f, 5.0f, 8.0f, 0.0f);//Ŀ��λ��
		//XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//������Ϸ�λ��
		//view = XMMatrixLookAtLH(Eye, At, Up);  //���ù۲�����ϵ

		////����ͶӰ����
		//projection = XMMatrixPerspectiveFovLH(
		//	XM_PIDIV2,
		//	800.0f / 600.0f,
		//	0.01f,
		//	100.0f);
		////������������ϵ
		//effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		////���ù۲�����ϵ
		//effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);

		////����ͶӰ����ϵ
		//effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection);

		//ͨ�����̸ı���������ͷ����.
		if (::GetAsyncKeyState('A') & 0x8000f)  //�����ƶ�
		{
			camera->MoveRightBy(-timeDelta * 5.0f);
		}

		if (::GetAsyncKeyState('D') & 0x8000f)  //�����ƶ�
		{
			camera->MoveRightBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('W') & 0x8000f)  //��ǰ�ƶ�
		{
			camera->MoveForwardBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('S') & 0x8000f)  //����ƶ�
		{
			camera->MoveForwardBy(-timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('Q') & 0x8000f)  //�����ƶ�
		{
			camera->MoveUpBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('E') & 0x8000f) //�����ƶ�
		{
			camera->MoveUpBy(-timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('Z') & 0x8000f) //���Ұڶ�
		{
			camera->Yaw(timeDelta);
		}

		if (::GetAsyncKeyState('X') & 0x8000f) //����ڶ�
		{
			camera->Yaw(-timeDelta);
		}

		if (::GetAsyncKeyState('C') & 0x8000f) //����
		{
			camera->Pitch(timeDelta);
		}

		if (::GetAsyncKeyState('V') & 0x8000f) //�¸�
		{
			camera->Pitch(-timeDelta);
		}
		//�������ɹ۲����
		camera->Apply();

		//��ʼ���������
		world = XMMatrixIdentity();

		//����ͶӰ����
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//������任����ĳ��������еľ�����������õ�Effect�����---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		XMMATRIX ViewMATRIX = XMLoadFloat4x4(&camera->GetView());
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&ViewMATRIX);    //���ù۲�����ϵ
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //�����ӵ�
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //����ͶӰ����ϵ
		//����һ��D3DX11_TECHNIQUE_DESC����������technique
		//D3DX11_TECHNIQUE_DESC techDesc;
		//technique->GetDesc(&techDesc);  //��ȡtechnique������
		//technique->GetPassByIndex(0)->Apply(0, immediateContext);
		
		static float beginTime = (float)timeGetTime();
		//���Ʋݵ�����
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureGrass);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 396);
		//���ƹ�·������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(396, 0);

		//world = XMMatrixIdentity();
		//world = XMMatrixRotationY(angle_car)*XMMatrixTranslation(transX + 4.0f, 0.0f, transZ+4.0f)*XMMatrixScaling(0.5f, 0.5f, 0.5f);
		//effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		//effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCar);
		//technique->GetPassByIndex(0)->Apply(0, immediateContext);
		//immediateContext->Draw(36, 432);
		//objLoaderBus->RenderEffect(immediateContext, technique);

		world = XMMatrixIdentity();
		//world = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		//background1
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBackground1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(18, 588);

		//world = XMMatrixIdentity();
		//world = XMMatrixScaling(0.5f, 0.5f, 0.5f);// *XMMatrixTranslation(0.0f, height - 5.0f, 0.0f);
		//effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		//effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber);
		//technique->GetPassByIndex(0)->Apply(0, immediateContext);
		//immediateContext->Draw(6, 486);

		/*float endTime = (float)timeGetTime();
		float timeInterval = endTime - beginTime;*/
		//if (transZ > 5.0f)
		//{
		//	world = XMMatrixIdentity();
		//	world = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		//	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		//	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart2);
		//	technique->GetPassByIndex(0)->Apply(0, immediateContext);
		//	immediateContext->Draw(6, 480);
		//}

		//���Ʋ��乫·
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(48, 486);
		//����ͣ����
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad3);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 534);
		//���1
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 540);
		//���2
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 546);

		//��λ
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 738);

		//���Ƽ�ʱ��
		static int timeboard[4] = { 0, 0, 0, 0 };
		static float GameTime = 0.0f;
		GameTime += timeDelta;
		int GameTimer;
		if (GameTime<10000.0f)
		{
			GameTimer = (int)GameTime;
			for (int i = 0; i < 4; i++)
			{

				timeboard[i] = GameTimer % 10;
				GameTimer /= 10;
			}
		}

		world = XMMatrixIdentity();
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[3]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);

		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[2]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);


		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[1]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);


		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[0]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);

		world = XMMatrixIdentity();
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Front);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 402);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Left);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 414);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Right);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 420);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Top);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 426);

		//��������������
		static float transX = 4.0f;
		static float transZ = 4.0f;
		static float angle_car = XM_PI / 2;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		{
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				angle_car -= 0.5f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				angle_car += 0.5f * timeDelta;
			transX -= cos(angle_car) * 8.0f * timeDelta;
			transZ += sin(angle_car) * 8.0f * timeDelta;

		}

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		{
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				angle_car += 0.5f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				angle_car -= 0.5f * timeDelta;
			transX += cos(angle_car) * 5.0f * timeDelta;
			transZ -= sin(angle_car) * 5.0f * timeDelta;
		}

		if (angle_car > XM_PI * 2)angle_car -= XM_PI * 2;

		//��ײ����߼�
		bool s0, s1, s2, s3, s4, s5, s6, s7, s8;
		static int life = 3;
		static float typr = 2.0f;
		s0 = transX <= 0 * typr && transZ <= 5 * typr;
		s1 = transZ >= 5 * typr&& transZ <= 7 * typr && transX <= 9 * typr;
		s2 = transX >= 3 * typr && transZ <= 3 * typr && transX <= 11 * typr;
		s3 = (transX >= 11 * typr) && (transZ >= 3 * typr) && (transZ <= 7 * typr);
		s4 = transX >= 11 * typr&&transZ >= 9 * typr;
		s5 = transX <= 9 * typr &&transX >= 3 * typr&& transZ >= 9 * typr&&transZ <= 10 * typr;
		s6 = transX <= 2 * typr &&transX >= 1 * typr&& transZ >= 9 * typr&&transZ <= 10 * typr;
		s7 = transX <= 5 * typr &&transX >= 1 * typr&& transZ >= 11 * typr&&transZ <= 14 * typr;
		s8 = transX <= 9 * typr &&transX >= 6 * typr&& transZ >= 11 * typr&&transZ <= 14 * typr;

		if (s0){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s1){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s2){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s3){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s4){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;

		}
		if (s5){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s6){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s7){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}
		if (s8){
			transX = 1.0f*typr;
			transZ = 1.0f*typr;
			life--;
		}

		if (life <= 0)//����ֵ�ù⣬��Ϸ����
		{
			world = XMMatrixIdentity();
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureGameOver);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 750);
		}
		if (transX == 5.5&&transZ == 13)//��Ϸ�ɹ��߼���������
		{
			world = XMMatrixIdentity();
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureWin);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 750);
		}

		//��������3
		world = XMMatrixIdentity();
		world = XMMatrixRotationY(angle_car)*XMMatrixTranslation(transX, 0.0f, transZ)*XMMatrixScaling(0.2f, 0.2f, 0.2f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Left);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 558);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Right);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 564);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Front);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 570);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Behand);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 576);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Top);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 582);

		//��������2
		world = XMMatrixIdentity();
		world = XMMatrixRotationY(XM_PIDIV2)*XMMatrixTranslation(50, 0.0f, 120)*XMMatrixScaling(0.2f, 0.2f, 0.2f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Left);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 606);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Right);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 612);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Front);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 618);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Behand);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 624);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus2Top);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 630);

		//��������3
		world = XMMatrixIdentity();
		world = XMMatrixRotationY(XM_PIDIV2)*XMMatrixTranslation(47, 0.0f, 120)*XMMatrixScaling(0.2f, 0.2f, 0.2f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus3Left);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 636);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus3Right);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 642);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus3Front);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 648);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus3Behand);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 654);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturebus3Top);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 660);

		//���̵�
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureCar);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(66, 666);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureLight);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 732);


		//if (angle_car >= 0 && angle_car < XM_PI / 2)
		//	Eye = XMVectorSet(transX - 10.0f, 3.0f, transZ - 10.0f, 0.0f);//���λ��
		//if (angle_car >= XM_PI/2 && angle_car<XM_PI)
		//	Eye = XMVectorSet(transX + 10.0f, 3.0f, transZ - 10.0f, 0.0f);//���λ��
		//if (angle_car >= XM_PI && angle_car<3/2 * XM_PI)
		//	Eye = XMVectorSet(transX + 10.0f, 3.0f, transZ + 10.0f, 0.0f);//���λ��
		//if (angle_car >=3/2 * XM_PI  && angle_car<XM_PI * 2)
		//	Eye = XMVectorSet(transX - 10.0f, 3.0f, transZ + 10.0f, 0.0f);//���λ��
		//camera->SetEye(Eye);   //�����ӵ�λ��

		/*world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 468);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 474);
		if (transZ>5.0)
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart2);
		else effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 480);*/

		world = XMMatrixIdentity();
		world = XMMatrixScaling(2.0f, 2.0f, 2.0f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		for (int i = life; i > 0; i--){
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 468 + (i - 1) * 6);
		}
		for (int j = 3 - life; j > 0; j--){
			if (j > 3)j = 3;
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart2);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 480 - (j - 1) * 6);
		}
		swapChain->Present(0, 0);
	}
	return true;
}

//�ص�����
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM IParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return::DefWindowProc(hwnd, msg, wParam, IParam);
}


int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		800,
		600,
		&renderTargetView,
		&immediateContext,
		&swapChain,
		&device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	return 0;
}