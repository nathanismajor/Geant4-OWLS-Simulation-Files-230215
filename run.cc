#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->OpenFile("output.root");
	
	
	man->CreateNtuple("Hits", "Hits");
	man->CreateNtupleDColumn("Event");
	man->CreateNtupleDColumn("X_Position");
	man->CreateNtupleDColumn("Y_Position");
	man->CreateNtupleDColumn("Z_Position");
	man->CreateNtupleDColumn("Wavelength");
	
	man->FinishNtuple(0);
	
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();
	man->CloseFile();
}



