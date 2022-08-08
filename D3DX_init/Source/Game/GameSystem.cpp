#include "framework.h"
#include "Source/DirectX/DirectX.h"

// GameSystem�N���X���g����悤�ɂ���
#include "GameSystem.h"

// �Q�[���̏����ݒ���s��
void GameSystem::Initialize()
{
	// �摜�̓ǂݍ���
	m_tex.Load("Data/Logo.png");
}

// ���̃Q�[���̎��Ԃ�i�߂�(���������s����)
void GameSystem::Execute()
{
	// ��ʂ�F�œh��Ԃ�
	float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	// �O�p�`�̕`��
	{
		// 2D���[�h�ɐ؂�ւ���
		D3D.ChangeMode_2D();

		// 2D�`��
		D3D.Draw2D(m_tex, 0, 0, 1, 1);
	}

	// �o�b�N�o�b�t�@�̓��e����ʂɕ\��
	D3D.m_swapChain->Present(1, 0);
}
