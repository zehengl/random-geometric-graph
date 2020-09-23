#include "ofApp.h"

bool compareNodeX(node i, node j)
{
   return i.x < j.x;
}


bool compareNodeId(node i, node j)
{
   return i.id < j.id;
}


bool compareNodeDegree(node i, node j)
{
   return i.degree < j.degree;
}


void createNode(float& x, float& y, float& z)
{
   x = ofRandom(-1, 1);
   y = ofRandom(-1, 1);
   z = ofRandom(-1, 1);
   float norm = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
   x = x / norm;
   y = y / norm;
   z = z / norm;
}


void ofApp::setup()
{
   r.set("r", 0.06, 0.01, 1);
   n.set("n", 100, 10, 5000);
   diameter.set("diameter", 512, 128, 1024);
   autoRotate.set("auto rotate", true);
   autoRotateSpeed.set("auto rotate speed", 0.1, 0.1, 5);
   showSphere.set("show sphere", true);
   showEdges.set("show edges", false);
   showMaxMin.set("show max/min", false);

   gui.setup();
   gui.add(r);
   gui.add(n);
   gui.add(diameter);
   gui.add(autoRotate);
   gui.add(autoRotateSpeed);
   gui.add(showSphere);
   gui.add(showEdges);
   gui.add(showMaxMin);

   ofSetWindowTitle("3D Random Geometric Graph");

   int windowsX = ofGetScreenWidth() / 2 - defaultWindowWidth / 2;
   int windowsY = ofGetScreenHeight() / 2 - defaultWindowHeight / 2;
   ofSetWindowPosition(windowsX, windowsY);
   ofSetFrameRate(30);
   nodes.assign(n, node());
   int id = 0;
   for (node& p:nodes)
   {
      p.id = id++;
      createNode(p.x, p.y, p.z);
   }

   constructEdges();
   formerR    = r;
   needUpdate = false;

   dampen = .5;
}


void ofApp::update()
{
   if (!needUpdate & (r != formerR | n != int(nodes.size())))
   {
      needUpdate = true;
   }

   if (autoRotate)
   {
      ofQuaternion yRot(autoRotateSpeed * dampen, ofVec3f(0, 1, 0));
      curRot *= yRot;
   }

   if (n > int(nodes.size()))
   {
      for (int i = nodes.size(); i < n; i++)
      {
         node p;
         p.id = i;
         createNode(p.x, p.y, p.z);
         nodes.push_back(p);
      }
   }
   else if (n < int(nodes.size()))
   {
      sort(nodes.begin(), nodes.end(), compareNodeId);
      nodes.erase(nodes.begin() + n, nodes.end());
   }

   if ((showEdges | showMaxMin) & needUpdate)
   {
      constructEdges();
      needUpdate = false;
   }

   formerR = r;
}


void ofApp::draw()
{
   gui.draw();

   ofPushMatrix();

   ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, -diameter / 2);

   ofVec3f axis;
   float   angle;
   curRot.getRotate(angle, axis);
   ofRotateDeg(angle, axis.x, axis.y, axis.z);

   if (showSphere)
   {
      ofEnableAlphaBlending();
      ofSetColor(ofColor::black, 15);
      ofNoFill();
      ofDrawSphere(0, 0, 0, diameter / 2);
      ofDisableAlphaBlending();
   }


   ofSetColor(ofColor::white);
   for (node p: nodes)
   {
      ofDrawSphere(p.x * diameter / 2, p.y * diameter / 2, p.z * diameter / 2, 1);
   }

   if (showEdges)
   {
      ofEnableAlphaBlending();

      ofSetColor(ofColor::peru, 25);
      for (edge e : edges)
      {
         ofDrawLine(
            e.from->x * diameter / 2, e.from->y * diameter / 2, e.from->z * diameter / 2,
            e.to->x * diameter / 2, e.to->y * diameter / 2, e.to->z * diameter / 2);
      }

      ofDisableAlphaBlending();
   }

   if (showMaxMin)
   {
      ofEnableAlphaBlending();

      ofSetColor(ofColor::darkBlue, 50);
      ofNoFill();
      ofDrawSphere(
         maxNode.x * diameter / 2,
         maxNode.y * diameter / 2,
         maxNode.z * diameter / 2, 10);

      ofSetColor(ofColor::red, 50);
      ofNoFill();
      ofDrawSphere(
         minNode.x * diameter / 2,
         minNode.y * diameter / 2,
         minNode.z * diameter / 2, 10);

      ofDisableAlphaBlending();

      ofSetColor(ofColor::white);
      ofDrawBitmapString(
         "Max Degree = " + to_string(maxNode.degree),
         maxNode.x * diameter / 2,
         maxNode.y * diameter / 2,
         maxNode.z * diameter / 2);
      ofDrawBitmapString(
         "Min Degree = " + to_string(minNode.degree),
         minNode.x * diameter / 2,
         minNode.y * diameter / 2,
         minNode.z * diameter / 2);
   }

   ofPopMatrix();
}


bool ofApp::toDrawLine(node a, node b)
{
   return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)) < r;
}


void ofApp::constructEdges()
{
   sort(nodes.begin(), nodes.begin() + n, compareNodeX);

   edges.clear();

   for (node& n:nodes)
   {
      n.degree = 0;
      n.neighbors.clear();
   }

   for (int i = 0; i < n; i++)
   {
      for (int j = i + 1; j < n; j++)
      {
         if (nodes[j].x - nodes[i].x <= r)
         {
            if (toDrawLine(nodes[i], nodes[j]))
            {
               edges.push_back(edge(nodes[i], nodes[j]));
               nodes[i].degree++;
               nodes[j].degree++;
               nodes[i].neighbors.push_back(nodes[j].id);
               nodes[j].neighbors.push_back(nodes[i].id);
            }
         }
         else
         {
            break;
         }
      }
   }

   auto [min, max] = minmax_element(nodes.begin(), nodes.end(), compareNodeDegree);

   maxNode = *max;
   minNode = *min;
}


void ofApp::keyPressed(int key)
{
   if (key == OF_KEY_RETURN)
   {
      for (node& p : nodes)
      {
         createNode(p.x, p.y, p.z);
         p.degree = 0;
         p.neighbors.clear();
      }
   }

   if (key == 'x')
   {
      img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
      img.save("screenshot.png");
   }
}


void ofApp::mouseDragged(int x, int y, int button)
{
   ofVec2f      mouse(x, y);
   ofQuaternion yRot((x - lastMouse.x) * dampen, ofVec3f(0, 1, 0));
   ofQuaternion xRot((y - lastMouse.y) * dampen, ofVec3f(-1, 0, 0));

   curRot    *= yRot * xRot;
   lastMouse  = mouse;
   autoRotate = false;
}


void ofApp::mousePressed(int x, int y, int button)
{
   lastMouse = ofVec2f(x, y);
}


void ofApp::mouseReleased(int x, int y, int button)
{
   autoRotate = true;
}
