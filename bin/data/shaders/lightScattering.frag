uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform vec2 lightPositionOnScreen;

uniform sampler2DRect firstPass;

varying vec2 texCoordVar;

const int NUM_SAMPLES = 100;

void main(){
    vec2 texCoord = texCoordVar;
    vec2 deltaTextCoord = vec2( texCoord - lightPositionOnScreen.xy );
    deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        texCoord -= deltaTextCoord;
        vec4 sample = texture2DRect(firstPass, texCoord);
        sample *= illuminationDecay * weight;
        gl_FragColor += sample;
        illuminationDecay *= decay;
    }
    gl_FragColor *= exposure;
}