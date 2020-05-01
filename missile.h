// Missile class


#ifndef MISSILE_H
#define MISSILE_H


#include "headers.h"
#include "gpuProgram.h"


class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vec3 p, vec3 v, float y, vec3 c ) {
    pos0 = p;
    pos1 = p;
    velocity = v;
    colour = c;
    destY = y;
  }

  // Draw the missile and its trail

  void draw( GPUProgram *gpuProgram) {

    vec3 verts[2] = {
      vec3( pos0.x, pos0.y, 4.0),
      vec3( pos1.x, pos1.y, 0.0)
    };

    gpuProgram->drawVertices( &verts[0], 2, GL_LINES, colour );
  }

  void drawOut(GPUProgram* gpuProgram) {

	  vec3 verts[2] = {
		vec3(pos0.x, pos0.y, 0.0),
		vec3(pos1.x, pos1.y, 0.0)
	  };

	  gpuProgram->drawVertices(&verts[0], 2, GL_LINES, colour);
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {
	  // In motion
	  if (velocity.y >= 0) {
		  return incomingReached();
	  }
	  else {
		  return outgoingReached();
	  }

  }

  bool incomingReached() {
	  bool isReached = false;
	  if (pos1.y >= destY) {
		  return true;

	  }
	  else {
		  return false;
	  }

  }

  bool outgoingReached() {
	  bool isReached = false;
	  if (pos1.y <= destY) {
		  return true;

	  }
	  else {
		  return false;
	  }

  }

  bool missileHit(vec3 missilePos, float radius) {
	  return (sqrt(((pos1 - missilePos)).squaredLength()) <= radius);
  }

 private:

  vec3  pos0;			// start position
  vec3  pos1;			// current position
  vec3  velocity;		// velocity
  vec3  colour;			// colour of missile trail
  float destY;			// y position at destination
};


#endif
