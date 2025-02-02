/*!
 *@brief	スプライト表示クラス。
 */

#pragma once

namespace tkEngine {
	/*!
	 *@brief	スプライト表示クラス。
	 */
	class CSprite {
	public:
		CSprite();
		~CSprite();
		/*!
		 *@brief	初期化。
		 */
		void Init(CTexture* texture);
		/*!
		*@brief	テクスチャを設定。
		*/
		void SetTexture(CTexture* texture)
		{
			m_texture = texture;
		}
		/*!
		*@brief	画像の位置を設定。
		*@details
		* 画面の中心が0,0となります。
		* 座標系は上方向が+y、右方向が+xになります。
		* UnityのuGUIに準拠。
		*
		*@param[in]		pos		座標。
		*/
		void SetPosition(const CVector2& pos)
		{
			m_pos = pos;
		}
		/*!
		*@brief	画像の位置を取得。
		*/
		const CVector2& GetPosition() const
		{
			return m_pos;
		}
		/*!
		*@brief	スプライトのサイズを設定。
		*@param[in]	size		サイズ。
		*/
		void SetSize(const CVector2& size)
		{
			m_size = size;
		}
		/*!
		*@brief	スプライトのサイズを取得。
		*/
		const CVector2& GetSize() const
		{
			return m_size;
		}
		/*!
		*@brief	ピボットを設定。
		*@details
		* 0.5, 0.5で画像の中心が基点。
		* 0.0, 0.0で画像の左下。
		* 1.0, 1.0で画像の右上。
		* UnityのuGUIに準拠。
		*@param[in]	pivot		サイズ。
		*/
		void SetPivot(const CVector2& pivot)
		{
			m_pivot = pivot;
		}
		void SetRotation( CQuaternion rot)
		{
			m_rotation = rot;
		}
		/*!
		*@brief	描画するUV座標の矩形を設定。
		*@param[in]		uvRect		xはuの左座標、yはvの上座標、zはuの右座標、wはvの下座標。
		*/
		void SetUVRect(const CVector4& uvRect)
		{
			m_uvRect = uvRect;
		}
		/*!
		 *@brief	描画。
		 *@param[in]	renderContext	レンダリングコンテキスト。
		 */
		void Draw(CRenderContext& renderContext);
		/*!
		*@brief	不透明度を設定。
		*@param[in]	alpha		不透明度。0.0で完全に透明。
		*/
		void SetAlpha(float alpha)
		{
			m_alpha = alpha;
		}
		/*!
		*@brief	不透明度を取得。
		*/
		float GetAlpha() const
		{
			return m_alpha;
		}
	private:
		CEffect*		m_effect = nullptr;		//!<エフェクト。
		CPrimitive		m_primitive;			//!<プリミティブ。
		CTexture*		m_texture = nullptr;	//<!テクスチャ。
		CVector2		m_pos = CVector2(0.0f, 0.0f);	//!<座標。
		CVector2		m_size = CVector2(0.0f, 0.0f);	//!<スプライトのサイズ。
		CVector2		m_pivot = CVector2(0.5f, 0.5f);	//!<ピボット
		CVector4		m_uvRect = CVector4(0.0f, 0.0f, 1.0f, 1.0f);	//!<UV座標。
		CQuaternion		m_rotation = CQuaternion::Identity;
		float			m_alpha = 1.0f;			//!<不透明度。
	};
}