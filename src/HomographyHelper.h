
#pragma once

#include "ofMain.h"


class HomographyHelper {
    
public:
    
    static void findHomography(
		vector<glm::vec3>& src, 
		vector<glm::vec3>& dst, 
		glm::mat4& homography
	);

    static void gaussian_elimination(float *input, int n);
};
