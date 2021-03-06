#version 330

uniform float Radius;
uniform float Size;
uniform float QuadSize;

in float f_Depth;
in vec3 f_Normal;
in vec2 f_PositionLocal;
in vec2 f_PositionNormalized;
in vec3 f_PositionVS;

out vec3 o_Color;
//out vec4 o_NormalDepth;
//out vec3 o_PositionVS;

void main()
{
    float normDistance = (Radius - length(f_PositionLocal)) / Radius;
    float factor = clamp(normDistance * 15, 0, 1);

	int qX = int((f_PositionNormalized.x * Size) / QuadSize);
	int qY = int((f_PositionNormalized.y * Size) / QuadSize);
	
	o_Color = ((qX + (qY % 2)) % 2 == 0 ? vec3(0.9f) - vec3(0.2f) * factor : vec3(0.9f));
//	o_NormalDepth = vec4(f_Normal, f_Depth);
//	o_PositionVS = f_PositionVS;
}

