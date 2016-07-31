/*!
*@brief	スキンモデル
*/
#ifndef _TKSKINMODEL_H_
#define _TKSKINMODEL_H_

#include "tkEngine/graphics/tkAnimation.h"

namespace tkEngine {
	class CEffect;
	class CSkinModelData;
	class CRenderContext;
	class CLight;
	/*!
	*@brief	スキンモデル
	*/
	class CSkinModel {
	public:
		CSkinModel();
		~CSkinModel();
		/*!
		*@brief	初期化
		*/
		void Init(CSkinModelData* modelData);
		/*!
		*@brief	描画
		*/
		void Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		*@brief	イミディエイト描画。
		*@details
		* この関数を実行すると即描画コマンドがGPUに送られます。
		*/
		void ImmidiateDraw(LPDIRECT3DDEVICE9 pd3ddevice, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix);
		
		/*!
		*@brief	ワールド行列を更新。
		*@details
		* 後でCMotionクラスに移動させます。
		*@param[in]		trans	平行移動。
		*@param[in]		rot		回転。
		*@param[in]		scale	拡大。
		*/
		void UpdateWorldMatrix( const CVector3& trans, const CQuaternion& rot, const CVector3& scale );
		/*!
		*@brief	ライトを設定。
		*/
		void SetLight(CLight* light)
		{
			m_light = light;
		}
		/*!
		*@brief	法線マップを設定。
		*@param[in] normalMap
		*/
		void SetNormalMap(CTexture* normalMap)
		{
			m_normalMap = normalMap;
		}
	private:
		void DrawMeshContainer(
			IDirect3DDevice9* pd3dDevice,
			LPD3DXMESHCONTAINER pMeshContainerBase,
			LPD3DXFRAME pFrameBase,
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix,
			D3DXMATRIX* projMatrix,
			CLight* light,
			CTexture* normalMap
		);
		void DrawFrame(
			IDirect3DDevice9* pd3dDevice,
			LPD3DXFRAME pFrame,
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix,
			D3DXMATRIX* projMatrix
		);
	private:
		CMatrix				m_worldMatrix;		//!<ワールド行列。
		CMatrix				m_rotationMatrix;	//!<回転行列。
		CSkinModelData*		m_skinModelData;	//!<スキンモデルデータ。
		CEffect*			m_pEffect;			//!<エフェクト。
		CAnimation			m_animation;		//!<アニメーション。
		CLight*				m_light;			//!<ライト。
		CTexture*           m_normalMap;		//!<法線マップ。
		static const int MAX_MATRIX_PALLET = 128;	//!<マトリクスパレットの最大数。
		D3DXMATRIX			m_boneMatrixPallet[MAX_MATRIX_PALLET];	//!<マトリクスパレット。
	};
}

#endif // #ifndef _TKSKINMODEL_H_