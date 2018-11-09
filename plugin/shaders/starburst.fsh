uniform lowp float qt_Opacity;
uniform lowp vec4 color;

varying lowp float vT;

#define PI 3.14159265359

void main()
{
    lowp float tt = smoothstep(0.0, 1.0, sin(vT * PI));

    gl_FragColor = color * qt_Opacity * tt;
}
