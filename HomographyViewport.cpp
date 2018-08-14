
#include "HomographyViewport.h"


HomographyViewport::HomographyViewport(){
    // corners defined clockwise from top-left
    // coords are normalized and relative to bounds
    corners.push_back(ofPoint(0,0));
    corners.push_back(ofPoint(1,0));
    corners.push_back(ofPoint(1,1));
    corners.push_back(ofPoint(0,1));
    
    // colors
    cornerColors.push_back(ofColor::red);
    cornerColors.push_back(ofColor::green);
    cornerColors.push_back(ofColor::blue);
    cornerColors.push_back(ofColor::turquoise);
    
    bShiftDown = false;
    
    name = "viewport";
}


void HomographyViewport::setup(ofRectangle _bounds, string _name){
    setup(_bounds.x, _bounds.y, _bounds.width, _bounds.height);
    name = _name;
}


void HomographyViewport::setup(float x, float y, float w, float h){
    bounds.set(x,y,w,h);
    
    transform.makeIdentityMatrix();
    transform.postMultScale(bounds.getWidth(), bounds.getHeight(), 1);
    transform.postMultTranslate(bounds.position);
    
    resetCorners();
    
    enableMouse();
}


void HomographyViewport::resetCorners(){
    corners[0].set(0,0);
    corners[1].set(1,0);
    corners[2].set(1,1);
    corners[3].set(0,1);
}


bool HomographyViewport::loadCorners(string path){
    ofXml xml;
    if(xml.load(path)){
        if(xml.exists("source")){
            xml.setTo("source");
            int count = xml.getNumChildren();
            for (int i=0; i<count; i++) {
                xml.setTo("corner[" + ofToString(i) + "]");
                float x = xml.getValue<float>("x");
                float y = xml.getValue<float>("y");
                
                corners[i].set(x,y);
                
                xml.setTo("../");
            }
            return true;
        }
    }
    return false;
}


bool HomographyViewport::saveCorners(string path){
    ofXml xml;
    xml.addChild("corners");
    xml.setTo("//corners");
    for(int i=0; i<corners.size(); i++){
        
        ofPoint& corner = corners[i];
        
        xml.setTo("//corners");
        xml.addChild("corner");
        xml.setTo("corner[" + ofToString(i) + "]");
        xml.addValue("x", corner.x);
        xml.addValue("y", corner.y);
    }
    return xml.save(path);
}


void HomographyViewport::draw(){
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);

    // bounds
//    ofSetColor(ofColor::white);
//    ofRect(bounds);
    
    // outline
    ofSetColor(ofColor::yellow);
    ofPolyline outline(corners);
    outline.setClosed(true);
    ofPushMatrix();
    ofMultMatrix(transform);
    outline.draw();
    ofPopMatrix();
    
    // corners
    ofSetRectMode(OF_RECTMODE_CENTER);
    for(int i=0; i<corners.size(); i++){
        ofSetColor(cornerColors[i]);
        ofRect(corners[i] * transform, cornerSize,cornerSize);
    }
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    //ofDrawBitmapStringHighlight(name, bounds.getBottomLeft()+ofPoint(5,20), ofColor::darkGrey);
    
    ofPopStyle();
}


void HomographyViewport::enableMouse(){
    if(bEnabled) return;
    ofAddListener(ofEvents().mouseReleased, this, &HomographyViewport::mouseReleased);
    ofAddListener(ofEvents().mousePressed, this, &HomographyViewport::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &HomographyViewport::mouseDragged);
    ofAddListener(ofEvents().keyPressed, this, &HomographyViewport::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &HomographyViewport::keyReleased);
    bEnabled = true;
    pickedVertex = NULL;
}


void HomographyViewport::disableMouse(){
    if(!bEnabled) return;
    ofRemoveListener(ofEvents().mouseReleased, this, &HomographyViewport::mouseReleased);
    ofRemoveListener(ofEvents().mousePressed, this, &HomographyViewport::mousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &HomographyViewport::mouseDragged);
    ofRemoveListener(ofEvents().keyPressed, this, &HomographyViewport::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &HomographyViewport::keyReleased);
    bEnabled = false;
}


void HomographyViewport::keyPressed(ofKeyEventArgs& args){
    if(OF_KEY_SHIFT)
        bShiftDown = true;
}


void HomographyViewport::keyReleased(ofKeyEventArgs& args){
    if(OF_KEY_SHIFT)
        bShiftDown = false;
}


void HomographyViewport::mouseDragged(ofMouseEventArgs& args){
    if(pickedVertex){
        ofPoint mouse(args);
        pickedVertex->set(mouse * transform.getInverse());
        
        // rectangle mode        
        if(bShiftDown){
            cornersToRect(pickedVertex, corners);
        }
    }
}


void HomographyViewport::mousePressed(ofMouseEventArgs& args){
    ofPoint mouse(args);
    for(int i = 0; i < 4; i++){
        ofPoint v = corners[i] * transform;
        if(v.distance(mouse) < cornerSize){
            pickedVertex = &corners[i];
            break;
        }
    }
}


void HomographyViewport::mouseReleased(ofMouseEventArgs& mouse){
    pickedVertex = NULL;
}


void HomographyViewport::cornersToRect(ofPoint* pickedCorner, vector<ofPoint>& corners){
    int pickedIndex = -1;
    for(int i=0; i<4; i++){
        if(&corners[i] == pickedCorner)
            pickedIndex = i;
    }
    // recalculate rectangle
    float left   = min(corners[0].x, corners[3].x);
    float top    = min(corners[0].y, corners[1].y);
    float right  = max(corners[1].x, corners[2].x);
    float bottom = max(corners[2].y, corners[3].y);
    if(pickedIndex == 0){
        left   = corners[0].x;
        top    = corners[0].y;
    }
    else if(pickedIndex == 1){
        right  = corners[1].x;
        top    = corners[1].y;
    }
    else if(pickedIndex == 2){
        right  = corners[2].x;
        bottom = corners[2].y;
    }
    else if(pickedIndex == 3){
        left   = corners[3].x;
        bottom = corners[3].y;
    }
    // update corners
    corners[0].set(left, top);
    corners[1].set(right, top);
    corners[2].set(right, bottom);
    corners[3].set(left, bottom);
}
