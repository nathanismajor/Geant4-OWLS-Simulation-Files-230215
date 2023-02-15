#include "generator.hh"

//#include "PrimaryGeneratorAction2.hh"
//#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
                        
  fAlphaMax = 15*deg; 
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
//particle direction uniform around fNewUz axis
  //
  //1- in World frame      
  //cosAlpha uniform in [cos(0), cos(fAlphaMax)]
  G4double cosAlpha = 1. - G4UniformRand()*(1.- std::cos(fAlphaMax));
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi      = twopi*G4UniformRand();  //psi uniform in [0, 2*pi]  
  
  // Use This for 0 Degree Measurement
  G4ThreeVector dir(cosAlpha,sinAlpha*std::sin(psi),sinAlpha*std::cos(psi));
  G4ThreeVector pos(-0.02025*m,0.,0.);
	//G4ThreeVector mom(1,0.,0.);
  
  //Use This for 90 Degree Measurement
  //G4ThreeVector dir(sinAlpha*std::sin(psi), sinAlpha*std::cos(psi), cosAlpha);
  //G4ThreeVector pos(0.,0.,-0.02025*m);
	//G4ThreeVector mom(0.,0.,1);
  
  //2- rotate dir   (rotateUz transforms uz to fNewUz)
  //dir.rotateUz(fNewUz);           

  fParticleGun->SetParticleMomentumDirection(dir);
  
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="opticalphoton";
	G4ParticleDefinition *particle = particleTable->FindParticle("opticalphoton");
	
	//energies of photons distributed across a spectrum approximated by a Gaussian distribution with mean 430nm and standard deviation 12398nm
	
	G4double particleEnergy = particleEnergy;
	particleEnergy = G4RandGauss::shoot(2.8834*eV, 0.1*eV);
	
	fParticleGun->SetParticlePosition(pos);
	//fParticleGun->SetParticleMomentumDirection(mom);
	//fParticleGun->SetParticleMomentum(2.8834*eV);
	fParticleGun->SetParticleMomentum(particleEnergy);
	fParticleGun->SetParticleDefinition(particle);    

  //create vertex
  //   
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  //set the polarization of the photons
  //fParticleGun->SetParticlePolarization(G4ThreeVector(0,0,0));
   
}
