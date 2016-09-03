#pragma once

class Sky : public IGameObject {
	CSkinModelData	skinModelData;		//スキンモデルデータ。
	CSkinModel		skinModel;			//スキンモデル。
	CAnimation		animation;			//アニメーション。
	CLight			light;				//ライト。
	CTexture		normalMap;
public:
	Sky();
	
	~Sky()
	{

	}
	void Start() override
	{

	}
	void Update() override;
	
	void Render(CRenderContext& renderContext) override;
	
};
