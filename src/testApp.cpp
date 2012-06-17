#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    
    scattering.load("shaders/lightScattering");
    phong.load("shaders/phong");
    thresholdShader.load("shaders/threshold");
    flareShader.load("shaders/flare");
    blurShader.setup(ofGetWidth(), ofGetHeight());
    modulateTextureShader.load("shaders/modulateTexture");
    
    sunImage.loadImage("sun.png");
    dirtImage.loadImage("lensdirt_highc.tga");
    
    ofFbo::Settings settings;
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    settings.internalformat = GL_RGBA;
    lightRays.allocate(settings);
    thresholdFbo.allocate(settings);
    lensFlareFbo.allocate(settings);
    blurredFbo.allocate(settings);
    
    settings.useDepth = true;
    firstPass.allocate(settings);
    
    modelLoader.loadModel("models/tank/bradle.3ds");
    modelLoader.setRotation(0, 45, 0, 1, 0);
    modelLoader.setRotation(1, -90, 1, 0, 0);
    
    cam.setup();
    cam.setFarClip(100000);
    cam.targetNode.move(0, -200, 1000);
    
    int width = 300;
    int height = 20;
    gui = new ofxUICanvas("SCATTERING");
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 10, exposure, "EXPOSURE"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 1, decay, "DECAY"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 1, density, "DENSITY"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 1, weight, "WEIGHT"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 1, threshR, "THRESH"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0, 100, samples, "SAMPLES"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0., 1., flareDispersal, "FLAREDISPERSAL"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0., 1000., flareHaloWidth, "FLAREHALOWIDTH"));
    
    gui->addWidgetDown(new ofxUISlider(width, 0, 0., 1., flareChromaticDistortion.x, "FLARECHROMATICR"));
    gui->addWidgetDown(new ofxUISlider(width, 0, 0., 1., flareChromaticDistortion.y, "FLARECHROMATICG"));
    gui->addWidgetDown(new ofxUISlider(width, 0, 0., 1., flareChromaticDistortion.z, "FLARECHROMATICB"));
    
    gui->addWidgetDown(new ofxUISlider(width, height, 0., 20., blurAmount, "BLURAMOUNT"));
    gui->addWidgetDown(new ofxUISlider(width, height, 0., 20., dirtAmount, "DIRTAMOUNT"));
    
    gui->addWidgetDown(new ofxUI2DPad(0, 0, width / 2.0, width / 2.0, ofPoint(0, ofGetWidth()), ofPoint(0, ofGetHeight()), lightPosition, "LIGHTPOSITION"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("settings.xml");
}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("settings.xml");
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e) {
    string title = e.widget->getName();
    if(title == "EXPOSURE")
        exposure = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "DECAY")
        decay = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "DENSITY")
        density = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "WEIGHT")
        weight = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "LIGHTPOSITION")
        lightPosition = ((ofxUI2DPad *)e.widget)->getScaledValue();

    if(title == "THRESH"){
        threshR = ((ofxUISlider *)e.widget)->getScaledValue();
        threshG = ((ofxUISlider *)e.widget)->getScaledValue();
        threshB = ((ofxUISlider *)e.widget)->getScaledValue();
    }
    
    if(title == "THRESHR")
        threshR = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "THRESHG")
        threshG = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "THRESHB")
        threshB = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "SAMPLES")
        samples = roundf(((ofxUISlider *)e.widget)->getScaledValue());
    if(title == "FLAREDISPERSAL")
        flareDispersal = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "FLAREHALOWIDTH")
        flareHaloWidth = ((ofxUISlider *)e.widget)->getScaledValue();
    
    if(title == "FLARECHROMATICR")
        flareChromaticDistortion.x = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "FLARECHROMATICG")
        flareChromaticDistortion.y = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "FLARECHROMATICB")
        flareChromaticDistortion.z = ((ofxUISlider *)e.widget)->getScaledValue();
    
    if(title == "BLURAMOUNT")
        blurAmount = ((ofxUISlider *)e.widget)->getScaledValue();
    if(title == "DIRTAMOUNT")
        dirtAmount = ((ofxUISlider *)e.widget)->getScaledValue();
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    glEnable(GL_DEPTH_TEST);
    
    firstPass.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0);
        cam.begin();  
    
        ofPushMatrix();
        ofTranslate(-2500, -4000, -5000);
        ofSetColor(255);
        sunImage.draw(0, 0, 2048, 2048);
        ofPopMatrix();
    
            phong.begin();            
            phong.setUniform1i("useTex", 1);
            phong.setUniform3f("lightPos", -2, 3, -2);
                modelLoader.drawFaces();
            phong.end();
        cam.end();
    firstPass.end();
    
    lightRays.begin();
    ofClear(0);
        scattering.begin();
        scattering.setUniform1f("exposure", exposure);
        scattering.setUniform1f("decay", decay);
        scattering.setUniform1f("density", density);
        scattering.setUniform1f("weight", weight);
        scattering.setUniform2f("lightPositionOnScreen", lightPosition.x, lightPosition.y);
        scattering.setUniformTexture("firstPass", firstPass, 0);
            glBegin(GL_QUADS);
                int x = 0;
                int y = 0;
                glTexCoord2f(x, y);
                glVertex2f(x, y);
            
                x = ofGetWidth();
                glTexCoord2f(x, y);
                glVertex2f(x, y);
            
                y = ofGetHeight();
                glTexCoord2f(x, y);
                glVertex2f(x, y);
                
                x = 0;
                glTexCoord2f(x, y);
                glVertex2f(x, y);
            
            glEnd();
        scattering.end();
    lightRays.end();
    
    thresholdFbo.begin();
    ofClear(0);    
        thresholdShader.begin();
        thresholdShader.setUniformTexture("tex0", firstPass, 0);
        thresholdShader.setUniform3f("thresholdRGB", threshR, threshG, threshB);
        thresholdShader.setUniform2f("dimensions", ofGetWidth(), ofGetHeight());
        thresholdShader.setUniform1i("flipXY", 1);
            firstPass.draw(0, 0);
        thresholdShader.end();    
    thresholdFbo.end();
    
    lensFlareFbo.begin();
    ofClear(0);
        flareShader.begin();
        flareShader.setUniformTexture("tex0", thresholdFbo, 0);
        flareShader.setUniform2f("dimensions", ofGetWidth(), ofGetHeight());
        flareShader.setUniform1i("nSamples", samples);
        flareShader.setUniform1f("flareDispersal", flareDispersal);
        flareShader.setUniform1f("flareHaloWidth", flareHaloWidth);
        flareShader.setUniform3fv("flareChromaticDistortion", flareChromaticDistortion.getPtr());
            thresholdFbo.draw(0, 0);
        flareShader.end();
    lensFlareFbo.end();
    
    blurredFbo.begin();
    ofClear(0);
        blurShader.begin(blurAmount);
            lensFlareFbo.draw(0, 0);
        blurShader.end();
    blurredFbo.end();
    
    lensFlareFbo.begin();
    ofClear(0);
        modulateTextureShader.begin();
        modulateTextureShader.setUniformTexture("tex0", blurredFbo, 0);
        modulateTextureShader.setUniformTexture("tex1", dirtImage, 1);
        modulateTextureShader.setUniform2f("tex0dim", blurredFbo.getWidth(), blurredFbo.getHeight());
        modulateTextureShader.setUniform2f("tex1dim", dirtImage.getWidth(), dirtImage.getHeight());
        modulateTextureShader.setUniform1f("amount", dirtAmount);
            blurredFbo.draw(0, 0);
        modulateTextureShader.end();
    lensFlareFbo.end();
    
    ofEnableAlphaBlending();
    glDisable(GL_DEPTH_TEST);
    lensFlareFbo.draw(0, 0);
    lightRays.draw(0, 0);
    firstPass.draw(0, 0);
}