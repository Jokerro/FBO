precision mediump float;
varying vec2 v_uvPos;
uniform sampler2D u_tex;

void main()
{
	gl_FragColor =  texture2D(u_tex, v_uvPos);
}
