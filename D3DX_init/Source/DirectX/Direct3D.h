#pragma once

// Direct3Dのライブラリを使用できるようにする
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Direct3Dの型・クラス・関数などを呼べるようにする
#include <d3d11.h>

// DirectXMath(数学ライブラリ)を使用できるようにする
#include <DirectXMath.h>

// ComPtrを使用できるようにする
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

//=========================================
// Direct3Dクラス
//=========================================
class Direct3D
{
public:

	// ※変数は今回は全てpublicにしますが、本来はprivateで隠すべき※

	// Direct3Dデバイス
	ComPtr<ID3D11Device>		m_device;
	// Direct3Dデバイスコンテキスト
	ComPtr<ID3D11DeviceContext>	m_deviceContext;
	// スワップチェイン
	ComPtr<IDXGISwapChain>		m_swapChain;
	// バックバッファーのRTビュー
	ComPtr<ID3D11RenderTargetView> m_backBufferView;

	//--------------------------------------------
	// Direct3Dを初期化し、使用できるようにする関数
	// hWnd		: ウィンドウハンドル
	// width	: 画面の幅
	// height	: 画面の高さ
	//--------------------------------------------
	bool Initialize(HWND hWnd, int width, int height);



	//=========================================
	// 今回このクラスは、どこからでもアクセスできるように
	// シングルトンパターンにします
	// ↓↓↓以下、シングルトンパターンのコード
	//=========================================
private:
	// 唯一のインスタンス用のポインタ
	static inline Direct3D* s_instance;
	// コンストラクタはprivateにする
	Direct3D() {}
public:
	// インスタンス作成
	static void CreateInstance()
	{
		DeleteInstance();

		s_instance = new Direct3D();
	}
	// インスタンス削除
	static void DeleteInstance()
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}
	// 唯一のインスタンスを取得
	static Direct3D& GetInstance()
	{
		return *s_instance;
	}
};

// Direct3Dの唯一のインスタンスを簡単に取得するためのマクロ
#define D3D Direct3D::GetInstance()
