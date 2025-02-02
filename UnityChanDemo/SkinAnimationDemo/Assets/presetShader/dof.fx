/*!
 * @brief	被写界深度。
 */



texture g_depthTexture;	//深度テクスチャ。
float2 g_rtSize;		//レンダリングターゲットのサイズ。

float2 g_nearFar;
sampler g_depthSampler = 
sampler_state
{
	Texture = <g_depthTexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float3 g_dofParam;		//
texture g_blurBack;	//奥ボケ
sampler g_blurBackSampler = 
sampler_state
{
	Texture = <g_blurBack>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

texture g_blurForward;	//手前ボケ
sampler g_blurForwardSampler = 
sampler_state
{
	Texture = <g_blurForward>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


struct VS_INPUT{
	float4	pos : POSITION;
};

struct VS_OUTPUT{
	float4 pos 	: POSITION;
	float2 tex0 : TEXCOORD0;
};
/*!
 * @brief	頂点シェーダ。
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
	
	tex.y = 1.0f - tex.y;
	Out.tex0 = tex + float2( 0.5/g_rtSize.x, 0.5/g_rtSize.y);
	return Out;
}

/*!
 * @brief	ピクセルシェーダー。
 */
float4 PSMain(VS_OUTPUT In ) : COLOR
{
	float4 depth = tex2D(g_depthSampler, In.tex0 + float2(0.5f/g_rtSize.x,0.0f));
	depth = min( depth, tex2D(g_depthSampler, In.tex0 + float2(-0.5f/g_rtSize.x,0.0f) ) );
	depth = min( depth, tex2D(g_depthSampler, In.tex0 + float2(0.0f,-0.5f/g_rtSize.y) ) );
	depth = min( depth, tex2D(g_depthSampler, In.tex0 + float2(0.0f,0.5f/g_rtSize.y) ) );
	
	//手前ボケ
	float alpha = depth - g_dofParam.z;
	float forwardRate = max( 0.0f, -g_dofParam.x - alpha );
	float backRate = max(0.0f, alpha - g_dofParam.y);
	alpha = max(forwardRate, backRate);
	alpha = alpha / ((g_dofParam.z- g_dofParam.x));
	float4 color = 0.0f;
	if(forwardRate < backRate){
		//奥ボケ
		alpha *= g_dofParam.x / (g_dofParam.y);
		alpha = min(1.0f, alpha);
		float4 blur = tex2D(g_blurBackSampler, In.tex0);
		color = float4(blur.xyz, alpha);

	}else{
		//手前ボケ
		alpha = min(1.0f, alpha * 2.0f);
		float4 blur = tex2D(g_blurForwardSampler, In.tex0);
		color = float4(blur.xyz, alpha);
	}
	
	return color;
}
/*!
 * @brief	手前ボケと奥ボケの合成テクニック。
 */
technique CombineBackForwardBoke
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}
