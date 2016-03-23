/*!
 * @brief	カメラ
 */

#ifndef _TKCAMERA_H_
#define _TKCAMERA_H_

namespace tkEngine{
	class CCamera{
	public:
		CCamera();
		~CCamera();
		/*!
		 * @brief	ビュー行列、プロジェクション行列、ビュープロジェクション行列を更新。
		 */
		void Update();
		/*!
		 * @brief	カメラの座標を設定。
		 *@param[in]	pos		カメラの座標。
		 */
		void SetPosition( const CVector3& pos ) 
		{
			m_position = pos;
		}
		/*!
		 * @brief	カメラの座標を取得
		 *@return	カメラの座標。
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	注視点を設定。
		 *@param[in]	target		注視点。
		 */
		void SetTarget( const CVector3& target )
		{
			m_target = target;
		}
		/*!
		 * @brief	注視点を取得。
		 *@return	注視点。
		 */
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/*!
		 * @brief	カメラの上方向を設定。
		 */
		void SetUp( const CVector3& up )
		{
			m_up = up
		}
		/*!
		 * @brief	カメラの上方向を取得
		 */
		const CVector3& GetUp() const
		{
			return m_up;
		}
		/*!
		 * @brief	ビュー行列を取得。
		 */
		const CMatrix& GetViewMatrix() const
		{
			return m_viewMatrix;
		}
		/*!
		 * @brief	プロジェクション行列を取得。
		 */
		const CMatrix& GetProjectionMatrix() const
		{
			return m_projectionMatrix();
		}
		/*!
		 * @brief	ビュープロジェクション行列を取得。
		 */
		const CMatrix& GetViewProjectionMatrix() const
		{
			return m_viewProjectionMatrix;
		}
		/*!
		 * @brief	遠平面を設定。
		 */
		void SetFar( f32 far )
		{
			m_far = far;
		}
		/*!
		 * @brief	近平面を設定。
		 */
		void SetNear( f32 near )
		{
			m_near = near;
		}
		/*!
		 * @brief	遠平面を取得。
		 */
		f32 GetFar() const
		{
			return m_far;
		}
		/*!
		 * @brief	近平面を取得。
		 */
		f32 GetNear() const
		{
			return m_near;
		}
	private:
		CVector3	m_position;				//!<カメラ位置。
		CVector3	m_up;					//!<カメラの上方向。
		CVector3	m_target;				//!<カメラの中止点。
		CMatrix		m_viewMatrix;			//!<ビュー行列。
		CMatrix		m_projectionMatrix;		//!<プロジェクション行列。
		CMatrix		m_viewProjectionMatrix;	//!<ビュープロジェクション行列。
		CMatrix		m_viewMatrixInv;		//!<ビュー行列の逆行列。
		f32			m_near;					//!<近平面。
		f32			m_far;					//!<遠平面。
		f32			m_viewAngle;			//!<画角(Y方向)。
		f32			m_aspect;				//!<アスペクト比。
		
	};
}
#endif //_TKCAMERA_H_