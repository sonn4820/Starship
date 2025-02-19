#pragma once

const char* g_defaultShaderSource = R"(
	float Interpolate(float start, float end, float fractionTowardEnd)
	{
		return start + fractionTowardEnd * (end - start);
	}
	float GetFractionWithinRange(float value, float rangeStart, float rangeEnd)
	{
		return (value - rangeStart) / (rangeEnd - rangeStart);
	}
	float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
	{
		float t = GetFractionWithinRange(inValue, inStart, inEnd);
		return Interpolate(outStart, outEnd, t);
	}
	cbuffer CameraConstants : register(b2)
	{
		float4x4 ProjectionMatrix;
		float4x4 ViewMatrix;
	};
	cbuffer ModelConstants : register(b3)
	{
		float4x4 ModelMatrix;
		float4 ModelColor;
	};

	Texture2D diffuseTexture: register(t0);
	SamplerState diffuseSampler: register(s0);

	struct vs_input_t
	{
		float3 localPosition : POSITION;
		float4 color : COLOR;
		float2 uv : TEXCOORD;
	};
	
	struct v2p_t
	{
		float4 position : SV_Position;
		float4 color : COLOR;
		float2 uv : TEXCOORD;
	};
	
	v2p_t VertexMain(vs_input_t input)
	{
		float4 localPosition = float4(input.localPosition, 1);

		float4 worldPosition = mul(ModelMatrix, localPosition);

		float4 viewPosition = mul(ViewMatrix, worldPosition);

		float4 clipPosition = mul(ProjectionMatrix, viewPosition);

		v2p_t v2p;
		v2p.position = clipPosition;
		v2p.color = input.color;
		v2p.uv = input.uv;
		return v2p;
	}
	
	float4 PixelMain(v2p_t input) : SV_Target0
	{
		float4 textureColor = diffuseTexture.Sample(diffuseSampler, input.uv);
		float4 vertexColor = input.color;
		float4 modelColor = ModelColor;
		float4 color = textureColor * vertexColor * modelColor;
		clip(color.a - 0.001f);
		return float4(color);
	}
)";