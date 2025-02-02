/*!
 * @brief	配置データ。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	オブジェクトの配置データを扱うクラス。
	 * @details
	 *	スケルトンデータを使います。CSkeletonクラスのインターフェースを
	 *	配置データとして扱いやすいように変換するアダプタクラスです。
	 */
	class CLocData : Noncopyable{
	public:
		struct SObjectData {
			CVector3 position;
			CQuaternion rotation;
		};
		CLocData()
		{
		}
		~CLocData()
		{
		}
		/*!
		*@brief	配置データを読み込み。
		*/
		bool Load(const wchar_t* filePath)
		{
			CSkeleton locData;

			if(locData.Load(filePath) == false){
				//読み込みに失敗。
				TK_WARNING_MESSAGE_BOX("配置データの読み込みに失敗しました。%ls", filePath);
				return false;
			}
			for (int i = 1; i < locData.GetNumBones(); i++) {
				SObjectData objData;
				auto bone = locData.GetBone(i);

				CVector3 scale;
				bone->CalcWorldTRS(objData.position, objData.rotation, scale);

				//3dsMaxとは軸が違うので、補正を入れる。
				auto t = objData.position.y;
				objData.position.y = objData.position.z;
				objData.position.z = -t;

				t = objData.rotation.y;
				objData.rotation.y = objData.rotation.z;
				objData.rotation.z = -t;

				//拡大は未対応かなぁ。

				m_objectData.push_back(std::move(objData));
			}
			return true;
		}
		/*!
		*@brief	配置されているオブジェクトの数を取得。
		*/
		int GetNumObject() const
		{
			return static_cast<int>(m_objectData.size());
		}
		/*!
		 *@brief	配置されているオブジェクトの座標を取得。
		 */
		const CVector3& GetObjectPosition(int objNo) const
		{
			return m_objectData.at(objNo).position;
		}
		/*!
		 *@brief	配置されているオブジェクトの回転を取得。
		 */
		const CQuaternion& GetObjectRotation(int objNo) const
		{
			return m_objectData.at(objNo).rotation;
		}
	private:
		CSkeleton	m_locData;	//!<スケルトンデータ。
		std::vector<SObjectData>	m_objectData;
	};
}