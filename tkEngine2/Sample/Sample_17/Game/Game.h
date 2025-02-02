#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
	CSkinModel skinModel;							//スキンモデル。
	CSkinModelData skinModelData;					//スキンモデルデータ。
	CAnimationClip m_animClip[1];					//アニメーションクリップ。
	CAnimation m_animation;
	CVector3 m_pos = CVector3::Zero;
};

