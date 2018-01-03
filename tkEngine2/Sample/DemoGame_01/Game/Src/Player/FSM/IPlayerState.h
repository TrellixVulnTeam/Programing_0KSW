/*!
 * @brief	プレイヤーのステートのインターフェース
 */

#pragma once

class CPlayerStateMachine;
class CPlayer;
class IPlayerState : public IGameObject {
public:
	IPlayerState(CPlayer* pl, CPlayerStateMachine* psm) :
		m_player(pl),
		m_psm(psm)
	{
	}
	/*!
	 * @brief	移動できるか判定
	 */
	virtual bool IsMove() const
	{
		return true;
	}
protected:
	CPlayer* m_player = nullptr;
	CPlayerStateMachine* m_psm = nullptr;
};