//
//  Util.h
//  TemplateProject
//
//  Created by xuzebin on 10/21/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Util_h
#define Util_h

//print average fps every 30 frames.
void printAverageFPS() {
    static int count = 10;
    static float lastTime;
    if (count == 30) {
        lastTime = glutGet(GLUT_ELAPSED_TIME);
    }
    if (count-- == 0) {
        count = 30;
        std::cout << "fps: " << 30.0 / ((glutGet(GLUT_ELAPSED_TIME) - lastTime) / 1000.0) << std::endl;
    }
}


#endif /* Util_h */
