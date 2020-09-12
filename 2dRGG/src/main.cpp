#include "ofMain.h"
#include "ofApp.h"

int main( )
{
   ofSetupOpenGL(ofApp::defaultWindowWidth, ofApp::defaultWindowHeight, OF_WINDOW);

   ofRunApp(new ofApp());
}
