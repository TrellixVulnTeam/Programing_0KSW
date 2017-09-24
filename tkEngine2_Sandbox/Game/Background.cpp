#include "stdafx.h"
#include "Background.h"



Background::Background()
{
}


Background::~Background()
{
}
bool Background::Start()
{
	m_skinModelData.Load(L"Assets/modelData/background.cmo");
	m_skinModel.Init(m_skinModelData);
	m_normalMapSRV.CreateFromDDSTextureFromFile(L"Assets/modelData/BB_Map_1A Normal.dds");
	m_skinModel.FindMesh([&](auto& mesh) {
		CModelEffect* effect = reinterpret_cast<CModelEffect*>(mesh->effect.get());
		if (effect->EqualMaterialName(L"Material #25")) {
			effect->SetNormalMap(m_normalMapSRV.GetBody());
		}
	});
	m_skinModel.SetShadowReceiverFlag(true);
	return true;
}
void Background::Update()
{
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisX, -90.0f);
	m_skinModel.Update({ 0.0f, 20.0f, 0.0f }, qRot, CVector3::One);
}
void Background::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
