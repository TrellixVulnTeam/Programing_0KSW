/*!
 * @brief	プレイヤー
 */

#include "stdafx.h"
#include "Player/Player.h"
#include "Player/PlayerRenderer.h"
#include "Player/PlayerAnimator.h"



CPlayer::CPlayer() :
	m_playerRenderer(this),
	m_playerAnimator(this),
	m_playerMove(this),
	m_playerTurn(this),
	m_playerStateMachine(this)
{
}
CPlayer::~CPlayer()
{
}

void CPlayer::OnDestroy()
{
	DeleteGO(&m_playerRenderer);
	DeleteGO(&m_playerAnimator);
	DeleteGO(&m_playerStateMachine);
	DeleteGO(&m_playerMove);
	DeleteGO(&m_playerTurn);
}
bool CPlayer::Start()
{
	AddGO(0, &m_playerRenderer, nullptr);
	AddGO(0, &m_playerAnimator, nullptr);
	AddGO(0, &m_playerStateMachine, nullptr);
	AddGO(0, &m_playerMove, nullptr);
	AddGO(0, &m_playerTurn, nullptr);
	return true;
}

void CPlayer::Update()
{
	//回転行列から前方方向を計算する。
	CMatrix mRotation;
	mRotation.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRotation.m[2][0];
	m_forward.y = mRotation.m[2][1];
	m_forward.z = mRotation.m[2][2];

	//XZ平面での前方方向を計算。
	m_forwardXZ = m_forward;
	m_forwardXZ.y = 0.0f;
	m_forwardXZ.Normalize();
}

void CPlayer::Render(CRenderContext& rc)
{
	(void)rc;
}