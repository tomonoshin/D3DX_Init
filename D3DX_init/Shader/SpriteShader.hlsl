
// ���_�V�F�[�_�[����o�͂���f�[�^
struct VSOutput
{
    float4 Pos : SV_Position;   // ���_�̍��W(�ˉe���W�n)
};

//========================================
// ���_�V�F�[�_�[
//========================================
VSOutput VS(float4 pos : POSITION)
{
    VSOutput Out;
    // ���_���W���A�������H�������̂܂܏o��
    Out.Pos = pos;
    return Out;
}

//========================================
// �s�N�Z���V�F�[�_�[
//========================================
float4 PS(VSOutput In) : SV_Target0
{
    // �ΐF�ɂ���
    return float4(0, 1, 0, 1);
}
