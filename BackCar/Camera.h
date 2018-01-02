#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "d3dUtility.h"

class Camera
{
	enum TransformFlag                    //�任��ʶ��
	{
		None =  0,                         
		Rotate = (1 << 0),      //<<Ϊλ�Ʋ�������������λ��0λ����0001
		Translate = (1 << 1)    //0001����λ��һλ�����0010��
	};
public:
	Camera();

	void Pitch(float angle);                //����
	void Yaw(float angle);                  //ƫת

	void SetEye(const XMVECTOR& position);  //�����ӵ�λ��

	const XMVECTOR& GetEye();          //��õ�ǰ�ӵ�λ��
	const XMVECTOR& GetAt();           //��õ�ǰ������Ĺ۲��
	const XMVECTOR& GetUp();           //��õ�ǰ�������������

	void MoveForwardBy(float value);  //��ǰ�ƶ��������valueȡ��ֵ������ƶ�
	void MoveRightBy(float value);    //�����ƶ��������valueȡ��ֵ��������ת
	void MoveUpBy(float value);       //�����ƶ��������valueȡ��ֵ�������ƶ�

	void Apply();                     //Ӧ�õ�ǰ�������ɹ۲�任����

	const XMFLOAT4X4& GetView();      //��ù۲�����ϵ�任����
private:
	UINT flag;
	float rotateX;       //��X�����ת�Ƕ�
	float rotateY;       //��Y�����ת�Ƕ�
	float rotateZ;       //��Z�����ת�Ƕ�

	XMVECTOR move;              //�ƶ�����
	XMFLOAT4X4 cameraRotation;  //�����ת����
	XMVECTOR eye;               //�ӵ�λ��
	XMVECTOR up;                //�����������
	XMVECTOR at;                //�۲�λ��
	XMFLOAT4X4 view;            //�۲����
};

#endif