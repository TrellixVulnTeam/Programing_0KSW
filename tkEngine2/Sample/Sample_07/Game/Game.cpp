#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "Player.h"
#include "Background.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
	m_lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig = NewGO<prefab::CDirectionLight>(0);
	//ライトの方向を設定。
	m_lig->SetDirection(m_lightDir);
	//ライトの色を設定。
	m_lig->SetColor({ 300.5f, 300.5f, 300.5f, 1.0f });


	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 15.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 15.0f, 100.0f });
	MainCamera().Update();
	//プレイヤーのインスタンスを生成する。
	m_player = NewGO<Player>(0);
	//背景のインスタンスを生成する。
	m_background = NewGO<Background>(0);
	return true;
}
/*!
*@brief	削除されるときに呼ばれる。
*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
* デストラクタより前に実行されます。
*/
void Game::OnDestroy()
{
	DeleteGO(m_player);
}
void Game::Update()
{
}
