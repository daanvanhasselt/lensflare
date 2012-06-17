varying vec2 texCoordVar;

void main(){
    texCoordVar = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}
