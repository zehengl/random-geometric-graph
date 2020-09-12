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


void ofApp::setup()
{
   r.set("r", 0.06, 0.01, 1);
   n.set("n", 100, 10, 10000);
   w.set("w", 512, 128, 1024);
   showEdges.set("show edges", false);
   showMaxMin.set("show node with max/min degree", false);

   gui.setup();
   gui.add(r);
   gui.add(n);
   gui.add(w);
   gui.add(showEdges);
   gui.add(showMaxMin);

   ofSetWindowTitle("2D Random Geometric Graph");

   int windowsX = ofGetScreenWidth() / 2 - defaultWindowWidth / 2;
   int windowsY = ofGetScreenHeight() / 2 - defaultWindowHeight / 2;
   ofSetWindowPosition(windowsX, windowsY);

   nodes.assign(n, node());
   int id = 0;
   for (node& p:nodes)
   {
      p.id = id++;
      p.x  = ofRandom(0, 1);
      p.y  = ofRandom(0, 1);
   }

   constructEdges();
   formerR    = r;
   needUpdate = false;
}


void ofApp::update()
{
   if (!needUpdate & (r != formerR | n != int(nodes.size())))
   {
      needUpdate = true;
   }

   if (n > int(nodes.size()))
   {
      for (int i = nodes.size(); i < n; i++)
      {
         node p;
         p.id = i;
         p.x  = ofRandom(0, 1);
         p.y  = ofRandom(0, 1);
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

   ofSetColor(ofColor::white);
   for (node n: nodes)
   {
      ofDrawCircle(translateX(n.x), translateY(n.y), 1);
   }

   if (showEdges)
   {
      ofEnableAlphaBlending();

      ofSetColor(ofColor::peru, 25);
      for (edge e : edges)
      {
         ofDrawLine(translateX(e.from->x), translateY(e.from->y), translateX(e.to->x), translateY(e.to->y));
      }

      ofDisableAlphaBlending();
   }

   if (showMaxMin)
   {
      ofEnableAlphaBlending();

      ofSetColor(ofColor::darkBlue, 50);
      ofDrawCircle(translateX(maxNode.x), translateY(maxNode.y), r * w);

      ofSetColor(ofColor::red, 50);
      ofDrawCircle(translateX(minNode.x), translateY(minNode.y), r * w);

      ofDisableAlphaBlending();

      ofSetColor(ofColor::white);
      ofDrawBitmapString("Max Degree = " + to_string(maxNode.degree), ofGetScreenWidth() * 0.05, translateY(maxNode.y));
      ofDrawBitmapString("Min Degree = " + to_string(minNode.degree), ofGetScreenWidth() * 0.05, translateY(minNode.y));
   }
}


void ofApp::keyPressed(int key)
{
   if (key == OF_KEY_RETURN)
   {
      for (node& n : nodes)
      {
         n.x      = ofRandom(0, 1);
         n.y      = ofRandom(0, 1);
         n.degree = 0;
         n.neighbors.clear();
      }
      needUpdate = true;
   }

   if (key == 'x')
   {
      img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
      img.save("screenshot.png");
   }
}


float ofApp::translateX(float x)
{
   return x * w + ofGetWidth() / 2 - w / 2;
}


float ofApp::translateY(float y)
{
   return y * w + ofGetHeight() / 2 - w / 2;
}


bool ofApp::toDrawLine(node a, node b)
{
   return pow(a.x - b.x, 2) + pow(a.y - b.y, 2) < pow(r, 2);
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
