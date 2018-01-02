#ifndef OBJLOADER_H_H
#define OBJLOADER_H_H

#include "d3dUtility.h"
#include <string>
#include <vector>
#include <fstream>

/// mtl�ļ�������Ĳ��ʽṹ����
class ObjMaterial
{
public:
	char						strName[1024];
	XMFLOAT4					vAmbient;	    //�����ⷴ����
	XMFLOAT4					vDiffuse;	    //ɢ��ⷴ����
	XMFLOAT4					vSpecular;	    //����ⷴ����
	int							nShininess;		//���淴��ϵ��
	ID3D11ShaderResourceView*	pTextureRV;     //�������

public:
	ObjMaterial();
	void Release();
};

/// �������ݽṹ����
struct ObjVertex
{
	XMFLOAT3					vPosition;  //λ������
	XMFLOAT2					vTexCoord;  //��������
	XMFLOAT3					vNormal;    //������
};

/// ������ṹ����
class ObjSubMesh
{
public:
	// ��������
	std::string					strMaterialName;
	// ������ͼ
	ID3D11ShaderResourceView*	pTextureRV;
	// �Ӽ���Ӧ��������ʼ���
	unsigned long				lStartIndex;
	// �Ӽ���Ӧ��������Ŀ
	unsigned long				lIndexCount;
public:
	ObjSubMesh();
	void Release();
};

/// OBJģ��������
class ObjLoader
{
public:
	// ����ģ�͵Ĺ�����ʹ�õ�����ϵ����������ϵ������������ϵ
	enum
	{
		Left_Hand_System = 1,
		Right_Hand_System
	};
public:
	ObjLoader();
	ObjLoader(ID3DX11Effect* objeffect);
	// ���ļ�����ģ��
	// resPath��ģ���ļ�·��
	// objFilename��obj�ļ�����������·����Ϣ
	// nHandSystem: ģ�ͼ�¼������������ϵ�µ����ݻ�����������ϵ�µ�����
	bool Load(ID3D11Device* device, char* resPath, char* objFilename, int nHandSystem = Left_Hand_System);
	// �ͷ���Դ
	void Release();
	// ����ģ��
	void RenderEffect(ID3D11DeviceContext* deviceContext, ID3DX11EffectTechnique* pTechnique);

	ID3DX11Effect* GetRenderEffect() { return effect; }

private:
	// ��ȡ������Ϣ��λ�á��������ߣ�
	bool ParseVertices(ID3D11Device* device, char* fileName, int nHandSystem);
	// ���������ļ�
	bool ParseMaterialFile(ID3D11Device* device, char* fileName);
	// ����������submesh���ṹ
	bool ParseSubset(char* fileName, int nHandSystem);
	// �������㻺�����������
	bool CreateBuffers(ID3D11Device* device);

private:

	// �ļ�����������Ϣ
	std::vector<XMFLOAT3>		m_vertices; //����
	std::vector<XMFLOAT2>		m_textures; //����
	std::vector<XMFLOAT3>		m_normals;  //������
	// ���񶥵���Ϣ
	std::vector<ObjVertex>		m_meshVertices;	
	// ������Ϣ
	std::vector<ObjSubMesh>		m_Mesh;
	// ������Ϣ
	std::vector<ObjMaterial>	m_Material;
	// OBJ��Դ·��
	std::string m_strResPath;
	
	// ���񶥵㻺��
	ID3D11Buffer				*m_meshVertexBuffer; 
	// ������������
	ID3D11Buffer				*m_meshIndexBuffer;
	// Effect����
	ID3DX11Effect* effect;

};

#endif