#include"../MyLibrary/FbxShaderHeader.hlsli"
#include"../MyLibrary/ADSAHeader.hlsli"


Texture3D<float4> tex:register(t0);
Texture3D<float4> tex2:register(t1);
Texture3D<float4> maskTex:register(t2);
SamplerState smp:register(s0);

float4 GetTexColor(GSOutput input, Texture3D<float4> t)
{
	//���[���h�s��K��
	float start = mul(worldMat, tex3DStart);
	float end = mul(worldMat, tex3DEnd);
	//smoothstep�Ɣ͈͂ŁAw�����߂�
	float w = smoothstep(start, end, input.worldPos.z);

	return t.Sample(smp, float3(input.uv * 8, w)) + baseColor;

}

float4 GetShaderColor(GSOutput input)
{
	float4 shaderColor = m_ambient;
	const float shininess = 4.0f;//����x
	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	for (int i = 0; i < DIR_LIGTH_MAX; i++)
	{

		float3 lightDir = light[i];

		float3 dotlightnormal = dot(-lightDir, input.normal);
		float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

		float3 diffuse = saturate(dotlightnormal * m_diffuse);
		float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

		shaderColor.rgb += diffuse + specular * lightColor[i];
	}

	shaderColor.a = m_alpha;


	return shaderColor;
}

float4 BlendAddSubMulColor(float4 texColor, float4 shaderColor)
{
	float alphaSum = (shaderColor.a + texColor.a) / 2;

	float3 changeColor = texColor.rgb;
	changeColor += addColor.rgb - subColor.rgb;
	changeColor *= mulColor.rgb;

	float changeAlpha = alphaSum;
	changeAlpha += addColor.a - subColor.a;
	changeAlpha *= mulColor.a;

	float4 sumColor = float4(shaderColor * changeColor, changeAlpha);

	return sumColor;
}

float GetMaskColor(GSOutput input, Texture3D<float4> t) 
{
	//���[���h�s��K��
	float start = mul(worldMat, tex3DStart);
	float end = mul(worldMat, tex3DEnd);
	//smoothstep�Ɣ͈͂ŁAw�����߂�
	float w = smoothstep(start, end, input.worldPos.z);

	// a�����^�[������
	return t.Sample(smp, float3(input.uv, w)).a;
}

float4 main(GSOutput input) : SV_TARGET
{
	// �e�N�X�`���̐F�擾
	float4 texColor = GetTexColor(input,tex);
	float4 texColor2 = GetTexColor(input,tex2);
	
	// �}�X�N�擾
	float mask = GetMaskColor(input, maskTex);
	float4 maskMulColor2 = float4(texColor2.rgb * mask, texColor2.a);
	float4 maskMulColor = float4(texColor.rgb * (1 - mask), texColor.a);

	float4 blendColor = (maskMulColor + maskMulColor2) / 2;

	float4 shaderColor = GetShaderColor(input);
	float4 sumColor = BlendAddSubMulColor(blendColor, shaderColor);

	blendColor.rgb *= 1.7f;

	return blendColor;
}