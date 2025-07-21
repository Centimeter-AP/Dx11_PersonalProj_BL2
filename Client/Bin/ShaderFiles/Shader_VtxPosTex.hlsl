#include "Engine_Shader_Defines.hlsli"

/* 상수테이블 ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
Texture2D g_DistortionMaskTexture;
Texture2D g_DepthTexture;

float g_fPercentage;
float g_fOpacity = 1.f;

// 타일 이미지용
Texture2D g_MaskTexture;
float2 g_fTileSize;
float2 g_fTileOffset;
float4 g_vColor = { 1.f, 1.f, 1.f, 1.f };
float g_fTimeAcc;

/* 정점의 기초적인 변환 (월드변환, 뷰, 투영변환) */ 
/* 정점의 구성 정보를 변형할 수 있다. */ 

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    /* SV_ : ShaderValue약자 */
    /* 내가 해야할 연산은 다 했으니 이제 니(장치)가 알아서 추가적인 연산을 해라. */     
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}

VS_OUT VS_UIBar(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = float2(In.vTexcoord.x + g_fPercentage, In.vTexcoord.y);
    
    return Out;
}


struct VS_OUT_PROJPOS
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    
};

VS_OUT_PROJPOS VS_MAIN_PROJPOS(VS_IN In)
{
    VS_OUT_PROJPOS Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    //if (Out.vColor.a < 0.2f)
    //    discard;
    
    Out.vColor.a *= g_fOpacity;
    
    return Out;
}

PS_OUT PS_MAIN_COLORING(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    //if (Out.vColor.a < 0.2f)
    //    discard;
    
    Out.vColor.rgb *= g_vColor.rgb;
    
    Out.vColor.a *= g_fOpacity;
    
    return Out;
}

PS_OUT PS_MAIN_BlackDiscard(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    if (Out.vColor.a < 0.2f)
        discard;
    if (all(Out.vColor.rgb < 0.1f))
        discard;
    
    return Out;
}

PS_OUT PS_MAIN_UIBar(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    //if (Out.vColor.a < 0.2f)
    //    discard;
    if (In.vTexcoord.x > 1.f || In.vTexcoord.x < 0.f)
        discard;    

    Out.vColor.a *= g_fOpacity;

    return Out;
}

PS_OUT PS_MAIN_UIBar_RightDiscard(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    //if (Out.vColor.a < 0.2f)
    //    discard;
    float distance = length(In.vTexcoord.x - 1.f);

    if (distance >= 1.f - g_fPercentage)
        discard;
    
    Out.vColor.a *= g_fOpacity;
    return Out;
}

struct PS_IN_PROJPOS
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

float4 SoftEffect(float4 vOrigColor, float4 vProjPos)
{
    float2 vTexcoord;
    
    vTexcoord.x = vProjPos.x / vProjPos.w;
    vTexcoord.y = vProjPos.y / vProjPos.w;
    
    vTexcoord.x = vTexcoord.x * 0.5f + 0.5f;
    vTexcoord.y = vTexcoord.y * -0.5f + 0.5f;
    
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
    
    if (vDepthDesc.y != 0.f)
    {
        float fOldViewZ = vDepthDesc.y * 1000.f;
        float fDiff = (fOldViewZ - vProjPos.w) / vProjPos.w;
        vOrigColor.a = vOrigColor.a * saturate(fDiff);
    }
    
    return vOrigColor;
}

PS_OUT PS_MAIN_SOFTEFFECT(PS_IN_PROJPOS In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    if (all(Out.vColor.rgb < 0.1f))
        discard;
    if (all(Out.vColor.rgb < 0.2f))
        Out.vColor.a = Out.vColor.r;
    /*화면 전체 기준(0, 0 ~ 1, 1)으로 이펙트의 픽셀이 그려질 위치에 해당하는 좌표 */    
    Out.vColor = SoftEffect(Out.vColor, In.vProjPos);
    
    //Out.vColor.rgb *= float3(0.6f, 0.8f, 1.f);
    return Out;
}


PS_OUT PS_MAIN_GRID(PS_IN In)
{
    PS_OUT Out;

    float2 baseUV = In.vTexcoord * g_fTileSize; 
    
    float2 finalUV = baseUV + g_fTileOffset.xy;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, finalUV);
    
    if (Out.vColor.a <= 0.01f)
        discard;
    
    float fDissolveValue = g_MaskTexture.Sample(DefaultSampler, finalUV).r;


    if (fDissolveValue <= 1.f - g_fOpacity)
        Out.vColor.a = fDissolveValue;
    
    return Out;
}

PS_OUT PS_MAIN_SOFTEFFECT_COLOR(PS_IN_PROJPOS In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    if (all(Out.vColor.rgb < 0.1f))
        discard;
    if (all(Out.vColor.rgb < 0.2f))
        Out.vColor.a = Out.vColor.r;
    /*화면 전체 기준(0, 0 ~ 1, 1)으로 이펙트의 픽셀이 그려질 위치에 해당하는 좌표 */    
    Out.vColor = SoftEffect(Out.vColor, In.vProjPos);
    
    //Out.vColor.rgb *= float3(0.6f, 0.8f, 1.f);
    Out.vColor *= g_vColor;
    return Out;
}

PS_OUT PS_MAIN_SOFTEFFECT_DISTORT(PS_IN_PROJPOS In)
{
    PS_OUT Out;
    
    float2 uv = In.vTexcoord;

    // 사인 파형으로 왜곡, sin(시간)은 -1에서 1 사이를 왔다갔다함 5.0은 주파수, 0.02는 진폭값
    uv.x += sin(g_fTimeAcc * 5.0f + uv.y * 10.0f) * 0.02f;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, uv);
    vector vDistortMask = g_DistortionMaskTexture.Sample(DefaultSampler, In.vTexcoord);
   
    //if (all(Out.vColor.rgb < 0.1f))
    //    discard;
    //if (all(Out.vColor.rgb < 0.2f))
    //    Out.vColor.a = Out.vColor.r;
    /*화면 전체 기준(0, 0 ~ 1, 1)으로 이펙트의 픽셀이 그려질 위치에 해당하는 좌표 */    
    Out.vColor = SoftEffect(Out.vColor, In.vProjPos);
    
    Out.vColor *= g_vColor;
    return Out;
}

technique11 DefaultTechnique
{
    pass Default //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass AlphaTest_BlackDiscard//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BlackDiscard();
    }
    pass AlphaTest_AlphaBlend//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass AlphaTest_AlphaBlendAndColor //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COLORING();
    }
    pass AlphaTest_AlphaBlend_For_UIBars//4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_UIBar();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UIBar();
    }
    pass AlphaTest_AlphaBlend_For_UIBars_RightDiscard//5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UIBar_RightDiscard();
    }
    pass SoftEffect // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_PROJPOS();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT();
    }
    pass Grid_Pick // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GRID();
    }
    pass WaterExplode // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_PROJPOS();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT_COLOR();
    }
    pass Phaselock_Distort // 9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_PROJPOS();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT_DISTORT();
    }
}
