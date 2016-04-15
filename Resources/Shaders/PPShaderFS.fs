precision mediump float;
varying vec2 v_uvPos;
uniform sampler2D u_tex;

void main()
{
	vec3 color =  texture2D(u_tex, v_uvPos).rgb;
	float gs = (color.r + color.g + color.b)/3.0;
	gl_FragColor = vec4(gs, gs, gs, 1.0);
}