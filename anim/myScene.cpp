////////////////////////////////////////////////////
// // Template code for  CSC 473
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <shared/defs.h>

#include "shared/opengl.h"

#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"
#include "SampleParticle.h"
#include "SampleGravitySimulator.h"
#include "CircleSimulator.h"
#include "Particle.h"
#include "ParticleSimulator.h"
#include "OscillateSimulator.h"

//#include <util/jama/tnt_stopwatch.h>
//#include <util/jama/jama_lu.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char *) &g_testVariable,
	"",0,(char *) NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo( button, state );

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button press event\n");

	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button release event\n") ;
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo = 
		GlobalResourceManager::use()->getMouseButtonInfo();

	if( updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON )
	{
		animTcl::OutputMessage(
			"My mouse motion callback received a mousemotion event\n") ;
	}

}	// myMotion


void MakeScene(void)
{

	/* 
	
	This is where you instantiate all objects, systems, and simulators and 
	register them with the global resource manager

	*/

	/* SAMPLE SCENE */

	bool success;

	// Create systems
	//SampleParticle* sphere1 = new SampleParticle( "sphere1" );

	ParticleSystem* particles = new ParticleSystem( "particles" );

	// Register systems
	//success = GlobalResourceManager::use()->addSystem( sphere1, true );
	// make sure it was registered successfully
	//assert( success );

	success = GlobalResourceManager::use()->addSystem( particles, true );
	

	// Create simulators
	//SampleGravitySimulator* gravSim1 = new SampleGravitySimulator("grav1", sphere1);


	// Register simulators
	//success = GlobalResourceManager::use()->addSimulator(gravSim1);

	// make sure it was registered successfully
	//assert( success );

	ParticleSimulator* particleSim = new ParticleSimulator("particleSim", particles);


	// Register simulators
	success = GlobalResourceManager::use()->addSimulator(particleSim);

	// make sure it was registered successfully
	assert( success );


	/* END SAMPLE SCENE */

	// the following code shows you how to retrieve a system that was registered 
	// with the resource manager. 

	BaseSystem* sampleSystemRetrieval;

	// retrieve the system
	//sampleSystemRetrieval = 
		//GlobalResourceManager::use()->getSystem( "sphere1" );

	// make sure you got it
	//assert( sampleSystemRetrieval );

	sampleSystemRetrieval = 
		GlobalResourceManager::use()->getSystem( "particles" );

	// make sure you got it
	assert( sampleSystemRetrieval );



	BaseSimulator* sampleSimulatorRetrieval;

	// retrieve the simulator
	sampleSimulatorRetrieval = 
		GlobalResourceManager::use()->getSimulator( "particleSim" );

	// make sure you got it
	assert( sampleSimulatorRetrieval );


}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{
	
	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	 animTcl::OutputMessage("My key callback received a key press event\n");
	return;

}	// myKey

static int testGlobalCommand(ClientData clientData, Tcl_Interp *interp, int argc, myCONST_SPEC char **argv)
{
	 animTcl::OutputMessage("This is a test command!");
    animTcl::OutputResult("100") ;
	return TCL_OK;

}	// testGlobalCommand

void mySetScriptCommands(Tcl_Interp *interp)
{

	// here you can register additional generic (they do not belong to any object) 
	// commands with the shell

	Tcl_CreateCommand(interp, "test", testGlobalCommand, (ClientData) NULL,
					  (Tcl_CmdDeleteProc *)	NULL);

}	// mySetScriptCommands
