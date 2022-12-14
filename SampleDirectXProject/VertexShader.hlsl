
struct VS_INPUT
{
	float4 position: POSITION;
	float3 color: COLOR;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
    float3 color : COLOR;
    float4 depthPosition : TEXTURE0;

};

cbuffer constant: register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    unsigned int m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//World space
    output.position = mul(input.position, m_world);
	//View space
    output.position = mul(output.position, m_view);
	//Screen space
    output.position = mul(output.position, m_proj);

	output.color = input.color;

    output.depthPosition = output.position;
	return output;
}