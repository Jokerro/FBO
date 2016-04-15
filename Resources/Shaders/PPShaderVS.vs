attribute vec3 a_posL;
attribute vec2 a_uvPos;
varying vec2 v_uvPos;

void main()
{
gl_Position = vec4(a_posL, 1.0);
v_uvPos = a_uvPos;
}