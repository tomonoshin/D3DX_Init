
// GameSystem�N���X���g����悤�ɂ���
#include "GameSystem.h"
// Direct3D�N���X���g����悤�ɂ���
#include "Source/DirectX/Direct3D.h"

// �Q�[���̏����ݒ���s��
void GameSystem::Initialize()
{

}

// ���̃Q�[���̎��Ԃ�i�߂�(���������s����)
void GameSystem::Execute()
{
    // ��ʂ�F�œh��Ԃ�
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

    // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
    D3D.m_swapChain->Present(1, 0);
}
