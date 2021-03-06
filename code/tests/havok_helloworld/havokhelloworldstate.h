#pragma once
//------------------------------------------------------------------------------
/**
    @class Havok::HavokHelloWorldState
    
    Dummy state to run a havok application.
    
    (C) 2013 gscept
*/
#include "basegamefeature/statehandlers/gamestatehandler.h"


namespace Physics
{
	class PhysicsServer;
	class PhysicsBody;
	class Scene;
}

namespace Game
{
	class Entity;
}

//------------------------------------------------------------------------------
namespace Tests
{
class HavokHelloWorldState : public BaseGameFeature::GameStateHandler
{
	__DeclareClass(HavokHelloWorldState);
public:
	/// constructor
	HavokHelloWorldState();
	/// destructor
	virtual ~HavokHelloWorldState();

	/// called when the state represented by this state handler is entered
	void OnStateEnter(const Util::String& prevState);
	/// called when the state represented by this state handler is left
	void OnStateLeave(const Util::String& nextState);
	/// called each frame as long as state is current, return new state
	Util::String OnFrame();

private:

	/// initialize physics stuff
	void InitPhysicsStuff();
	/// close physics stuff
	void ClosePhysicsStuff();

	Ptr<Physics::PhysicsServer> physicsServer;	//< handles the simulation
	Physics::Scene* physicsWorld;				//< the physics world

	Ptr<Physics::PhysicsBody> testPhysicsBody;			// test physics body

	Ptr<Game::Entity> camera;
}; 
} 
// namespace Havok
//------------------------------------------------------------------------------