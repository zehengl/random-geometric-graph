#ifndef NODE_H
#define NODE_H

struct node
{
public:
   float       x;
   float       y;
   float       z;
   int         degree;
   int         id;
   vector<int> neighbors;

   node()
   {
      x      = 0;
      y      = 0;
      z      = 0;
      degree = 0;
      id     = 0;
   }
};

struct edge
{
public:
   node *from;
   node *to;

   edge(node& a, node& b)
   {
      from = &a;
      to   = &b;
   }
};

#endif
