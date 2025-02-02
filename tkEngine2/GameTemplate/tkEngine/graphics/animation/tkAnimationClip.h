/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	キーフレーム。
	 */
	struct Keyframe {
		unsigned int boneIndex;		//!<ボーンインデックス。
		float time;					//!<時間。
		CMatrix transform;			//!<トランスフォーム。
	};
	/*!
	*@brief	キーフレーム。
	*/
	struct KeyframeRow {
		unsigned int boneIndex;		//!<ボーンインデックス。
		float time;					//!<時間。
		CVector3 transform[4];		//!<トランスフォーム。
	};
	/*!
	 *@brief	アニメーションクリップ。
	 */
	class CAnimationClip : Noncopyable {
	public:
		/*!
		 *@brief	ボーンのフリーズ情報。
		 */
		struct SFreezeBoneInfo {
			int boneNo;		//!<フリーズさせるボーン番号。
			bool freezeX;	//!<X方向の移動をフリーズさせる？
			bool freezeY;	//!<Y方向の移動をフリーズさせる？
			bool freezeZ;	//!<Z方向の移動をフリーズさせる？
		};
		typedef std::vector<Keyframe*>		keyFramePtrList;
		/*!
		* @brief	コンストラクタ
		*/
		CAnimationClip() 
		{
			m_freezeBoneInfo.boneNo = -1;
			m_freezeBoneInfo.freezeX = false;
			m_freezeBoneInfo.freezeY = false;
			m_freezeBoneInfo.freezeZ = false;
		}
		/*!
		 *@brief	デストラクタ。
		 */
		~CAnimationClip();
		/*!
		 *@brief	アニメーションクリップをロード。
		 *@param[in]	filePath	ファイルパス。
		 *@param[in]	clipName	クリップ名。
		 */
		void Load(const wchar_t* filePath, const wchar_t* cliipName = nullptr);
		/*!
		*@brief	ループする？
		*/
		bool IsLoop() const
		{
			return m_isLoop;
		}
		/*!
		*@brief	ループフラグを設定する。
		*/
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}
		/*!
		*@brief
		*/
		const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList() const
		{
			return *m_topBoneKeyFramList;
		}
		/*!
		 *@brief	クリップ名を取得。
		 */
		const wchar_t* GetName() const
		{
			return m_clipName.c_str();
		}
		/*!
		*@brief	アニメーションをフリーズするボーンの情報を設定。
		*@details
		* フリーズされたボーンのY方向の移動量はすべてのボーンから打ち消されます。
		*  →なにで使える？
		*		→アニメーションに移動量が入っているジャンプモーションとかで使える。
		*/
		void SetFreezeBoneInfo(int boneNo, bool freezeX, bool freezeY, bool freezeZ)
		{
			m_freezeBoneInfo.boneNo = boneNo;
			m_freezeBoneInfo.freezeX = freezeX;
			m_freezeBoneInfo.freezeY = freezeY;
			m_freezeBoneInfo.freezeZ = freezeZ;
		}
		/*!
		*@brief	X方向のアニメーションをフリーズしているボーンの番号を取得。
		*/
		const SFreezeBoneInfo& GetFreezeBoneInfo() const
		{
			return m_freezeBoneInfo;
		}
	private:
		std::wstring m_clipName;	//!<アニメーションクリップの名前。
		bool m_isLoop = false;	//!<ループフラグ。
		typedef std::unique_ptr<Keyframe> KeyframePtr;
		std::vector<KeyframePtr>	m_keyframes;	//キーフレーム。
		std::vector<keyFramePtrList>	m_keyFramePtrListArray;	//ボーンごとのキーフレームのリストを管理するための配列。
		keyFramePtrList*				m_topBoneKeyFramList = nullptr;
		SFreezeBoneInfo m_freezeBoneInfo;	//!<フリーズさせるボーン情報。
	};
	typedef std::unique_ptr<CAnimationClip>	CAnimationClipPtr;
}