
// �e�N�X�`��
Texture2D g_texture : register(t0);
// �T���v��
SamplerState g_sampler : register(s0);
 
// ���_�V�F�[�_�[����o�͂���f�[�^
struct VSOutput
{
    float4 Pos : SV_Position;   // ���_�̍��W(�ˉe���W�n)
    float2 UV : TEXCOORD0;      // UV���W
};
 
//========================================
// ���_�V�F�[�_�[
//========================================
VSOutput VS(float4 pos : POSITION,
            float2 uv : TEXUV)
{
    VSOutput Out;
    // ���_���W���A�������H�������̂܂܏o��
    Out.Pos = pos;
    // ���_��UV���W���A�������H�������̂܂܏o��
    Out.UV = uv;
    return Out;
}
 
//========================================
// �s�N�Z���V�F�[�_�[
//========================================
float4 PS(VSOutput In) : SV_Target0
{
    // �e�N�X�`������F���擾
    float4 texColor = g_texture.Sample(g_sampler, In.UV);
 
    // �ΐF�ɂ���
    return texColor;
}
