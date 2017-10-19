#include "ofApp.h"

//--------------------------------------------------------------


void ofApp::setup(){
    
    intervals = ofGetWindowHeight()/30;
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	leap.open();
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    for(int i = 0; i<100; i++){
        leftPoints[i] = ofVec3f(-ofGetWindowWidth()/2,i*intervals-2*ofGetWindowHeight()/3,0);
        rightPoints[i] = ofVec3f(ofGetWindowWidth()/2,i*intervals-2*ofGetWindowHeight()/3,0);
    }
}


//--------------------------------------------------------------
void ofApp::update(){
	fingersFound.clear();
	
	//here is a simple example of getting the hands and drawing each finger and joint
	//the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
	
	//if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method.
	//there you can work with the frame data directly.
    
    
    
    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms.
    
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
        }
        
        float distX = 0;
        float distY = 0;
        float distZ = 0;
        float distance = 0;
        
        if(simpleHands.size()==2){
            if(simpleHands[0].isLeft){
                distX = simpleHands[1].handPos.x-simpleHands[0].handPos.x;
                distY = simpleHands[1].handPos.y-simpleHands[0].handPos.y;
                distZ = simpleHands[1].handPos.z-simpleHands[0].handPos.z;
            }else{
                distX = simpleHands[0].handPos.x-simpleHands[1].handPos.x;
                distY = simpleHands[0].handPos.y-simpleHands[1].handPos.y;
                distZ = simpleHands[0].handPos.z-simpleHands[1].handPos.z;
            }
            distance = ofDist(simpleHands[1].handPos.x,simpleHands[1].handPos.y,simpleHands[1].handPos.z,simpleHands[0].handPos.x,simpleHands[0].handPos.y,simpleHands[0].handPos.z);
            
            colorChangeR = abs(distX)/30;
            colorChangeB = abs(distY)/30;
            colorChangeG = abs(distZ)/30;
            
            intervals = ofGetWindowHeight()/100+distance/20;
            distY*=5;
            distZ*=20;
            
            for(int i =0; i<100; i++){
                leftPoints[i] = ofVec3f(-ofGetWindowWidth()/2-distX,i*intervals-2*ofGetWindowHeight()/3-distY/2,-distZ/2);
                rightPoints[i] = ofVec3f(ofGetWindowWidth()/2+distX,i*intervals-2*ofGetWindowHeight()/3+distY/2,+distZ/2);
            }
            
        }
    }
    
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
	cam.begin();
    cam.setOrientation(ofPoint(0,0,0));
    
//	ofPushMatrix();
//    ofRotate(90, 0, 0, 1);
//    ofSetColor(20);
//    ofDrawGridPlane(800, 20, false);
//	ofPopMatrix();
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
//        ofSetColor(0, 0, 255);
//        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
//        ofSetColor(255, 255, 0);
//        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(255);
//            ofDrawSphere(tip.x, tip.y, tip.z, 4);
        }
        
    }
    
    
    for(int i = 0; i<100; i++){
        ofSetColor(i*colorChangeR, i*colorChangeB, i*colorChangeG);
        ofSetLineWidth(3);
        ofDrawLine(leftPoints[i], rightPoints[i]);
    }
    
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
