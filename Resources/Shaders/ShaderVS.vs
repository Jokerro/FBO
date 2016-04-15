attribute vec3 a_posL;
attribute vec2 a_uvPos;
varying vec2 v_uvPos;
uniform mat4 u_matT;

void main()
{
gl_Position = u_matT*vec4(a_posL, 1.0);
v_uvPos = a_uvPos;
}