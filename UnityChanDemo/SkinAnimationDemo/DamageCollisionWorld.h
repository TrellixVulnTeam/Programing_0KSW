/*!
 * @brief	敵、プレイヤーにダメージを与えるコリジョンワールド。
 */

#pragma once

class DamageCollisionWorld : public IGameObject{
public:
	/*!
	 * @brief	属性。
	 */
	enum EnAttr{
		enDamageToEnemy,		//敵にダメージを与えるコリジョン。
		enDamageToPlayer,		//プレイヤーにダメージを与えるコリジョン。
	};
	/*!
	 * @brief	コンストラクタ。
	 */
	DamageCollisionWorld();
	/*!
	 * @brief	デストラクタ。
	 */
	~DamageCollisionWorld();
	/*!
	 * @brief	コリジョンを追加。
	 */
	void Add( 
		float radius, 
		const CVector3& pos, 
		float life, 
		float damage,
		EnAttr attr
	)
	{
		Collision colli;
		colli.radius = radius;
		colli.position = pos;
		colli.life = life;
		colli.attr = attr;
		colli.time = 0.0f;
		collisions.push_back(colli);
	}
	void Start() override
	{

	}
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	*@brief	描画
	*/
	void Render(CRenderContext& renderContext) override
	{

	}
private:
	struct Collision{
		float radius;			//半径。
		CVector3 position;		//中心。
		float life;				//寿命。
		float damage;			//ダメージ量。
		EnAttr attr;			//属性。
		float time;
	};
	std::list<Collision>		collisions;
};