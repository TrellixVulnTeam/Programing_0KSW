#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"
#include "Physics/SphereCollider.h"
#include "Physics/RigidBody.h"
#include "CharacterController.h"

/*!
 * @brief	ユニティちゃん
 */
class UnityChan : public IGameObject {
private:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//立ち。
		AnimationWalk,		//歩き。
		AnimationRun,		//走り。
		AnimationJump,		//ジャンプ。
	};
public:
	enum EnState {
		enStateRun,			//走っている。
		enStateStand,		//立ち止まっている。
		enState_RideOnCar,	//車に乗っている
	};
	bool					isUpdateAnim;		//

	UnityChan() :
		position(CVector3::Zero),
		isUpdateAnim(false)
	{

	}
	void Start() override ;
	void Update() override ;
	void Render( CRenderContext& renderContext ) override;
	void SetPosition(const CVector3& position)
	{
		this->position = position;
	}
	const CVector3& GetPosition() const
	{
		return position;
	}
	/*!
	* @brief	状態を取得。
	*/
	EnState GetState() const
	{
		return state;
	}
	/*!
	* @brief	ポイントライトの位置を取得。
	*/
	const CVector3& GetPointLightPosition() const
	{
		return pointLightPosition;
	}
	/*!
	* @brief	ポイントライトの色を取得。
	*/
	const CVector4& GetPointLightColor() const
	{
		return pointLightColor;
	}
private:
	/*!
	* @brief	ポイントライトの位置を更新。
	*/
	void UpdatePointLightPosition();
	/*!
	* @brief	アニメーションコントロール。
	*/
	void AnimationControl();
	/*!
	* @brief	アニメーション再生。
	*/
	void PlayAnimation(AnimationNo animNo);
private:
	CSkinModelDataHandle	skinModelData;
	CSkinModel				skinModel;			//スキンモデル。
	CAnimation				animation;			//アニメーション。
	CLight					light;				//ライト。
	AnimationNo				currentAnimSetNo;
	CTexture				normalMap;			//法線マップ。
	CTexture				specMap;			//スペキュラマップ。
	CVector3				position;			//座標。
	CQuaternion				rotation;			//回転
	CVector3				toLightPos;			//
	CVector3				pointLightPosition;	//ポイントライトの位置。
	CVector4				pointLightColor;	//ポイントライトのカラー。
	CVector3				toLampLocalPos;		//ランプのローカル座標。
	EnState					state;				//状態。
	EnState					lastFrameState;		//前のフレームの状態。
	bool					isPointLightOn;		//ポイントライトのスイッチ。
	RigidBody				rigidBody;			//剛体。
	CharacterController		characterController;	//キャラクタコントローラ。
};