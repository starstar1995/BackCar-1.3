#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include<Windows.h>
//XNA��ѧ�����ͷ�ļ�
#include<d3dcompiler.h>
#include<xnamath.h>

//DirectX11���ͷ�ļ�
#include<D3D11.h>
#include<D3DX11.h>
#include<d3dx11effect.h>

//Direct11��ؿ�
#pragma comment(lib,"Effects11")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

namespace d3d//����һ��d3d�����ռ�
{
	//��ʼ��d3d
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		ID3D11RenderTargetView ** renderTargetView,//Ŀ����Ⱦ��ͼ����
		ID3D11DeviceContext ** immediateContext,//ִ�������Ľӿ�
		IDXGISwapChain ** swapChain,//�������ӿ�
		ID3D11Device ** device);//�豸�ýӿڣ�ÿ��D3D����������һ���豸
	//��Ϣѭ��
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//�ص�����
	LRESULT CALLBACK WndProc(
		HWND,
		UINT msg,
		WPARAM,
		LPARAM IParam);
}
#endif