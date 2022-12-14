#include "framework.h"
#include "Source/DirectX/DirectX.h"

// GameSystemクラスを使えるようにする
#include "GameSystem.h"

// ゲームの初期設定を行う
void GameSystem::Initialize()
{
	// 画像の読み込み
	m_tex.Load("Data/Logo.png");
}

// このゲームの時間を進める(処理を実行する)
void GameSystem::Execute()
{
	// 画面を青色で塗りつぶす
	float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	// 三角形の描画
	{
		// 2Dモードに切り替える
		D3D.ChangeMode_2D();

		// 2D描画
		D3D.Draw2D(m_tex, 0, 0, 1, 1);
	}

	// バックバッファの内容を画面に表示
	D3D.m_swapChain->Present(1, 0);
}
