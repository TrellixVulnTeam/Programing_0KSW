/*!
 * @brief	ボックス
 */

#ifndef _CBOXSHAPE_H_
#define _CBOXSHAPE_H_

#include "tkEngine/graphics/tkMaterial.h"
#include "tkEngine/shape/tkShapeVertex.h"

namespace tkEngine{
	/*!
	 * @brief	ボックス
	 */
	class CBoxShape{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CBoxShape();
		/*!
		 * @brief	デストラクタ。
		 */
		~CBoxShape();
		/*!
		 *@brief	カラーボックスを作成。
		 *@param[in]	size	サイズ。
		 *@param[in]	color	カラー。
		 */
		void Create( CVector3 size, u32 color );
		/*!
		 * @brief	リリース。
		 */
		void Release();
		/*!
		*@brief	更新
		*/
		void UpdateWorldMatrix();
		/*!
		*@brief	描画。
		*@param[in]	renderContext	レンダリングコンテキスト。
		*/
		void Render(CRenderContext& renderContext);
		/*!
		*@brief	ワールド行列の取得。
		*/
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	座標を設定。
		*/
		void SetPosition(const CVector3& position)
		{
			m_position = position;
		}
		/*!
		*@brief	回転を設定。
		*/
		void SetRotation(const CQuaternion& rotation)
		{
			m_rotation = rotation;
		}
		/*!
		*@brief	プリミティブを取得。
		*/
		CPrimitive* GetPrimitive()
		{
			return m_pPrimitive;
		}
	private:
		bool m_isCreatePrimitive;	//!<内部でプリミティブを作成したかどうかのフラグ。
		CPrimitive*	m_pPrimitive;	//!<プリミティブ。
		CVector3	m_position;		//!<座標。
		CQuaternion	m_rotation;		//!<回転。
		CMatrix		m_worldMatrix;	//!<ワールド行列。
	};
}

#endif // _CBOXSHAPE_H_