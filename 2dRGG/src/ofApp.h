#pragma once

#include "ofxGui.h"
#include "ofMain.h"

#include "node.h"

class ofApp : public ofBaseApp {
   ofParameter<float> r;
   ofParameter<int> n;
   ofParameter<int> w;
   ofParameter<bool> showEdges;
   ofParameter<bool> showMaxMin;
   ofxPanel gui;
   ofImage img;
   vector<node> nodes;
   vector<edge> edges;
   float formerR;
   bool needUpdate;
   node maxNode;
   node minNode;

public:
   void setup();
   void update();
   void draw();
   void keyPressed(int key);

   float translateX(float x);
   float translateY(float y);
   bool toDrawLine(node a, node b);
   void constructEdges();

   static const int defaultWindowWidth  = 1024;
   static const int defaultWindowHeight = 768;
};
