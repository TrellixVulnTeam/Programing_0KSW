#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 20.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 20.0f, 500.0f });
	MainCamera().SetNear(0.1f);
	MainCamera().SetFar(1000.0f);
	MainCamera().Update();

	//モデルデータをロード。
	m_skinModelData.Load(L"modelData/unityChan.cmo");
	m_skinModel.Init(m_skinModelData);
	m_animClip[0].Load(L"animData/idle.tka", L"idle");
	m_animClip[0].SetLoopFlag(true);
	m_animation.Init(m_skinModelData, m_animClip, 1);
	m_animation.Play(0);
	return true;
}
void Game::Update()
{
	//上下左右のキー入力による移動処理。
	if (Pad(0).IsPress(enButtonRight)) {
		m_position.x -= 5.0f;
	}
	else if (Pad(0).IsPress(enButtonLeft)) {
		m_position.x += 5.0f;
	}
	else if (Pad(0).IsPress(enButtonUp)) {
		m_position.y += 5.0f;
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		m_position.y -= 5.0f;
	}
	//アナログスティックによる移動処理。
	CVector3 stick;
	
	stick.x = -Pad(0).GetLStickXF();	//アナログスティックのxの入力量を取得。
	stick.y = Pad(0).GetLStickYF();	//アナログスティックのxの入力量を取得。
	stick.z = 0.0f;
	m_position += stick * 5.0f;
	
	m_animation.Update(GameTime().GetFrameDeltaTime());
	CQuaternion qRot = CQuaternion::Identity;
	qRot.SetRotationDeg(CVector3::AxisX, 90.0f);
	//ワールド行列を更新。
	m_skinModel.Update(m_position, qRot, CVector3::One);
}
void Game::Render(CRenderContext& rc)
{
	//描画。
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}