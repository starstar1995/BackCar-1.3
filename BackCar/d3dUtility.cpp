#include"d3dUtility.h"

//��ʼ�� d3d
bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width,
	int height,
	ID3D11RenderTargetView ** renderTargetView,//Ŀ����Ⱦ��ͼ����
	ID3D11DeviceContext ** immediateContext,//ִ�������Ľӿ�
	IDXGISwapChain ** swapChain,//�������ӿ�
	ID3D11Device ** device)//�豸�ýӿڣ�ÿ��D3D����������һ���豸
{
	//������һ������
	//���裺1.���һ��������2.ע�ᴰ����3.��������4.������ʾ�͸���
	//1.��ƴ���
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Direct3D11App";

	//2.ע�ᴰ��
	if (!RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	//3.��������
	HWND hwnd = ::CreateWindow(L"Direct3D11App",
		L"D3D11",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		0,
		0,
		hInstance,
		0);
	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	//4.������ʾ�͸���
	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);
	//�������ڽ���  ��ʼ��ʼ�� D3D
	//��ʼ��D3D���� 1.�����������������DXGI_SQAP_CHAIN_DESC�ṹ
	//2.ʹ��D3D11CreateDcviceAndSwapChain����D3D�豸(ID3D11Device) ִ�������Ľӿ�(ID3DeviceContext),���� ���ӿ�(IDXGISwapChain)
	//3.����Ŀ����Ⱦ����(ID3D11RenderTargetView)
	//4.�����ӿ�(View Port)

	//��һ��
	DXGI_SWAP_CHAIN_DESC sd;//������DXGI_SWAP_CHAIN_DESC�Ķ���sd
	ZeroMemory(&sd, sizeof(sd));//��ZeroMemory��sd���г�ʼ��
	sd.BufferCount = 1;//�������к�̨����������ͨ��Ϊ1
	sd.BufferDesc.Width = width;//�������Ĵ��ڿ�
	sd.BufferDesc.Height = height;//�������Ĵ��ڸ�
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//��ʾ������Alpha��8λ
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//�ڶ���
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	//��ȡ D3D_FEATURE_LEVEL�����Ԫ�ظ���
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//����D3D11CreateDeviceAndSwapChain�������������豸��ִ��������
	//�ֱ����swapChain,devoce,immedicateContext
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		swapChain,
		device,
		NULL,
		immediateContext)))
	{
		::MessageBox(0, L"CreateDevice - FAILED", 0, 0);
		return false;
	}

	//������
	HRESULT hr = 0;
	ID3D11Texture2D * pBackBuffer = NULL;
	//����GetBuffer()�����õ���̨������󣬲�������&pBackBuffer��
	hr = (*swapChain)->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	//�ж�getbuffer�Ƿ���óɹ�
	if (FAILED(hr))
	{
		::MessageBox(0, L"GetBuffer - FAILED", 0, 0);
		return false;
	}

	//����createredertargetview��������ȾĿ����ͼ
	//���������rendertargetview��
	hr = (*device)->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		renderTargetView);

	pBackBuffer->Release();
	//�ж�CreateRenderTargetView�Ƿ���óɹ� 
	if (FAILED(hr))
	{
		::MessageBox(0, L"GetRender - FAILED", 0, 0);
		return false;
	}

	//����ȾĿ����ͼ�󶨵���Ⱦ����
	(*immediateContext)->OMSetRenderTargets(1,
		renderTargetView,
		NULL);

	//���Ĳ�
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0,
		vp.TopLeftY = 0;

	//�����ӿ�
	(*immediateContext)->RSSetViewports(
		1,
		&vp);
	return true;
}


//��ʼ�� d3d ����
//��Ϣѭ������
int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//�ڶ��λ�ȡ��ǰʱ��
			float currTime = (float)timeGetTime();
			//��ȡ�����¼�֮���ʱ���
			float timeDelta = (currTime - lastTime) * 0.001f;
			//������ʾ������
			ptr_display(timeDelta);
			lastTime = currTime;
		}
	}
	return msg.wParam;
}