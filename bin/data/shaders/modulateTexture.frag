uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform vec2 tex0dim;
uniform vec2 tex1dim;

uniform float amount;

varying vec2 texCoords;

void main(){
    vec4 originalColor = texture2DRect(tex0, texCoords);
    vec4 secondaryColor = texture2DRect(tex1, texCoords / tex0dim * tex1dim);
    gl_FragColor = vec4(originalColor.rgb * secondaryColor.rgb * vec3(amount), originalColor.a);
}