#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
flat out int v_VertexID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    v_VertexID = gl_VertexID;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}



#type fragment
#version 330 core

flat in int v_VertexID;
out vec4 color;

void main() {
    float r = float(v_VertexID % 100) / 100.0;
    float g = float((v_VertexID / 100) % 100) / 100.0;
    color = vec4(r, g, 0.0, 1.0);
}
