#pragma once

namespace Engine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual eint32 GetWidth() const = 0;
		virtual eint32 GetHeight() const = 0;
		//virtual eint32 GetID() const = 0;
		virtual CComPtr<ID3D11ShaderResourceView> GetTexture() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual eint32 GetWidth() const = 0;
		virtual eint32 GetHeight() const = 0;

		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};
}