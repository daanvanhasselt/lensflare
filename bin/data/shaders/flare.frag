uniform sampler2DRect tex0;

varying vec2 texCoord;
uniform vec2 dimensions;
uniform int nSamples;
uniform float flareDispersal;
uniform float flareHaloWidth;
uniform vec3 flareChromaticDistortion;

vec3 textureDistorted(sampler2DRect tex, vec2 sample_center, vec2 sample_vector, vec3 distortion) {
	return vec3(
		texture2DRect(tex, sample_center + sample_vector * distortion.r).r,
		texture2DRect(tex, sample_center + sample_vector * distortion.g).g,
		texture2DRect(tex, sample_center + sample_vector * distortion.b).b
	);
}

void main(){
    vec2 imageCenter = vec2(0.5);
    vec2 sampleVector = (imageCenter - (texCoord / dimensions)) * flareDispersal;
    vec2 haloVector = normalize(sampleVector) * flareHaloWidth;
    
    vec3 color = textureDistorted(tex0, texCoord + haloVector, haloVector, flareChromaticDistortion).rgb * 3.0;
    for(int i = 0; i < nSamples; i++){
        vec2 offset = sampleVector * float(i);
        color += textureDistorted(tex0, texCoord + (offset * dimensions), offset * dimensions, flareChromaticDistortion).rgb;
    }
        
    gl_FragColor = vec4(color, 1.);
}