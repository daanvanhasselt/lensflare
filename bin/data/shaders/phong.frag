varying vec3 normal;
varying vec3 toLight;

uniform sampler2DRect tex0;
varying vec2 texCoords;

uniform int useTex;

void main(){
    vec4 ambientColor;
    vec4 diffuseColor;
    if(useTex == 1){
        ambientColor = mix(texture2DRect(tex0, texCoords), vec4(0., 0., 0., 1.), 0.7);
        diffuseColor = texture2DRect(tex0, texCoords);
    }
    else{
        ambientColor = vec4(0.2, 0.2, 0.2, 1.0);
        diffuseColor = vec4(0.5, 0.2, 0.2, 1.0);
    }
    
    vec3 normalizedNormal = normalize(normal);
    vec3 normalizedToLight = normalize(toLight);
    
    float diffuseTerm = abs( dot(normalizedNormal, normalizedToLight));
    
    float numberOfLevels = 255.0;
    diffuseTerm *= numberOfLevels;
    int diffuseTermRounded = int(diffuseTerm);
    diffuseTerm = float(diffuseTermRounded) / numberOfLevels;
    gl_FragColor = ambientColor + diffuseColor * diffuseTerm;
}