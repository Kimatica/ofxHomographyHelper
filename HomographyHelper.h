
#pragma once

#include "ofMain.h"

class HomographyHelper {
    
public:
    
    static void findHomography(vector<ofPoint>& src, vector<ofPoint>& dst, ofMatrix4x4& homography);
    static void findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]);
    static void gaussian_elimination(float *input, int n);
};
