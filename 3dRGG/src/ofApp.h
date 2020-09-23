#pragma once

#include "ofxGui.h"
#include "ofMain.h"

#include "node.h"

class ofApp : public ofBaseApp {
   ofParameter<float> r;
   ofParameter<int> n;
   ofParameter<int> diameter;
   ofParameter<bool> autoRotate;
   ofParameter<float> autoRotateSpeed;
   ofParameter<bool> showSphere;
   ofParameter<bool> showEdges;
   ofParameter<bool> showMaxMin;
   ofxPanel gui;
   ofImage img;
   vector<node> nodes;
   vector<edge> edges;
   ofQuaternion curRot;
   ofVec2f lastMouse;
   float dampen;
   float formerR;
   bool needUpdate;
   node maxNode;
   node minNode;

public:
   void setup();
   void update();
   void draw();

   void keyPressed(int key);

   void mouseDragged(int x, int y, int button);
   void mousePressed(int x, int y, int button);
   void mouseReleased(int x, int y, int button);

   bool toDrawLine(node a, node b);
   void constructEdges();

   static const int defaultWindowWidth  = 1024;
   static const int defaultWindowHeight = 768;
};
