#include "Engine_Shader_Defines.hlsli"

/* ������̺� ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

float g_fPercentage;

/* ������ �������� ��ȯ (���庯ȯ, ��, ������ȯ) */ 
/* ������ ���� ������ ������ �� �ִ�. */ 

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    /* SV_ : ShaderValue���� */
    /* ���� �ؾ��� ������ �� ������ ���� ��(��ġ)�� �˾Ƽ� �߰����� ������ �ض�. */     
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
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
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = float2(In.vTexcoord.x + g_fPercentage, In.vTexcoord.y);
    
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
   
    if (Out.vColor.a < 0.2f)
        discard;
    
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

    if (Out.vColor.a < 0.2f)
        discard;
    if (In.vTexcoord.x > 1.f || In.vTexcoord.x < 0.f)
        discard;


    return Out;
}

PS_OUT PS_MAIN_UIBar_RightDiscard(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    if (Out.vColor.a < 0.2f)
        discard;
    float distance = length(In.vTexcoord.x - 1.f);

    if (distance <= g_fPercentage)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass AlphaTest_BlackDiscard
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BlackDiscard();
    }
    pass AlphaTest_AlphaBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass AlphaTest_AlphaBlend_For_UIBars
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_UIBar();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UIBar();
    }
    pass AlphaTest_AlphaBlend_For_UIBars_RightDiscard
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UIBar_RightDiscard();
    }
}
