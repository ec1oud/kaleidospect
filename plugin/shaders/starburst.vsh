attribute highp vec3 vertexCoord;

uniform highp mat4 qt_Matrix;
uniform highp mat4 pmvMatrix;
uniform lowp float qt_Opacity;
uniform lowp vec4 color;

varying lowp float vT;

void main()
{
    highp float r = vertexCoord.x;
    highp float theta = vertexCoord.y;

    gl_Position = qt_Matrix * pmvMatrix * vec4(r * cos(theta), r * sin(theta), 0., 1.);
    vT = vertexCoord.z; // 1 on one side of each stroke, 0 on the other
}
