
#pragma once

#include "ofMain.h"
#include "ofxXmlPoco.h"


class HomographyViewport {

public:
    
    HomographyViewport();
    
    void setup(ofRectangle bounds, string name);
    void setup(float x, float y, float w, float h);
    
	glm::mat4& getTranform(){ return transform; }
    
    vector<glm::vec3>& getCorners(){ return corners; };
    
    bool loadCorners(string path);
    bool saveCorners(string path);
    void resetCorners();
    
    void enableMouse();
    void disableMouse();
    
    void draw();
    
    float getWidth();
    float getHeight();
    
private:
    
    vector<glm::vec3> corners;
    ofRectangle bounds;
    glm::mat4 transform;
    
    // mouse/keyboard interaction:
    bool bEnabled;
    bool bShiftDown;
	glm::vec3* pickedVertex;
    void keyPressed(ofKeyEventArgs& args);
    void keyReleased(ofKeyEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);
    
    // helpers:
    void drawCorners(ofPolyline& corners, const ofMatrix4x4& transform);
    // arrange the corners as a rectagle
    void cornersToRect(glm::vec3* pickedCorner, vector<glm::vec3>& corners);
    
    string name;
    vector<ofColor> cornerColors;
    const float cornerSize = 12;
};
