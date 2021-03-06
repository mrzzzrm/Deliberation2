#version 330

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Transform;
uniform float FarPlaneZ;

in vec3 Position;
in vec3 Normal;

out vec4 f_NormalDepth;
out vec3 f_PositionVS;

void main()
{
	vec4 positionWS = Transform * vec4(Position, 1.0f);
	vec4 positionVS = View * positionWS;
	gl_Position = Projection * positionVS;
	f_PositionVS = positionVS.xyz;
	f_NormalDepth = vec4((View * Transform * vec4(Normal, 0.0f)).xyz, -positionVS.z/FarPlaneZ);
}

