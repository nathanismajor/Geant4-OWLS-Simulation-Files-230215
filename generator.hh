#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	
	G4double fAlphaMax = 15*deg;
	G4double theta = 90*deg, phi = 45*deg;
	G4ThreeVector fNewUz = G4ThreeVector(std::sin(theta)*std::cos(phi),
                         std::sin(theta)*std::sin(phi),
                         std::cos(theta));

	virtual void GeneratePrimaries(G4Event*);
	
private:
	G4ParticleGun *fParticleGun;
};

#endif
