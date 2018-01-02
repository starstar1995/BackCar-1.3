#include "ObjLoader.h"

//#pragma warning(disable : 4127)

//������Ĺ��캯��
ObjMaterial::ObjMaterial()
{
	memset( strName , 0 , sizeof(char) * 1024 );
	vAmbient = XMFLOAT4( 1.0f , 1.0f , 1.0f, 1.0f);
	vDiffuse = XMFLOAT4( 1.0f , 1.0f , 1.0f, 1.0f);
	vSpecular = XMFLOAT4( 1.0f , 1.0f , 1.0f, 1.0f);
	nShininess = 0;
	pTextureRV = NULL;
}
//��������ͷź�������Ҫ�ͷ��������
void ObjMaterial::Release()
{
	if(pTextureRV)
	{
		(pTextureRV)->Release();
		(pTextureRV)=NULL;
	}
}

//������Ĺ��캯��
ObjSubMesh::ObjSubMesh()
{
	pTextureRV = 0;
}

//��������ͷź�������Ҫ�ͷ��������
void ObjSubMesh::Release()
{
	if(pTextureRV)
	{
		pTextureRV->Release();
		pTextureRV = 0;
	}
}

//ObjLoader��Ĭ�Ϲ��캯��
ObjLoader::ObjLoader()
{
	m_meshVertexBuffer = 0;
	m_meshIndexBuffer = 0;
	effect = 0;
}

//ObjLoader�Ĺ��캯������ID3DX11Effect������Ϊ����
ObjLoader::ObjLoader(ID3DX11Effect* objeffect)
{
	m_meshVertexBuffer = 0;
	m_meshIndexBuffer = 0;
	effect = objeffect;
}

//��OBJ�ļ��ж���������Ϣ
bool ObjLoader::Load(ID3D11Device* device, char* resPath, char* objFilename, int nHandSystem)
{
	m_strResPath = resPath;

	std::string objFullFilename(resPath);  //ȡ���ļ�·��
	objFullFilename += objFilename;        //��·�����ļ����ϲ��������ļ���
	ParseVertices(device, (char*)objFullFilename.c_str(), nHandSystem); //�����ļ��еĶ�����Ϣ��������
	ParseSubset((char*)objFullFilename.c_str(), nHandSystem);           //�����Ӽ�
	CreateBuffers(device);                                              //�������㻺��

	return true;
}

//�����ļ��еĶ�����Ϣ��������
bool ObjLoader::ParseVertices(ID3D11Device* device, char* fileName, int nHandSystem)
{
	std::ifstream inFile;    //����һ���ļ����������
	inFile.open(fileName);   //���ļ�
	if(inFile.fail())        //�ж��ļ��Ƿ�򿪳ɹ�
	{
		return false;
	}

	while(true)              //ͨ��һ��ѭ����ȡ�ļ���ֱ���ļ��������˳�ѭ��
	{
		std::string input;   //����һ���ַ���
		inFile>>input;       //��ȥ�ļ��������һ�д��ݸ�input�ַ���
		if(input == "v")     //��ȡinput��ֵΪv���ʾ֮�������Ϊ��������
		{
			XMFLOAT3 pos;    //����һ��XMFLOAT3������ļ��еĶ�������
			inFile>>pos.x;   //����x������
			inFile>>pos.y;   //����y������
			inFile>>pos.z;   //����z������

			if(Right_Hand_System == nHandSystem) //�������������ϵ��Ҫ����z��ת��
			{
				pos.z = pos.z * -1.0f;
			}
			m_vertices.push_back( pos );  //�����������������m_vertices��Ա��
		}
		else if(input == "vt") //��ȡinput��ֵΪvt���ʾ֮�������Ϊ��������
		{
			XMFLOAT2 tc;
			inFile>>tc.x;      //��������x������
			inFile>>tc.y;      //��������y������

			if(Right_Hand_System == nHandSystem)
			{
				tc.y = 1.0f - tc.y;
			}
			m_textures.push_back( tc );  //����������������������m_textures��Ա��
		}
		else if(input == "vn")  //��ȡinput��ֵΪvn���ʾ֮�������Ϊ������
		{
			XMFLOAT3 nor;
			inFile>>nor.x;      
			inFile>>nor.y;
			inFile>>nor.z;

			if(Right_Hand_System == nHandSystem)
			{
				nor.z = nor.z * -1.0f;
			}
			m_normals.push_back( nor );  //���������ķ����������m_normals��Ա����
		}
		else if(input == "mtllib")       //��ȡinput��ֵΪmtllib���ʾ֮�������Ϊ�����ļ���
		{
			// ���������ļ�
			std::string matFileName;
			inFile>>matFileName;

			// �����ļ��м�¼�������ļ����ǲ�����·����Ϣ��
			ParseMaterialFile(device, (char*)matFileName.c_str());
		}
		/// usemtl��f��Ϣ��ParseSubset��������ȡ
		inFile.ignore( 1024 , '\n' );
		if( inFile.eof() )  //�����ļ��������˳�ѭ��
			break;
	}

	inFile.close();  //�ؼ��ļ�����������

	return true;
}

//��ȡ�����ļ��в�����Ϣ
bool ObjLoader::ParseMaterialFile(ID3D11Device* device, char* fileName)
{
	std::ifstream inFile;                                //����һ���ļ�����������
	std::string fullFileName = m_strResPath + fileName;  //��ȡ�ļ�ȫ��
	inFile.open(fullFileName);                           //���ļ�
	if(inFile.fail())                                    //�ж��ļ����Ƿ�ɹ�
	{
		return false;
	}

	ObjMaterial* pMat = NULL;         //����һ��ObjMaterial���ʶ���

	while(true)                       //ͨ��ѭ�����ж�ȡ�����ļ��е�����
	{
		std::string input;
		inFile>>input;
		if(input == "newmtl")        //�������input��ֵΪnewmtl�򴴽�һ���²�����Ϣ
		{
			if(pMat != NULL)         //���pMat�ﱾ���ʹ��ڲ�����Ϣ�����һ��mtl�ļ�������ֲ��ʵ������
			{
				m_Material.push_back(*pMat); //�򽫲�����Ϣ����m_Material��Ա
				pMat->Release();             
				delete pMat;
			}

			pMat = new ObjMaterial;  //�½�һ�����ʶ���

			inFile>>pMat->strName;   //��newmtl����ַ�������Ϊ��������
		}
		else if(pMat != NULL)        //������ʶ���pMat��Ϊ��
		{
			if(input == "Ka")        //�������input��ֵΪKa������ֵΪ���ʵĻ����ⷴ����
			{
				//���ò��ʵĻ����ⷴ����
				inFile>>pMat->vAmbient.x;
				inFile>>pMat->vAmbient.y;
				inFile>>pMat->vAmbient.z;
			}
			else if(input == "Kd")  //�������input��ֵΪKd������ֵΪ���ʵ�ɢ��ⷴ����
			{
				//���ò��ʵ�ɢ��ⷴ����
				inFile>>pMat->vDiffuse.x;
				inFile>>pMat->vDiffuse.y;
				inFile>>pMat->vDiffuse.z;
			}
			else if(input == "Ks")  //�������input��ֵΪKs������ֵΪ���ʵľ����ⷴ����
			{
				//���ò��ʵľ���ⷴ����
				inFile>>pMat->vSpecular.x;
				inFile>>pMat->vSpecular.y;
				inFile>>pMat->vSpecular.z;
			}
			else if(input == "Ns")  //�������input��ֵΪNs������ֵΪ���ʵľ����ⷴ����
			{
				inFile>>pMat->nShininess;  //���þ��淴��ϵ��
			}
			else if(input == "map_Kd")  //�������input��ֵΪmap_Kd������ֵΪ�����ļ���
			{
				// �����ж��ļ����Ƿ���ڣ���������ڣ���ô��һ���ַ�����'\n'
				char ch;
				inFile.get(ch);
				if(ch == ' ' || ch == '\t')
				{
					inFile.get(ch);
				}

				if(ch == '\n')               //�������û���ļ���
				{
					pMat->pTextureRV = NULL;//����������Ϊ��
					if( inFile.eof() )      //������ļ���β������ѭ��
						break;
					else                    //�������ѭ��
						continue;
				}

				std::string mapFileName;    //�����ַ�������ȡ�ļ���
				inFile>>mapFileName;
				// �˴���ҪΪ�����ļ��������·������ΪOBJ�ļ��ǲ�����·����Ϣ��
				mapFileName = ch + mapFileName;
				mapFileName = m_strResPath + mapFileName;
				//�������ļ��ж�ȡ�ļ�������ȡ����Ϣ������ʶ����pTextureRV��Ա
				HRESULT result = D3DX11CreateShaderResourceViewFromFileA(
					device, mapFileName.c_str(), NULL, NULL,  &pMat->pTextureRV, NULL);
				if(FAILED(result))
				{
					::MessageBoxA(NULL, "Create texture failed", "Notice", 0);
					return false;
				}
			}
		}
		inFile.ignore( 1024 , '\n' );

		if( inFile.eof() )  //�ļ�����������ѭ��
			break;
	}

	// ���pMat��Ҫ���һ�Σ��������һ��mtl�ļ�ֻ��һ��������Ϣ�����
	// �����ж��������Ϣ������������һ��������Ϣ����m_Material����
	if(pMat != NULL)
	{
		m_Material.push_back(*pMat);
	}

	return true;
}

//���ݶ���Ķ�����Ϣ�����Ӽ�
bool ObjLoader::ParseSubset(char* fileName,int nHandSystem)
{
	std::ifstream inFile;   //����һ���ļ�����������
	inFile.open(fileName);  //���ļ�
	if(inFile.fail())       //�жϴ��Ƿ�ɹ�
	{
		return false;
	}

	ObjSubMesh* pSubMesh = NULL;    //����һ��ObjSubMesh����
	unsigned long lStartIndex = 0;  //������ʼ
	unsigned long lIndexCount = 0;  //��������

	while(true)             
	{
		std::string input;
		inFile>>input;
		if(input == "usemtl")            //�������input��ֵΪusemtl������ֵΪ���ʵ�����
		{
			if(pSubMesh != NULL)         //���ObjSubMesh����Ϊ��
			{
				pSubMesh->lStartIndex = lStartIndex;              //����ObjSubMesh�����������ʼλ��
				pSubMesh->lIndexCount = lIndexCount - lStartIndex;//����ObjSubMesh�����������С

				m_Mesh.push_back(*pSubMesh);   //�����ObjSubMesh��������m_Mesh��Ա��
				delete pSubMesh;
			}

			pSubMesh = new ObjSubMesh;   //�½�һ��ObjSubMesh����

			inFile>>pSubMesh->strMaterialName;  //����ObjSubMesh����Ĳ�����
			// ���ݲ������ƻ�ȡ������ͼ����
			std::vector<ObjMaterial>::iterator iter = m_Material.begin(); //����һ��������
			while(iter != m_Material.end())   //��m_Material�����һ�����������һ��
			{
				if(iter->strName == pSubMesh->strMaterialName) break; //����Ƚϲ�������������ͬ���˳�
				iter++;
			}
			if(iter == m_Material.end())     //�������������ʼ�ͽ�β����ͬһλ�ã���˵��������Ϊ��
			{
				::MessageBoxA(NULL, "Material Error: can not find material", "Error", 0);
				return false;
			}
			else
				pSubMesh->pTextureRV = iter->pTextureRV;    //��������������ֵ��pSubMesh������

			lStartIndex = lIndexCount;                      //����������ʼλ��
		}
		else if(pSubMesh != NULL)  //�������input��ֵ����usemtl��pSubMesh��Ϊ�����ʾ
		{
			if(input == "f")      //�������input��ֵΪf��֮�������Ϊ���꣬����������������
			{
				ObjVertex vertex[3];  //�������
				int posIndex[3];      //λ������
				int texIndex[3];      //��������
				int normalIndex[3];   //����������

				char ch;
				if(Left_Hand_System == nHandSystem) //��������ϵ
				{	//���ζ����������ֵ			
					inFile >> posIndex[0] >> ch >> texIndex[0] >> ch >> normalIndex[0]
					>> posIndex[1] >> ch >> texIndex[1] >> ch >> normalIndex[1]
					>> posIndex[2] >> ch >> texIndex[2] >> ch >> normalIndex[2];
				}
				else    //��������ϵ
				{   //���ζ����������ֵ
					inFile >> posIndex[2] >> ch >> texIndex[2] >> ch >> normalIndex[2]
					>> posIndex[1] >> ch >> texIndex[1] >> ch >> normalIndex[1]
					>> posIndex[0] >> ch >> texIndex[0] >> ch >> normalIndex[0];
				}
				//ͨ������ֵ��������
				//��ν������������֮ǰ�����õ�m_vertices��m_textures��m_normals��Ӧ�±�
				for(int i = 0; i != 3; ++i)
				{
					vertex[i].vPosition = m_vertices[posIndex[i] - 1];  //λ��
					vertex[i].vTexCoord = m_textures[texIndex[i] - 1];  //����
					vertex[i].vNormal = m_normals[normalIndex[i] - 1];  //������

					// �ռ����ж���
					this->m_meshVertices.push_back(vertex[i]);

					lIndexCount++;
				}
			}
		}
		inFile.ignore( 1024 , '\n' );

		if( inFile.eof() )
			break;
	}
	// pSubMesh��Ҫ���һ��
	if(pSubMesh != NULL)
	{
		pSubMesh->lStartIndex = lStartIndex;
		pSubMesh->lIndexCount = lIndexCount - lStartIndex;
		m_Mesh.push_back(*pSubMesh);
	}

	return true;
}

//�������㻺��
bool ObjLoader::CreateBuffers(ID3D11Device* device)
{
	// ����mesh�������������
	ObjVertex* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc; //��������
	D3D11_SUBRESOURCE_DATA vertexData, indexData;        //����Դ����
	HRESULT result;

	int vertexCount = m_meshVertices.size();

	vertices = new ObjVertex[vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned int[vertexCount];
	if(!indices)
	{
		return false;
	}

	//������������
	for(int i = 0; i != vertexCount; ++i)
	{
		vertices[i].vPosition = m_meshVertices[i].vPosition;
		vertices[i].vTexCoord = m_meshVertices[i].vTexCoord;
		vertices[i].vNormal = m_meshVertices[i].vNormal;
		indices[i] = i;
	}

	//��䶥�㻺�������ṹ
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ObjVertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	//�������㻺��
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(this->m_meshVertexBuffer));
	if(FAILED(result))
	{
		return false;
	}
	//����������������ṹ
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	//������������
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(this->m_meshIndexBuffer));
	if(FAILED(result))
	{
		return false;
	}

	// buffers�����ɹ��Ժ�vertex��index����Ϳ���ɾ���ˣ���Ϊ�����Ѿ������Ƶ�buffers����
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

//����ģ��
void ObjLoader::RenderEffect(ID3D11DeviceContext* deviceContext, ID3DX11EffectTechnique* pTechnique)
{
	UINT stride = sizeof(ObjVertex);
	UINT offset = 0;
	bool textureOn;

	deviceContext->IASetVertexBuffers( 0, 1, &this->m_meshVertexBuffer, &stride, &offset );//���ö��㻺��
	deviceContext->IASetIndexBuffer( this->m_meshIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );  //������������
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);         //����ͼԪ����

	for(std::vector<ObjSubMesh>::iterator iter = m_Mesh.begin(); iter != m_Mesh.end(); ++iter)
	{
		// ��ȡ��������
		std::vector<ObjMaterial>::iterator iter2;
		//���ݲ��ʺ�����ͬ���������������
		for(iter2 = m_Material.begin(); iter2 != m_Material.end(); ++iter2)
		{
			if(iter2->strName == iter->strMaterialName)
			{
				if(iter2->pTextureRV != NULL)
				{
					::ID3DX11EffectShaderResourceVariable* fxShaderResource = 
						effect->GetVariableByName("Texture")->AsShaderResource();
					fxShaderResource->SetResource(iter2->pTextureRV);
					textureOn = true;
				}
				else
					textureOn = false;
				break;
			}
		}

		//���ò�����Ϣ
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(iter2->vAmbient));
	    effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(iter2->vDiffuse));
	    effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(iter2->vSpecular));

		D3DX11_TECHNIQUE_DESC techDesc;
		pTechnique->GetDesc( &techDesc );
		//����pass�ĸ���ѭ���������壬������fx�ļ���ֻ��ÿ��techniqueֻ��һ��pass����ֻѭ��һ��
		for(UINT i = 0; i < techDesc.Passes; ++i)
		{
			pTechnique->GetPassByIndex(i)->Apply(0,deviceContext);
			deviceContext->DrawIndexed( iter->lIndexCount, iter->lStartIndex, 0 );
		}
	}
}

//�ͷź���
void ObjLoader::Release()
{
	if(m_meshIndexBuffer)
	{
		m_meshIndexBuffer->Release();
		m_meshIndexBuffer = 0;
	}

	if(m_meshVertexBuffer)
	{
		m_meshVertexBuffer->Release();
		m_meshVertexBuffer = 0;
	}

	for(std::vector<ObjSubMesh>::iterator iter = m_Mesh.begin(); iter != m_Mesh.end(); ++iter)
	{
		iter->Release();
	}

	for(std::vector<ObjMaterial>::iterator iter = m_Material.begin(); iter != m_Material.end(); ++iter)
	{
		iter->Release();
	}
}

