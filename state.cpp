// state.cpp

#include "headers.h"

#include "state.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;

  gpuProgram->activate();

  glUniformMatrix4fv( glGetUniformLocation( gpuProgram->id(), "M"), 1, GL_TRUE, &M[0][0] );

  silos[0].draw( gpuProgram );
  
  for (i=0; i<silos.size(); i++)
    silos[i].draw( gpuProgram );

  for (i=0; i<cities.size(); i++)
    cities[i].draw( gpuProgram );

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw( gpuProgram );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].drawOut( gpuProgram );

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw( gpuProgram );

  gpuProgram->deactivate();
}


// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


void State::updateState( float deltaT )

{
  int i;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missile generation
  // should increase with time.
  //
  // CHANGE THIS
  float rateInc = randIn01()<0.009 + currentTime/1000;	//Increase rate of generation
  //cout << randIn01() << endl;

  if (rateInc) {	// New missile 

	float randXDest = randIn01();	// Generate random x destination
    missilesIn.add( Missile( vec3( randIn01(), worldTop, 0), // source
			     vec3( -0.02*randXDest, -0.1, 0 ),         // velocity
			     0,                              // destination y
			     vec3( 1,1,0 ) ) );              // colour
  }

  // Look for terminating missiles

  for (i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
	  // CHANGE THIS: ADD AN EXPLOSION
	  explosions.add(Circle(vec3(missilesIn[i].position()), 0.5, 0.05, vec3(1.0, 1.0, 0.0)));
      missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
	  explosions.add(Circle(vec3(missilesOut[i].position()), 0.5, 0.05, vec3(1.0, 1.0, 0.0)));
      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (i=0; i<explosions.size(); i++)
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO

		// Check cities
		for (int j = 0; j < cities.size(); j++) {
			if (cities[j].isHit(explosions[i].position(), explosions[i].maxRadius())) {
				cities.remove(j);
			}
		}

		// Check silos
		for (int k = 0; k < silos.size(); k++) {
			if (silos[k].siloHit(explosions[i].position(), explosions[i].maxRadius())) {
				silos.remove(k);
			}
		}

	  explosions.remove(i);
      i--;
    }

  // Look for incoming missiles that hit an explosion and are
  // destroyed

  for (i = 0; i < explosions.size(); i++) {
	  if (explosions[i].radius() <= explosions[i].maxRadius())
		  for (int j = 0; j < missilesIn.size(); j++) {
			  if (missilesIn[j].missileHit(explosions[i].position(), explosions[i].radius())) {
				  missilesIn.remove(j);
			  }
		 
  }
  }
  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expandContract( deltaT );
}


// Fire a missile

void State::fireMissile( int siloIndex, float x, float y)

{
  const float speed = 0.3;
    
  if (silos[siloIndex].canShoot()) {
	
    silos[siloIndex].decrMissiles();

    // CHANGE THIS
    missilesOut.add( Missile( silos[siloIndex].position(),           // source
			      speed *(vec3(x,y,0)-silos[siloIndex].position()), // velocity
			      worldTop,								 // destination y
			      vec3( 0,1,0 ) ) );                     // colour
  }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vec3( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vec3( 0.2, 0, 0 ) ) );
  cities.add( City( vec3( 0.3, 0, 0 ) ) );
  cities.add( City( vec3( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vec3( 0.6, 0, 0 ) ) );
  cities.add( City( vec3( 0.7, 0, 0 ) ) );
  cities.add( City( vec3( 0.8, 0, 0 ) ) );
}
