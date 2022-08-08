#include "framework.h"
#include "GameSystem.h"
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

	// 三角形の描画
	{
		// １頂点の形式(今回は座標だけ)
		struct VertexType
		{
			DirectX::XMFLOAT3 Pos;	// 座標
		};
		// 三角形を作るため、頂点を３つ作る
		VertexType v[3] = {
			{{0,0,0}},
			{{1,-1,0}},
			{{-1,-1,0}},
		};

		//-----------------------------
		// 頂点バッファ作成
		// ・上記で作った３つの頂点はそのままでは描画に使用できないんす…
		// ・ビデオメモリ側に「頂点バッファ」という形で作る必要があります！
		// ・今回は効率無視して、その場で作って、使って、すぐ捨てます。
		//-----------------------------
		// 作成するバッファの仕様を決める
		// ・今回は頂点バッファにするぞ！って感じの設定
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
		vbDesc.ByteWidth = sizeof(v);					// 作成するバッファのバイトサイズ
		vbDesc.MiscFlags = 0;							// その他のフラグ
		vbDesc.StructureByteStride = 0;					// 構造化バッファの場合、その構造体のサイズ
		vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
		vbDesc.CPUAccessFlags = 0;

		// 上の仕様を渡して頂点バッファを作ってもらう
		// もちろんデバイスさんにお願いする
		ComPtr<ID3D11Buffer> vb;
		D3D11_SUBRESOURCE_DATA initData = { &v[0], sizeof(v), 0 };	// 書き込むデータ
		// 頂点バッファの作成
		D3D.m_device->CreateBuffer(&vbDesc, &initData, &vb);

		//++++++++++++++++++++ ここから描画していきます ++++++++++++++++++++

		// ここからは「描画の職人」デバイスコンテキストくんが大活躍

		// 頂点バッファを描画で使えるようにセットする
		UINT stride = sizeof(VertexType);
		UINT offset = 0;
		D3D.m_deviceContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
		// プロミティブ・トポロジーをセット
		D3D.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//-----------------------------
		// シェーダーをセット
		//-----------------------------
		D3D.m_deviceContext->VSSetShader(D3D.m_spriteVS.Get(), 0, 0);
		D3D.m_deviceContext->PSSetShader(D3D.m_spritePS.Get(), 0, 0);
		D3D.m_deviceContext->IASetInputLayout(D3D.m_spriteInputLayout.Get());

		// こんな感じで、ひたすらデバイスコンテキストに情報を渡す

		//-----------------------------
		// 描画実行
		//-----------------------------
		// デバイスコンテキストくん、上記のセットした内容で描画してください、とお願いする
		D3D.m_deviceContext->Draw(3, 0);

	}

    // バックバッファの内容を画面に表示
    D3D.m_swapChain->Present(1, 0);
}
