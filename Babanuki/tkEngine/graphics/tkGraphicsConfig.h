/*!
 * @brief	グラフィックスコンフィグ。
 */

#ifndef _TKGRAPHICSCONFIG_H_
#define _TKGRAPHICSCONFIG_H_

namespace tkEngine{
	/*!
	 * @brief	エッジ抽出処理のコンフィグ
	 */
	struct SEdgeRenderConfig{
		bool isEnable;		//!<エッジ抽出処理が有効かどうかのフラグ。
		int	idMapWidth;		//!<IDマップの幅。
		int idMapHeight;	//!<IDマップの高さ。
	};
	
	/*!
	 * @brief	影処理のコンフィグ。
	 */
	struct SShadowRenderConfig{
		void Init()
		{
			isEnable = false;
			shadowMapWidth = 512;
			shadowMapHeight = 512;
			isDisableSoftShadow = false;
			numShadowMap = 1;
		}
		bool 	isEnable;				//!<影を落とす処理が有効かどうかのフラグ。
		int		shadowMapWidth;			//!<シャドウマップの幅。
		int		shadowMapHeight;		//!<シャドウマップの高さ。
		bool	isDisableSoftShadow;	//!<ソフトシャドウが無効かどうかのフラグ。
		int		numShadowMap;			//!<シャドウマップの枚数。(最大4枚)
	};
	/*!
	* @brief	Bloomのコンフィグ
	*/
	struct SBloomConfig {
		bool	isEnable;		//!<ブルームが有効かどうかのフラグ。
	};
	/*!
	* @brief	リフレクションマップのコンフィグ
	*/
	struct SReflectionMapConfig {
		bool	isEnable;				//!<リフレクションマップが有効かどうかのフラグ。
		int		reflectionMapWidth;		//!<リフレクションマップの幅。
		int		reflectionMapHeight;	//!<リフレクションマップの高さ。
	};
	/*!
	* @brief	被写界深度のコンフィグ。
	*/
	struct SDofConfig {
		bool isEnable;					//被写界深度が有効かどうかのフラグ。
	};
	/*!
	* @brief	アンチエイリアスのコンフィグ。
	*/
	struct SAAConfig {
		bool isEnable;					//アンチエイリアスが有効かどうかのフラグ。
	};
	/*!
	* @brief	モーションブラーのコンフィグ。
	*/
	struct SMotionBlurConfig {
		bool isEnable;
	};
	
	/*!
	 * @brief	グラフィックスコンフィグ。
	 */
	struct SGraphicsConfig{
		SEdgeRenderConfig		edgeRenderConfig;		//!<エッジ抽出処理のコンフィグ。
		SShadowRenderConfig		shadowRenderConfig;		//!<影の処理のコンフィグ。
		SBloomConfig			bloomConfig;			//!<Bloomのコンフィグ。
		SReflectionMapConfig	reflectionMapConfig;	//!<リフレクションマップのコンフィグ。
		SDofConfig				dofConfig;				//!<被写界深度のコンフィグ。
		SAAConfig				aaConfig;				//!<アンチエイリアスのコンフィグ。
		SMotionBlurConfig		motionBlurConfig;		//<!モーションブラーのコンフィグ。
		
	};
}

#endif //_TKGRAPHICSCONFIG_H_