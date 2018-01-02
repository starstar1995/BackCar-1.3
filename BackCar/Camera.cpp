#include "Camera.h"

Camera::Camera()  //���캯������ʼ��Camera��ĳ�Ա����
{
    flag = Rotate | Translate;    //ָ����ʶ��
	rotateX = 0.0f;               //��X����ת�ĽǶ�
	rotateY= 0.0f;                //��Y����ת�ĽǶ�
	rotateZ= 0.0f;                //��Z����ת�ĽǶ�
	move = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);  //��ʼ���ƶ�λ��
	eye = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);   //��ʼ���ӵ�λ��
	//��ʼ���������ת����
	//������Ҫ��XMMATRIX����ת���XMStoreFloat4x4����
	XMMATRIX cameraRotationMATRIX = XMMatrixIdentity();
	XMStoreFloat4x4(&cameraRotation, cameraRotationMATRIX);
	//��ʼ������Ĺ۲����
	//ͬ����Ҫ��XMMATRIX����ת���XMStoreFloat4x4����
	XMMATRIX viewMATRIX = XMMatrixIdentity();
	XMStoreFloat4x4(&view, viewMATRIX);
}

//����
void Camera::Pitch( float angle )
{
	rotateX += angle;
	flag |= Rotate;
}

//ƫת
void Camera::Yaw( float angle )
{
	rotateY += angle;
	flag |= Rotate;
}

//�����ӵ�λ��
void Camera::SetEye( const XMVECTOR& position )
{
	eye = position;
	flag |= Translate;
}

//��õ�ǰ�ӵ�λ��
const XMVECTOR& Camera::GetEye()
{
	return eye;
}

//��õ�ǰ������Ĺ۲��
const XMVECTOR& Camera::GetAt()
{
	return at;
}

//��õ�ǰ�������������
const XMVECTOR& Camera::GetUp()
{
	return up;
}

//��ǰ�ƶ��������valueȡ��ֵ������ƶ�
void Camera::MoveForwardBy( float value )
{
	move = XMVectorSetZ(move, value);
	flag |= Translate;
}

//�����ƶ��������valueȡ��ֵ��������ת
void Camera::MoveRightBy( float value )
{
	move = XMVectorSetX(move, value);
	flag |= Translate;
}

//�����ƶ��������valueȡ��ֵ�������ƶ�
void Camera::MoveUpBy( float value )
{
	move = XMVectorSetY(move, value);
	flag |= Translate;
}

//Ӧ�õ�ǰ�������ɹ۲�任����
void Camera::Apply()
{
	XMMATRIX cameraRotationMATRIX;    //����XMMATRIXһ���������ھ���任
	if (flag != None)
	{
		
		if ((flag & Rotate) != 0)  //���������ת��ʶ
		{
			//��Rotateȡ�񣬼�1110���ٺ�flag����������
			//���flag=0011��������Ϊ0010
			flag &= ~Rotate; 
			//����������ϵ�Ͻ�����ת�任
			cameraRotationMATRIX = XMMatrixRotationX(rotateX) * XMMatrixRotationY(rotateY) * XMMatrixRotationZ(rotateZ);
			XMStoreFloat4x4(&cameraRotation, cameraRotationMATRIX); //�����ɵ�XMMATRIX��ŵ���ԱcameraRotation
		}
		
		if ((flag & Translate) != 0)//�������ƽ�Ʊ�ʶ
		{
			//��Translateȡ�񣬼�1110���ٺ�flag����������
			//���flag=0010��������Ϊ0000
			flag &= ~Translate;
			//�ӳ�ԱcameraRotation�����ת����
			cameraRotationMATRIX = XMLoadFloat4x4(&cameraRotation);
			//���¼����ӵ�λ��
			eye += XMVector4Transform(move, cameraRotationMATRIX);
			move = XMVectorZero(); //�����ƶ�����
		}
		//�ӳ�ԱcameraRotation�����ת����
		cameraRotationMATRIX = XMLoadFloat4x4(&cameraRotation);
		//���¼���۲��
		at = eye + XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), cameraRotationMATRIX);
		//���¼��������������
		up = XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), cameraRotationMATRIX);
		//���¼���۲����
		XMMATRIX viewMATRIX = XMMatrixLookAtLH(eye, at, up);
		//�������ɵĹ۲��������Աview��
		XMStoreFloat4x4(&view, viewMATRIX);
	}
}

//��ù۲�����ϵ�任����
const XMFLOAT4X4& Camera::GetView()
{
	return view;
}
