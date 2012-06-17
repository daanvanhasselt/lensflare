#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxAssimpModelLoader.h"
#include "ofxBlurShader.h"
#include "ofxGameCamera.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    ofShader scattering;
    ofFbo firstPass;
    ofFbo lightRays;

    ofFbo occluded;
    
    ofShader thresholdShader;
    ofFbo thresholdFbo;
    
    ofShader flareShader;
    ofFbo lensFlareFbo;
    int samples;
    float flareDispersal;
    float flareHaloWidth;
    ofPoint flareChromaticDistortion;
    
    ofImage sunImage;
    ofImage dirtImage;

    ofShader modulateTextureShader; 
    float dirtAmount;
    
    ofFbo blurredFbo;
    ofxBlurShader blurShader;
    float blurAmount;
    
    float threshR, threshG, threshB;
    
    ofShader phong;
    
    float exposure;
    float decay;
    float density;
    float weight;
    ofPoint lightPosition;
    
    ofxUICanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
    ofxAssimpModelLoader modelLoader;
    ofxGameCamera cam;
};
