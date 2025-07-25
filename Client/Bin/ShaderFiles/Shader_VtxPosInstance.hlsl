#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
vector g_vCamPosition;
float3 g_vColor = { 1.f, 1.f, 1.f };

struct VS_IN
{
    float3 vPosition : POSITION;       
    
    row_major float4x4 TransformMatrix : WORLD;
    
    float2 vLifeTime : TEXCOORD0;    
};

struct VS_OUT
{
    /* SV_ : ShaderValue약자 */
    /* 내가 해야할 연산은 다 했으니 이제 니(장치)가 알아서 추가적인 연산을 해라. */     
    float4 vPosition : POSITION;
    
    float2 vPSize : PSIZE;
    
    float2 vLifeTime : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;    
   
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);    
    
    Out.vPosition = mul(vPosition, g_WorldMatrix);
    
    Out.vPSize = float2(length(In.TransformMatrix._11_12_13), length(In.TransformMatrix._21_22_23));
    
    Out.vLifeTime = In.vLifeTime;
    
    return Out;
}

/* 그리는 형태에 따라서 호출된다. */ 

struct GS_IN
{
    float4 vPosition : POSITION;
    
    float2 vPSize : PSIZE;
    
    float2 vLifeTime : TEXCOORD0;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
    
    float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
    float3 vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);    
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    
    Triangles.Append(Out[0]);
    Triangles.Append(Out[1]);
    Triangles.Append(Out[2]);
    Triangles.RestartStrip();
    
    Triangles.Append(Out[0]);
    Triangles.Append(Out[2]);
    Triangles.Append(Out[3]);
    Triangles.RestartStrip();
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;    
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);           
    
    if(Out.vColor.a < 0.01f || Out.vColor.r < 0.02f)
        discard;
    
    Out.vColor.a *= saturate(In.vLifeTime.y / In.vLifeTime.x);
    //Out.vColor.a = Out.vColor.a * saturate(In.vLifeTime.x - In.vLifeTime.y);
    //Out.vColor.rgb *= g_vColor;

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    return Out;
}

PS_OUT PS_MAIN_GRID(PS_IN In)
{
    PS_OUT Out;    
    
    float2 baseUV = In.vTexcoord * 0.5f;
    
    float2 finalUV = baseUV + float2(0.f, 0.5f);
    
    Out.vColor = g_Texture.Sample(DefaultSampler, finalUV);
    
    if(Out.vColor.a < 0.3f)
        discard;
    

    
    Out.vColor.a = In.vLifeTime.y / In.vLifeTime.x;
    Out.vColor.rgb *= g_vColor;

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    return Out;
}

PS_OUT PS_MAIN_COLOR(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (Out.vColor.a < 0.3f || Out.vColor.r < 0.2f)
        discard;
    

    
    //Out.vColor.a = saturate(In.vLifeTime.x - In.vLifeTime.y);
    Out.vColor.rgb *= g_vColor;

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    return Out;
}


technique11 DefaultTechnique
{
  
    pass Default//0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();     
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
 
    pass Grid_Pick//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();    
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_GRID();      
    }
   
    pass Color//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_COLOR();
    }
}
