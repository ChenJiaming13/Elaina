#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 uLightMatrices[6];

out vec4 vFragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for (int Face = 0; Face < 6; ++Face)
    {
        gl_Layer = Face; // built-in variable that specifies to which face we render.
        for (int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            vFragPos = gl_in[i].gl_Position;
            gl_Position = uLightMatrices[Face] * vFragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 