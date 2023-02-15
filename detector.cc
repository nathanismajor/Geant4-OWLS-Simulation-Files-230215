#include "detector.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4OpticalPhoton.hh"
//#include "PhysicalConstants.hh"


	//Sensitive Detector Mapped to CVH100 Lens

MyLensDetector::MyLensDetector(G4String name) : G4VSensitiveDetector(name)
{}

MyLensDetector::~MyLensDetector()
{}
// What Parameters to Measure

G4bool MyLensDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist)
{
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4cout 
	<< "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~OUTPUT START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << G4endl;
	
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4cout << "Photon Arrived At Detector, Position = " << posPhoton << G4endl;
	
	G4double depositedEnergy = postStepPoint->GetTotalEnergy();
	G4cout << "Photon Arrived At Detector, Energy Deposited = " << depositedEnergy << " eV" << G4endl;
	
	G4cout
	<< "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~OUTPUT END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << G4endl;
	
	G4Track *track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);
	
	//Storing Data In ROOT File
	
//Assigning Data to Columns and Rows of ROOT File
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	
	man->FillNtupleIColumn(0,evt);
	man->FillNtupleDColumn(1, posPhoton[0]);
	man->FillNtupleDColumn(2, posPhoton[1]);
	man->FillNtupleDColumn(3, posPhoton[2]);
	man->FillNtupleDColumn(4, (0.0000000000000041357*CLHEP::c_light)/depositedEnergy);
	man->AddNtupleRow(0);
	
	/*
	G4int nrg = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	man->FillNtupleIColumn(0,nrg);
	man->FillNtupleDColumn(1, depositedEnergy);
	man->AddNtupleRow(0);
	*/
	
	return true;
}
