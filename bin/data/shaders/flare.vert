varying vec2 texCoord;
uniform vec2 dimensions;

void main(){
    texCoord = gl_MultiTexCoord0.st;
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}