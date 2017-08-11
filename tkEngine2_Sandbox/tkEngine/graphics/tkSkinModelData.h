/*!
 *@brief	スキンモデルデータ。
 */

#pragma once

#include "tkEngine/graphics/tkSkeleton.h"

namespace tkEngine{
	class CAnimationClip;
	typedef std::unique_ptr<CAnimationClip>	CAnimationClipPtr;
	class CModelEffect : public DirectX::IEffect {
	public:
		CShader m_vsShader;
		CShader m_psShader;
		ID3D11ShaderResourceView* diffuseTex = nullptr;
		bool isSkining;
	public:
		void __cdecl Apply(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);
			deviceContext->PSSetShaderResources(0, 1, &diffuseTex);

		}

		void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
		{
			*pShaderByteCode = m_vsShader.GetByteCode();
			*pByteCodeLength = m_vsShader.GetByteCodeSize();
		}
		void SetDiffuseTexture(ID3D11ShaderResourceView* tex)
		{
			diffuseTex = tex;
		}
	};
	/*!
	*@brief
	*  スキンなしモデルエフェクト。
	*/
	class CNonSkinModelEffect : public CModelEffect {
	public:
		CNonSkinModelEffect()
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMain", CShader::EnType::VS);
			m_psShader.Load("Assets/shader/model.fx", "PSMain", CShader::EnType::PS);
			isSkining = false;
		}
	};
	/*!
	*@brief
	*  スキンモデルエフェクト。
	*/
	class CSkinModelEffect : public CModelEffect {
	public:
		CSkinModelEffect()
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", CShader::EnType::VS);
			m_psShader.Load("Assets/shader/model.fx", "PSMain", CShader::EnType::PS);
			isSkining = true;
		}
	};

	/*!
	 *@brief	スキンモデルデータ。
	 */
	class CSkinModelData : Noncopyable{
	public:
		
		/*!
		 *@brief	コンストラクタ。
		 */
		CSkinModelData();
		/*!
		 *@brief	デストラクタ。
		 */
		~CSkinModelData();
		/*!
		 *@brief	ロード。
		 *@param[in]	filePath	ファイルパス。
		 *@return	trueが返ってきたらロード成功。
		 */
		bool Load( const wchar_t* filePath );
		DirectX::Model& GetBody()
		{
			return *m_modelDx;
		}
		/*!
		 *@brief	スケルトンを取得。
		 */
		CSkeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		 *@brief	アニメーションクリップのリストを取得。
		 */
		std::vector<CAnimationClipPtr>& GetAnimationClips()
		{
			return m_animationClips;
		}
	private:
		
		std::unique_ptr<DirectX::Model> m_modelDx;
		std::vector<CAnimationClipPtr>	m_animationClips;	//!<アニメーションクリップ
		CSkeleton	m_skeleton;
	};
}