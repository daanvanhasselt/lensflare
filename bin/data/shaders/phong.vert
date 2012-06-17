varying vec3 normal;
varying vec3 toLight;

uniform sampler2DRect tex0;
uniform vec3 lightPos;

varying vec2 texCoords;

void main(){
    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
    
    texCoords = gl_MultiTexCoord0.st;
    
    vec3 projectedLightPos = (gl_ModelViewProjectionMatrix * vec4(lightPos, 1.)).xyz;
    vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
    toLight = vec3(projectedLightPos - pos.xyz);
    gl_Position = pos;
    normal = gl_Normal;
}