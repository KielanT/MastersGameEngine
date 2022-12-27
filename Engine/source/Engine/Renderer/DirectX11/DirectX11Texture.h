#pragma once
#include "Engine/Interfaces/ITexture.h"


namespace Engine
{
	class DirectX11Texture : public Texture2D
	{
	public:
		DirectX11Texture(const std::string& path);
		virtual ~DirectX11Texture();

		virtual eint32 GetWidth() const override { return m_Width; }
		virtual eint32 GetHeight() const override { return m_Height; }
		//virtual eint32 GetID() const override { return m_ID; }
		virtual CComPtr<ID3D11ShaderResourceView> GetTexture() const override { return m_SRV; }

	private:
		eint32 m_Width, m_Height;
		eint32 m_ID;
		std::string m_Path;

		CComPtr<ID3D11ShaderResourceView> m_SRV;
	};
}

