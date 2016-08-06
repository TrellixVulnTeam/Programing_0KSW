/*!
 * @brief	スキンモデルシェーダー。(4ボーンスキニング)
 */

//スキン行列。
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;
float		g_numBone;			//骨の数。

float4x4	g_worldMatrix;			//!<ワールド行列。
float4x4	g_rotationMatrix;		//!<回転行列。

texture g_diffuseTexture;		//ディフューズテクスチャ。
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};


/*!
 * @brief	入力頂点
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

/*!
 * @brief	インスタンシング描画用の入力頂点。
 */
struct VS_INPUT_INSTANCING
{
	VS_INPUT	base;
	float4 mWorld1	: TEXCOORD1;		//ワールド行列の1行目
	float4 mWorld2	: TEXCOORD2;		//ワールド行列の2行目
	float4 mWorld3	: TEXCOORD3;		//ワールド行列の3行目
	float4 mWorld4	: TEXCOORD4;		//ワールド行列の4行目
};
/*!
 * @brief	出力頂点。
 */
struct VS_OUTPUT
{
	float4  Pos     : POSITION;
    float3  Normal	: NORMAL;
    float2  Tex0    : TEXCOORD0;
};
/*!
 *@brief	ワールド座標とワールド法線をスキン行列から計算する。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal )
{
	Pos = 0.0f;
	Normal = 0.0f;
	//ブレンドするボーンのインデックス。
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//ブレンドレート。
	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(In.Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(In.Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 
    
	Pos += (mul(In.Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    Normal += (mul(In.Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
}
/*!
 *@brief	ワールド座標とワールド法線を計算。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 */
void CalcWorldPosAndNormal( VS_INPUT In, out float3 Pos, out float3 Normal )
{
	Pos = mul(In.Pos, g_worldMatrix );
	Normal = mul(In.Normal, g_rotationMatrix );
}
/*!
 *@brief	頂点シェーダー。
 *@param[in]	In			入力頂点。
 *@param[in]	hasSkin		スキンあり？
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In, Pos, Normal );
	}
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.Normal = normalize(Normal);
    o.Tex0 = In.Tex0;
	return o;
}
/*!
 *@brief	頂点シェーダー。
 *@param[in]	In			入力頂点。
 *@param[in]	hasSkin		スキンあり？
 */
VS_OUTPUT VSMainInstancing( VS_INPUT_INSTANCING In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In.base, Pos, Normal );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//ワールド行列をかける。
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.Normal = normalize(Normal);
    o.Tex0 = In.base.Tex0;
	return o;
}
/*!
 * @brief	ピクセルシェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	return tex2D(g_diffuseTextureSampler, In.Tex0);
}
/*!
 *@brief	スキンありモデル用のテクニック。
 */
technique SkinModel
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMain(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	スキンなしモデル用のテクニック。
 */
technique NoSkinModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}

/*!
 *@brief	スキンありモデルのインスタンシング描画用のテクニック。
 */
technique SkinModelInstancing
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMainInstancing(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	スキンなしモデルのインスタンシング描画用のテクニック。
 */
technique NoSkinModelInstancing
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}
