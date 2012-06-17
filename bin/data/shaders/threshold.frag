uniform sampler2DRect tex0;
uniform vec3 thresholdRGB;

varying vec2 texCoord;

void main(){
    vec4 color = texture2DRect(tex0, texCoord) - vec4(thresholdRGB, 0.);
    gl_FragColor = color;
}