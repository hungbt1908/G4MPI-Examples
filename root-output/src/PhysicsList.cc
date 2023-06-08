#include "PhysicsList.hh"
#include "G4EmStandardPhysics_option3.hh"

PhysicsList::PhysicsList() 
: G4VModularPhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics_option3);
}

PhysicsList::~PhysicsList()
{}