/*!
 * @brief	スプライト。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/2D/tkSprite.h"

namespace tkEngine{
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	CSprite::CSprite()
	{
	}
	CSprite::~CSprite()
	{
	}
	void CSprite::Init(CShaderResourceView& tex, float w, float h)
	{
		//シェーダーロード。
		m_ps.Load("Assets/shader/sprite.fx", "PSMain", CShader::EnType::PS);
		m_vs.Load("Assets/shader/sprite.fx", "VSMain", CShader::EnType::VS);
		m_size.x = w;
		m_size.y = h;
		float halfW = w * 0.5f;
		float halfH = h * 0.5f;
		//頂点バッファのソースデータ。
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-halfW, -halfH, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(halfW, -halfH, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-halfW, halfH, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(halfW, halfH, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		
		m_primitive.Create(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			4,
			sizeof(SSimpleVertex),
			vertices,
			4,
			CIndexBuffer::enIndexType_16,
			indices
		);
		m_textureSRV = &tex;
		m_cb.Create(nullptr, sizeof(SSpriteCB));
	}
	void CSprite::Draw(CRenderContext& rc, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		if (m_textureSRV == nullptr) {
			TK_WARNING("m_textureSRV is nullptr");
			return;
		}
		SSpriteCB cb;
		//ピボットを考慮に入れた平行移動行列を作成。
		//ピボットは真ん中が0.0, 0.0、左上が-1.0f, -1.0、右下が1.0、1.0になるようにする。
		CVector2 localPivot = m_pivot;
		localPivot.x -= 0.5f;
		localPivot.y -= 0.5f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//画像のハーフサイズを求める。
		CVector2 halfSize = m_size;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		CMatrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(m_position);
		mRot.MakeRotationFromQuaternion(m_rotation);
		mScale.MakeScaling(m_scale);
		cb.WVP.Mul(mPivotTrans, mScale);
		cb.WVP.Mul(cb.WVP, mRot);
		cb.WVP.Mul(cb.WVP, mTrans);
		cb.WVP.Mul(cb.WVP, viewMatrix);
		cb.WVP.Mul(cb.WVP, projMatrix);

		rc.UpdateSubresource(m_cb, &cb);
		rc.VSSetConstantBuffer(0, m_cb);
		rc.PSSetShaderResource(0, *m_textureSRV);
		rc.PSSetShader(m_ps);
		rc.VSSetShader(m_vs);
		rc.IASetInputLayout(m_vs.GetInputLayout());
		m_primitive.Draw(rc);
	}
}