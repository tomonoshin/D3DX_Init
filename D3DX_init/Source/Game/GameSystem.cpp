
// GameSystemクラスを使えるようにする
#include "GameSystem.h"
// Direct3Dクラスを使えるようにする
#include "Source/DirectX/Direct3D.h"

// ゲームの初期設定を行う
void GameSystem::Initialize()
{

}

// このゲームの時間を進める(処理を実行する)
void GameSystem::Execute()
{
    // 画面を青色で塗りつぶす
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

    // バックバッファの内容を画面に表示
    D3D.m_swapChain->Present(1, 0);
}
