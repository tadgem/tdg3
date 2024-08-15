#include "physics.h"
#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

using namespace JPH;

// Callback for traces, connect this to your own trace function if you have one

void tdg::physics::init() {
  // Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
  // This needs to be done before any other Jolt function is called.
  RegisterDefaultAllocator();

  // Install trace and assert callbacks
  //Trace = TraceImpl;

  // Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
  // It is not directly used in this example but still required.
  Factory::sInstance = new Factory();

  // Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
  // If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
  // If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
  RegisterTypes();
}
