/*!
 * @brief	モデルシェーダー。
 */

#include "modelCB.h"
#include "modelStruct.h"
#include "modelSRV.h"
#include "sampleBRDF.h"

#include "LightingFunction.h"


/*!
 *@brief	影を計算。
 */
int CalcShadow( float3 worldPos )
{
	int shadow = 0;
	//ちょっと適当。
	if(isShadowReceiver){
		//影を落とす。
		[unroll]
		for(int i = 0; i < NUM_SHADOW_MAP; i++ ){
			float4 posInLVP = mul(mLVP[i], float4(worldPos, 1.0f) );
			posInLVP.xyz /= posInLVP.w;
			
			float depth = min(posInLVP.z / posInLVP.w, 1.0f);
			
			//uv座標に変換。
			float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy  + float2(0.5f, 0.5f);
			float2 shadow_val = 1.0f;
			if(shadowMapUV.x < 0.99f && shadowMapUV.y < 0.99f && shadowMapUV.x > 0.01f && shadowMapUV.y > 0.01f){
				if(i == 0){
					shadow_val = shadowMap_0.Sample(Sampler, shadowMapUV ).r;
				}else if(i == 1){
					shadow_val = shadowMap_1.Sample(Sampler, shadowMapUV ).r;
				}else if(i == 2){
					shadow_val = shadowMap_2.Sample(Sampler, shadowMapUV ).r;
				}
				if( depth > shadow_val.r + 0.006f ){
					shadow = 1;
				}
				break;
			}
		}
	}
	return shadow;
}
/*!
 *@brief	法線を計算。
 */
float3 CalcNormal( float3 normal, float3 biNormal, float3 tangent, float2 uv )
{
	if(hasNormalMap){
		//法線マップがある。
		float3 binSpaceNormal = normalMap.Sample(Sampler, uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f)- 1.0f;
		normal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z; 
	}
	return normal;
}
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダーのコアプログラム。
-------------------------------------------------------------------------------------- */
PSInput VSMainCore( VSInputNmTxVcTangent In, float4x4 worldMat )
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(worldMat, In.Position);
	psInput.Pos = pos;
	pos = mul(mView, pos);
	psInput.posInView = pos;
	pos = mul(mProj, pos);
	psInput.posInProj = pos;
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(worldMat, In.Normal));
	psInput.Tangent = normalize(mul(worldMat, In.Tangent));
    return psInput;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	return VSMainCore(In, mWorld);
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー(インスタンシング描画用)。
-------------------------------------------------------------------------------------- */
PSInput VSMainInstancing( VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID )
{
	return VSMainCore(In, instanceMatrix[instanceID]);
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	//スキン行列を計算。
	float4x4 skinning = CalcSkinMatrix(In);
	//ワールド座標、法線、接ベクトルを計算。
	float4 pos = mul(skinning, In.Position);
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	psInput.Pos = pos;
	pos = mul(mView, pos);
	psInput.posInView = pos;
	pos = mul(mProj, pos);
	psInput.posInProj = pos;

	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	
    return psInput;
}


/*!
 *@brief	Z値を書き込むためだけの描画パスで使用される頂点シェーダー。
 *			スキンなしモデル用
 *@details
 * 現在はシャドウマップ作成とZPrepassで使用されています。
 */
PSInput_RenderToDepth VSMain_RenderDepth(VSInputNmTxVcTangent In)
{
	PSInput_RenderToDepth psInput = (PSInput_RenderToDepth)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.posInProj = pos;
	return psInput;
}
/*!
 *@brief	Z値を書き込むためだけの描画パスで使用される頂点シェーダー。
 *			スキンなしインスタンシングモデル用
 *@details
 * 現在はシャドウマップ作成とZPrepassで使用されています。
 */

PSInput_RenderToDepth VSMainInstancing_RenderDepth(
	VSInputNmTxVcTangent In, 
	uint instanceID : SV_InstanceID
)
{
	PSInput_RenderToDepth psInput = (PSInput_RenderToDepth)0;
	float4 pos;
	pos = mul(instanceMatrix[instanceID], In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.posInProj = pos;
	return psInput;
}
/*!
 *@brief	Z値を書き込むためだけの描画パスで使用される頂点シェーダー。
 *			スキンありモデル用。
 *@details
 * 現在はシャドウマップ作成とZPrepassで使用されています。
 */
PSInput_RenderToDepth VSMainSkin_RenderDepth(VSInputNmTxWeights In)
{
	PSInput_RenderToDepth psInput = (PSInput_RenderToDepth)0;
	//スキン行列を計算。
	float4x4 skinning = CalcSkinMatrix(In);
	//ワールド座標、法線、接ベクトルを計算。
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	
	psInput.Position = pos;
	psInput.posInProj = pos;
	return psInput;
	
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
#if 1
	//エッジを描画する
	{
		float2 screenUV = In.posInProj.xy / In.posInProj.w;
		screenUV *= float2(0.5f, -0.5f);
		screenUV += 0.5f;
		float depth = depthTexture.Sample(Sampler, screenUV).r;
		
		//上と右のテクセルを調べて、大きく深度値が異なっていたらエッジとみなす。
		float2 texSize;
		float level;
		depthTexture.GetDimensions( 0, texSize.x, texSize.y, level );
		float2 uv = screenUV + float2(2.0f / texSize.x, 0.0f);
		float depth2 = depthTexture.Sample(Sampler, uv).r;
		if(abs(depth - depth2) > 0.0001f){
			return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		uv = screenUV + float2(0.0f, 2.0f / texSize.y);
		depth2 = depthTexture.Sample(Sampler, uv).r;
		if(abs(depth - depth2) > 0.0001f){
			return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	//アルベド。
	float4 albedo = float4(albedoTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	float4 color = albedo * float4(ambientLight, 1.0f);
	int shadow = CalcShadow(In.Pos);
	if(shadow != 0){
		color.xyz *= 0.5f;
	}
	//視点までのベクトルを求める。
	float3 toEye = normalize(eyePos - In.Pos);
	//従ベクトルを計算する。
	float3 biNormal = normalize(cross(In.Tangent, In.Normal));
	//法線を計算。
	float3 normal = normalize(In.Normal);
	float toEyeLen = length(toEye);
	float3 toEyeDir = float3(1.0f, 0.0f, 0.0f);
	if(toEyeLen > 0.001f){
		toEyeDir = toEye / toEyeLen;
	}
	
	float3 toEyeReflection = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	
	//ポイントライトを計算。
	color.xyz += CalcPointLight(
		albedo,
		In.Pos, 
		In.posInProj, 
		normal,
		In.Tangent,
		biNormal,
		toEyeDir,
		toEyeReflection, 
		1.0f,
		0.0f
	);
	
	return color;
#else
	float3 lig = 0.0f;
	//視点までのベクトルを求める。
	float3 toEye = normalize(eyePos - In.Pos);
	//従ベクトルを計算する。
	float3 biNormal = normalize(cross(In.Tangent, In.Normal));
	//アルベド。
	float4 albedo = float4(albedoTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	//法線を計算。
	float3 normal = CalcNormal( In.Normal, biNormal, In.Tangent, In.TexCoord);
		
	float specPow = 0.0f;
	float roughness = 1.0f;
	if(hasSpecularMap){
		float4 t = specularMap.Sample(Sampler, In.TexCoord);
		specPow = t.x;
		roughness = 1.0f - t.w;
		roughness *= 0.8f;	//@todo マテリアルパラメータにすべきだな。
	}
	float toEyeLen = length(toEye);
	float3 toEyeDir = float3(1.0f, 0.0f, 0.0f);
	if(toEyeLen > 0.001f){
		toEyeDir = toEye / toEyeLen;
	}

	float3 toEyeReflection = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	
	//影を計算。
	int shadow = CalcShadow(In.Pos);	
	//ディレクションライト
	float3 finalColor = 0.0f;
	if(shadow == 0){
		//影が落ちている場合はディレクションライトはカットする。
		finalColor = CalcDirectionLight(
			albedo,
			In.Pos, 
			normal, 
			In.Tangent,
			biNormal,
			toEyeDir,
			toEyeReflection, 
			roughness,
			specPow
		);
	}
	
	//ポイントライトを計算。
	finalColor += CalcPointLight(
		albedo,
		In.Pos, 
		In.posInProj, 
		normal,
		In.Tangent,
		biNormal,
		toEyeDir,
		toEyeReflection, 
		roughness,
		specPow
	);
    
	//アンビエントライト。
	finalColor += CalcAmbientLight(
		albedo,
		normal,
		In.Tangent,
		biNormal,
		toEyeDir,
		roughness,
		specPow
	);
	
	// brightness
	float brightness = 1.0f;
    finalColor *= brightness;
/*
    // exposure
    float exposure = 1.0f;
    finalColor *= pow( 2.0, exposure );
  */  
    float gamma = 2.2f;
    finalColor = max( 0.0f, pow( finalColor, 1.0 / gamma ) );
    if(isnan(finalColor.x) || isnan(finalColor.y) || isnan(finalColor.z)){
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
    return float4(finalColor, 1.0f); 
#endif
}


/*!
 *@brief	Z値を書き込むためだけの描画パスで使用されるピクセルシェーダー。
 *@details
 * 現在はシャドウマップ作成とZPrepassで使用されています。
 */
float4 PSMain_RenderDepth( PSInput_RenderToDepth In ) : SV_Target0
{
	float z = In.posInProj.z / In.posInProj.w;
	return float4(z, z*z, 0.0f, 1.0f);
}

/*!
 *@brief	シルエット描画。
 */
float4 PSMain_Silhouette( PSInput In ) : SV_Target0
{
	//ディザパターン
	static const int pattern[] = {
	    0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
	    48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
	    12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
	    60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
	    3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
	    51, 19, 59, 27, 49, 17, 57, 25,
	    15, 47,  7, 39, 13, 45,  5, 37,
	    63, 31, 55, 23, 61, 29, 53, 21 
	};
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
	float2 screenPos = In.posInProj.xy / In.posInProj.w;
	screenPos = screenPos * 0.5f + 0.5f;
	
	//ディザリングを試す。
	float2 uv = fmod(screenPos * 1000.0f, 8.0f);
	float t = 0.0f;
	int x = (int)clamp(uv.x, 0.0f, 7.0f );
	int y = (int)clamp(uv.y, 0.0f, 7.0f );
	int index = y * 8 + x;
	t = (float)pattern[index] / 256.0f;
	return float4(t, t, t, 1.0f);
}