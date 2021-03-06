#version 330

uniform Globals
{
    mat4 ViewProjection;
};

uniform mat4 Transform;

in vec3 Position;
in vec3 Color;

out vec3 f_Color;

void main()
{	
	vec4 transformed = ViewProjection * Transform * vec4(Position, 1.0f);

    f_Color = Color;

	gl_Position = transformed;
}

