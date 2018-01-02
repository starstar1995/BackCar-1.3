//����任����ĳ�������
cbuffer MatrixBuffer
{
	matrix World;             //����任����
	matrix View;              //�۲�任����
	matrix Projection;        //ͶӰ�任����
};

Texture2D Texture;  //�������

SamplerState Sampler  //���������
{
	Filter = MIN_MAG_MIP_LINEAR;  //�������Թ���
	AddressU = WRAP;  //ѰַģʽΪWrap
	AddressV = WRAP;  //ѰַģʽΪWrap
};

//������ɫ��������ṹ
struct VS_INPUT
{
	float4 Pos:POSITION;  //��������
	float2 Tex:TEXCOORD;  //��������
};

//������ɫ��������ṹ
struct VS_OUTPUT
{
	float4 Pos:SV_POSITION;  //λ��
	float2 Tex:TEXCOORD;  //��������
};

//������ɫ��
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);           //��������任��mulΪ����˷��ĺ���
	output.Pos = mul(output.Pos, View);       //�۲�����任��mul��һ������Ϊ����
	output.Pos = mul(output.Pos, Projection);   //ͶӰ����任

	output.Tex = input.Tex;  //��������
	return output;

}
//������ɫ��������ṹ
float4 PS(VS_OUTPUT input) :SV_Target
{
	return Texture.Sample(Sampler, input.Tex);  //��������
}

//����Technique
//////////////////////////////////////////////////////////////////////////
// Technique����ͬ��ɫ����������ʵ��ĳ�ֹ���
// ����Technique��technique11�ؼ��ֽ���
//////////////////////////////////////////////////////////////////////////
technique11 TexTech
{
	//Techniqueͨ��Pass��Ӧ�ò�ͬ��Ч����ÿ��Technique�����ж��Pass
	//������ֻ��һ��Pass
	pass P0
	{
		//���ö�����ɫ��
		//CompileShader���������汾��һ����Ŀ����ɫ��������ʹ�õ���SM5.0
		//��һ������Ϊ�����Լ���һ����ɫ������
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS())); //����������ɫ��
	}
}
