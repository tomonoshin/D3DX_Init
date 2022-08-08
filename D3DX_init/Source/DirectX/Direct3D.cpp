#include "framework.h"
#include "Direct3D.h"
#include <windows.h>

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
	//=====================================================
	// �t�@�N�g���[�쐬(�r�f�I �O���t�B�b�N�̐ݒ�̗񋓂�w��Ɏg�p�����I�u�W�F�N�g)
	//=====================================================
	ComPtr<IDXGIFactory> factory;

	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
	{
		return false;
	}

	//=====================================================
	//�f�o�C�X����(��Ƀ��\�[�X�쐬���Ɏg�p����I�u�W�F�N�g)
	//=====================================================
	UINT creationFlags = 0;

#ifdef _DEBUG
	// DEBUG�r���h����Direct3D�̃f�o�b�O��L���ɂ���
	// (�������d�����ׂ����G���[���킩��)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};

	// �f�o�C�X�ƂŃf�o�C�X�R���e�L�X�g���쐬
	D3D_FEATURE_LEVEL futureLevel;
	if (FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_device,
		&futureLevel,
		&m_deviceContext)))
	{
		return false;
	}

	//=====================================================
	// �X���b�v�`�F�C���쐬(�t�����g�o�b�t�@�ɕ\���\�ȃo�b�N�o�b�t�@��������)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC scDesc = {};						// �X���b�v�`�F�[���̐ݒ�f�[�^
	scDesc.BufferDesc.Width = width;						// ��ʂ̕�
	scDesc.BufferDesc.Height = height;						// ��ʂ̍���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�t�@�̌`��
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.SampleDesc.Count = 1;							// MSAA�͎g�p���Ȃ�
	scDesc.SampleDesc.Quality = 0;							// MSAA�͎g�p���Ȃ�
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g�p���@
	scDesc.BufferCount = 2;									// �o�b�t�@�̐�
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;									// �E�B���h�E���[�h(TRUE:�E�C���h�E, FALSE:�t���X�N���[��)
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�C���̍쐬
	if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
	{
		return false;
	}

	// �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
	{
		return false;
	}

	// �o�b�N�o�b�t�@���\�[�X�p��RTV���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = scDesc.BufferDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
	{
		return false;
	}

	//=====================================================
	// �f�o�C�X�R���e�L�X�g�ɕ`��Ɋւ���ݒ���s���Ă���
	//=====================================================

	// �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
	m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);

	//=====================================================
	// �V�F�[�_�[�̍쐬
	//=====================================================
	// ���_�V�F�[�_�[��ǂݍ��݁��R���p�C��
	ComPtr<ID3DBlob> compiledVS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, nullptr)))
	{
		return false;
	}
	// �s�N�Z���V�F�[�_�[��ǂݍ��݁��R���p�C��
	ComPtr<ID3DBlob> compiledPS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr)))
	{
		return false;
	}

	// ���_�V�F�[�_�[�쐬
	if (FAILED(m_device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &m_spriteVS)))
	{
		return false;
	}
	// �s�N�Z���V�F�[�_�[�쐬
	if (FAILED(m_device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &m_spritePS)))
	{
		return false;
	}

	// �P���_�̏ڍׂȏ��
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXUV",	  0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ���_�C���v�b�g���C�A�E�g�쐬
	if (FAILED(m_device->CreateInputLayout(&layout[0], layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &m_spriteInputLayout)))
	{
		return false;
	}

	return true;
}

void Direct3D::ChangeMode_2D()
{
	// 2D�p���_�V�F�[�_�[�Z�b�g
	m_deviceContext->VSSetShader(m_spriteVS.Get(), 0, 0);
	// 2D�p�s�N�Z���V�F�[�_�[�Z�b�g
	m_deviceContext->PSSetShader(m_spritePS.Get(), 0, 0);
	// ���̓��C�A�E�g�Z�b�g
	m_deviceContext->IASetInputLayout(m_spriteInputLayout.Get());

	// �l�p�`�p ���_�o�b�t�@�쐬(����̂�)
	if (m_vbSquare == nullptr)
	{
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
		vbDesc.ByteWidth = sizeof(VertexType2D) * 4;	// �쐬����o�b�t�@�̃o�C�g�T�C�Y
		vbDesc.MiscFlags = 0;							// ���̑��̃t���O
		vbDesc.StructureByteStride = 0;					// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y

		vbDesc.Usage = D3D11_USAGE_DYNAMIC;				// �쐬����o�b�t�@�̎g�p�@
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_device->CreateBuffer(&vbDesc, nullptr, &m_vbSquare);
	}

	// ���_�o�b�t�@��`��Ŏg����悤�ɃZ�b�g����
	UINT stride = sizeof(VertexType2D);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vbSquare.GetAddressOf(), &stride, &offset);
	// �v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �T���v���[�X�e�[�g���쐬���Z�b�g����
	{
		// �ٕ����t�B���^�����O��ԁAWrap���[�h
		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���`�t�B���^�����O
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// �X�e�[�g�I�u�W�F�N�g�쐬
		ComPtr<ID3D11SamplerState> state;
		m_device->CreateSamplerState(&desc, &state);

		// �e�V�F�[�_�[��0�ԖڂɃZ�b�g(���ۂ͕K�v�ȃV�F�[�_�[�����Z�b�g���Ă�������)
		m_deviceContext->VSSetSamplers(0, 1, state.GetAddressOf()); // ���_�V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->PSSetSamplers(0, 1, state.GetAddressOf()); // �s�N�Z���V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->GSSetSamplers(0, 1, state.GetAddressOf()); // �W�I���g���V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->CSSetSamplers(0, 1, state.GetAddressOf()); // �R���s���[�g�V�F�[�_�[��0�ԖڂɃZ�b�g
	}
}

void Direct3D::Draw2D(const Texture& tex, float x, float y, float w, float h)
{
	float hW = w * 0.5f;
	float hH = h * 0.5f;

	// ���_�f�[�^�쐬
	VertexType2D v[4] = {
		{{x - hW, y - hH, 0}, {0, 1}},	// ����
		{{x - hW, y + hH, 0}, {0, 0}},	// ����
		{{x + hW, y - hH, 0}, {1, 1}},	// �E��
		{{x + hW, y + hH, 0}, {1, 0}},	// �E��
	};

	// ���_�o�b�t�@�Ƀf�[�^����������
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_deviceContext->Map(m_vbSquare.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// �f�[�^�R�s�[
		memcpy_s(pData.pData, sizeof(v), &v[0], sizeof(v));

		m_deviceContext->Unmap(m_vbSquare.Get(), 0);
	}

	// �e�N�X�`�����A�X���b�g0�ɃZ�b�g
	m_deviceContext->PSSetShaderResources(0, 1, tex.m_srv.GetAddressOf());

	// �f�o�C�X�R���e�L�X�g����A��L�̃Z�b�g�������e�ŕ`�悵�Ă�������
	m_deviceContext->Draw(4, 0);
}
