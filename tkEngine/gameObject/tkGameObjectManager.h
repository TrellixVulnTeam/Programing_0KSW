/*!
 *@brief	CGameObjectのマネージャー。
 */

#ifndef _CGAMEOBJECTMANAGER_H_
#define _CGAMEOBJECTMANAGER_H_

#include "tkEngine/gameObject/tkGameObject.h"
#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	struct SRenderContextMap;
	/*!
	 *@brief	CGameObjectのマネージャ
	 */
	class CGameObjectManager : Noncopyable{
	private:
		CGameObjectManager() :
			m_gameObjectPriorityMax(0)
		{
		}
		~CGameObjectManager()
		{
		}
		/*!
		*@brief	ゲームオブジェクトの名前キーを作成。
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = CUtil::MakeHash("Undefined");	//名前キー。
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = CUtil::MakeHash(objectName);
			}
			return hash;
		}
	public:
		/*!
		 *@brief	インスタンスの取得。
		 */
		static CGameObjectManager& Instance()
		{
			static CGameObjectManager instance;
			return instance;
		}
		/*!
		*@brief	実行。
		*@param[in]	renderContext		レンダリングコンテキスト。
		*@param[in]	numRenderContext	レンダリングコンテキストの数。
		*@param[in]	renderContextMap	レンダリングコンテキストのマップ。
		*@param[in]	preRender			プリレンダー
		*@param[in]	postEffect			ポストエフェクト。
		*/
		void Execute( 
			CRenderContext* renderContext, 
			int numRenderContext, 
			const SRenderContextMap* renderContextMap,
			CPreRender& preRender,
			CPostEffect& postEffect
		);
		/*!
		 *@brief	初期化。
		 *@param[in]	gameObjectPrioMax	ゲームオブジェクトの優先度の最大値。(255まで)
		 */
		void Init( int gameObjectPrioMax );
		/*!
		*@brief	ゲームオブジェクトの追加。
		*@param[in]	prio			実行優先順位。
		*@param[in] go				ゲームオブジェクト
		*@param[in] objectName		オブジェクト名
		*/
		void AddGameObject(GameObjectPrio prio, IGameObject* go, const char* objectName = nullptr)
		{
			if (!go->m_isRegist) {
				go->Awake();
				unsigned int hash = MakeGameObjectNameKey(objectName);
				m_gameObjectListArray.at(prio).push_back(go);
				go->m_isRegist = true;
				go->m_priority = prio;
				go->m_isStart = false;
				go->m_nameKey = hash;
				if (go->m_isDead) {
					//死亡フラグが立っている。
					//削除リストに入っていたらそこから除去する。
					go->m_isDead = false;
				}
				
			}
		}
		/*!
		 *@brief	ゲームオブジェクトのnew
		 *@details
		 * この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
		 *@param[in]	prio		実行優先順位。
		 *@param[in]	objectName	オブジェクト名。
		 */
		template<class T>
		T* NewGameObject(GameObjectPrio prio, const char* objectName = nullptr)
		{
			TK_ASSERT( prio <= m_gameObjectPriorityMax, "ゲームオブジェクトの優先度の最大数が大きすぎます。");
			T* newObject = new T();
			newObject->Awake();
			newObject->SetMarkNewFromGameObjectManager();
			unsigned int hash = MakeGameObjectNameKey(objectName);
			m_gameObjectListArray.at(prio).push_back(newObject);
			newObject->m_isRegist = true;
			newObject->m_priority = prio;
			newObject->m_nameKey = hash;
			return newObject;
		}
		/*!
		*@brief	ゲームオブジェクトの検索。
		*@details
		* 重いよ！
		*@param[in]	objectName		オブジェクト名。
		*/
		template<class T>
		T* FindGameObject(const char* objectName)
		{
			unsigned int nameKey = CUtil::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//見つけた。
						return dynamic_cast<T*>(go);
					}
				}
			}
			//見つからなかった。
			return nullptr;
		}
		/*!
		*@brief	ゲームオブジェクトの検索。
		*@details
		* 重いよ！
		*@param[in]	objectName		オブジェクト名。
		*/
		template<class T>
		void FindGameObject(const char* objectName, std::function<void(T* obj)> func)
		{
			unsigned int nameKey = CUtil::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//見つけた。
						func(dynamic_cast<T*>(go));
					}
				}
			}
		}
		/*!
		 *@brief	ゲームオブジェクトの削除。
		 */
		void DeleteGameObject( IGameObject* gameObject )
		{
			if (gameObject != nullptr
				&& gameObject->m_isRegist
			) {
				gameObject->SetDeadMark();
				gameObject->OnDestroy();
				gameObject->m_isRegist = false;
				gameObject->m_isRegistDeadList = true;
				m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
				gameObject = nullptr;
			}
		}
		/*!
		*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
		*/
		
		void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)> func)
		{
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					unsigned int goTags = go->GetTags();
					if ((goTags & tags) != 0) {
						func(go);
					}
				}
			}
			
		}
	private:
		/*!
		 *@brief	ゲームオブジェクトの削除を実行。
		 */
		void ExecuteDeleteGameObjects();
	private:
		typedef std::list<IGameObject*>	GameObjectList;
		std::vector<GameObjectList>	m_gameObjectListArray;		//!<ゲームオブジェクトの優先度付きリスト。
		std::vector<GameObjectList>	m_deleteObjectArray[2];		//!<削除するオブジェクトのリスト。削除処理を行っている最中にDeleteGameObjectが呼ばれる可能性が高いので、ダブルバッファ化。
		GameObjectPrio				m_gameObjectPriorityMax;	//!<ゲームオブジェクトの優先度の最大数。
		int m_currentDeleteObjectBufferNo = 0;					//!<現在の削除オブジェクトのバッファ番号。
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<ゲームオブジェクトの優先度の最大値。
	};

	static inline CGameObjectManager& GameObjectManager()
	{
		return CGameObjectManager::Instance();
	}
	/*!
	 *@brief	ゲームオブジェクト生成のヘルパー関数。
	 *@param[in]	priority	プライオリティ。
	 *@param[in]	objectName	オブジェクト名。
	 */
	template<class T>
	static inline T* NewGO( int priority, const char* objectName = nullptr)
	{
		return GameObjectManager().NewGameObject<T>( priority, objectName );
	}
	/*!
	 *@brief	ゲームオブジェクト削除のヘルパー関数。
	 * NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	 *@param[in]	go		削除するゲームオブジェクト。
	 */
	static inline void DeleteGO(IGameObject* go )
	{
		GameObjectManager().DeleteGameObject(go);
	}
	/*!
	 *@brief	ゲームオブジェクトの追加のヘルパー関数。
	 *@param[in]	go			追加するゲームオブジェクト。
	 *@param[in]	priority	プライオリティ。
	 *@param[in]	objectName	ゲームオブジェクトの名前。
	 */
	static inline void AddGO(int priority, IGameObject* go, const char* objectName = nullptr)
	{
		GameObjectManager().AddGameObject(priority, go, objectName);
	}
	/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*/
	template<class T>
	static inline T* FindGO(const char* objectName)
	{
		return GameObjectManager().FindGameObject<T>(objectName);
	}
	/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*/
	template<class T>
	static inline void FindGO(const char* objectName, std::function<void(T* obj)> func)
	{
		return GameObjectManager().FindGameObject<T>(objectName, func);
	}
	/*!
	*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
	*/
	static inline 	void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)> func)
	{
		GameObjectManager().FindGameObjectsWithTag(tags, func);
	}
}
#endif // _CGAMEOBJECTMANAGER_H_
 