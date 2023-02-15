#include "construction.hh"
#include "G4VisAttributes.hh"
#include "G4Scintillation.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	
	//World Volume (1m Cube)
	
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	G4Box *solidWorld = new G4Box("solidWorld",1*m, 1*m, 1*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	G4VisAttributes* white = new G4VisAttributes(G4Colour(255/255., 255/255., 255/255.));
        
        white->SetVisibility(false);
        white->SetForceSolid(true);
       
        logicWorld->SetVisAttributes(white); 
	
	
	//Cuvette

	G4Material* vesselMat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	
	G4Box *solidVessel = new G4Box("solidVessel", 0.0125*m, 0.045*m, 0.0125*m);
	
	G4LogicalVolume *logicVessel = new G4LogicalVolume(solidVessel, vesselMat, "logicVessel");
	
	//G4VPhysicalVolume *physVessel = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicVessel, "physVessel", 0, false, 0, true);
	  
	new G4PVPlacement(0, G4ThreeVector(), logicVessel, "Vessel", logicWorld, false, 0);

	//Construction of Sample Materials

//Atomic Masses

G4double Ti_mass = 47.867*g/mole;
G4double O_mass = 16.00*g/mole;
G4double H_mass = 1.01*g/mole;
G4double C_mass = 12.011*g/mole;
G4double N_mass = 14.007*g/mole;
G4double Na_mass = 22.989*g/mole;
G4double S_mass = 32.065*g/mole;

//Material Densities

G4double tio2density = 4.23*g/cm3;
G4double h2odensity = 1*g/cm3;
G4double ppodensity = 1.06*g/cm3;
G4double sdbsdensity = 1.02*g/cm3;
G4double dbdensity = 0.856*g/cm3;
G4double labdensity = 0.863*g/cm3;

//Constructing Elements

G4Element* Ti = new G4Element("Titanium", "Ti", 1., Ti_mass);
G4Element* O = new G4Element("Oxygen", "O", 1., O_mass);
G4Element* H = new G4Element("Hydrogen", "H", 1., H_mass);
G4Element* C = new G4Element("Carbon", "C", 1., C_mass);
G4Element* N = new G4Element("Nitrogen", "C", 1., N_mass);
G4Element* Na = new G4Element("Sodium", "Na", 1., Na_mass);
G4Element* S = new G4Element("Sulfur", "S", 1., S_mass);

//Constructing Materials

G4Material* TiO2 = new G4Material("Titanium Dioxide", tio2density, 2);
	TiO2->AddElement(Ti, 1);
	TiO2->AddElement(O, 2);

G4Material* H2O = new G4Material("Water", h2odensity, 2);
	H2O->AddElement(H, 2);
	H2O->AddElement(O, 1);

G4Material* PPO = new G4Material("PPO", ppodensity, 4);
	PPO->AddElement(C, 15);
	PPO->AddElement(H, 11);
	PPO->AddElement(N, 1);
	PPO->AddElement(O, 1);

G4Material* LAB = new G4Material("LAB", labdensity, 4);
	LAB->AddElement(C, 6);
	LAB->AddElement(H, 5);
	LAB->AddElement(C, 12);
	LAB->AddElement(H, 25);

G4Material* SDBS = new G4Material("SDBS", sdbsdensity, 5);
	SDBS->AddElement(C, 18);
	SDBS->AddElement(H, 29);
	SDBS->AddElement(Na, 1);
	SDBS->AddElement(O, 3);
	SDBS->AddElement(S, 1);
	
	G4Material* DB = new G4Material("DB", dbdensity, 4);
	DB->AddElement(C, 12);
	DB->AddElement(H, 25);
	DB->AddElement(C, 6);
	DB->AddElement(H, 5);

//Constructing Material Blends

G4Material* opqblnd = new G4Material("opaqueblend", h2odensity, 2);
	opqblnd->AddMaterial(TiO2, 1*perCent);
	opqblnd->AddMaterial(H2O, 99*perCent);
	
G4Material* LAB_PPO = new G4Material("LABPPOblend", labdensity,2);
	LAB_PPO->AddMaterial(LAB, 50*perCent);
	LAB_PPO->AddMaterial(PPO, 50*perCent);
	
G4Material* DB_PPO = new G4Material("DBPPOblend", dbdensity, 2);
	DB_PPO->AddMaterial(DB, 50*perCent);
	DB_PPO->AddMaterial(PPO, 50*perCent);
	
G4Material* DB_PPO_WATER = new G4Material("DBPPOwater", h2odensity, 2);
	DB_PPO_WATER->AddMaterial(DB_PPO, 10*perCent);
	DB_PPO_WATER->AddMaterial(H2O, 90*perCent);
        
	//Scintillation Properties of the Control Sample (Water)
	
	G4Material* waterSample = nist->FindOrBuildMaterial("G4_WATER");
	
	G4MaterialPropertiesTable *wtrsmpl = new G4MaterialPropertiesTable();
	
	std::vector<G4double> mie_water = {
    167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m,
    128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
    97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m,
    71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
    51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m,
    36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
    24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m,
    16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
    9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m,
    5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
    3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m,
    1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m
  };
  
  std::vector<G4double> energy_water = {
    1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV,
    1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
    1.7971 * eV,  1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV,
    1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
    2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV,
    2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
    2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV,
    2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
    3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV,
    3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
    4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV,
    5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV
  };
	
	G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindexwtrsmpl[2] = {1.33,1.33};
	wtrsmpl->AddProperty("RINDEX", energy, rindexwtrsmpl, 2);
	G4double wtrsmpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double wtrsmpl_SCINT[3] = {0.1,1.0,0.1};
	G4double wtrsmpl_abslen[3] = {227*m,227*m,227*m};
	wtrsmpl->AddProperty("SCINTILLATIONCOMPONENT1", wtrsmpl_Energy, wtrsmpl_SCINT, 3);
	wtrsmpl->AddConstProperty("SCINTILLATIONYIELD", 127. / MeV);
	wtrsmpl->AddConstProperty("RESOLUTIONSCALE",1.);
	wtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	wtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	wtrsmpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	wtrsmpl->AddProperty("ABSLENGTH", wtrsmpl_Energy, wtrsmpl_abslen, 3);
	
	G4double mie_water_const[3] = {0.99, 0.99, 0.8};

  	wtrsmpl->AddProperty("MIEHG", energy_water, mie_water, false, true);
  	wtrsmpl->AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
  	wtrsmpl->AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
  	wtrsmpl->AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);
	
	waterSample->SetMaterialPropertiesTable(wtrsmpl);

	//Scintillation Properties of Titanium Dioxide Concentrated in Water
	/*
	G4Material* tio2_Sample = nist->FindOrBuildMaterial("opqblnd"); 
	//G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindextio2_smpl[2] = {1.342842,1.342842};
	G4MaterialPropertiesTable *tio2_smpl = new G4MaterialPropertiesTable();
	wtrsmpl->AddProperty("RINDEX", energy, rindextio2_smpl, 2);
	G4double tio2_smpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double tio2_smpl_SCINT[3] = {0.1,1.0,0.1};
	G4double tio2_smpl_abslen[3] = {227*m,227*m,227*m};
	tio2_smpl->AddProperty("SCINTILLATIONCOMPONENT1", tio2_smpl_Energy, tio2_smpl_SCINT, 3);
	tio2_smpl->AddConstProperty("SCINTILLATIONYIELD", 40000. / MeV);
	tio2_smpl->AddConstProperty("RESOLUTIONSCALE",1.);
	tio2_smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	tio2_smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	tio2_smpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	tio2_smpl->AddProperty("ABSLENGTH", tio2_smpl_Energy, tio2_smpl_abslen, 3);
	tio2_Sample->SetMaterialPropertiesTable(tio2_smpl);
	*/
	
	//Scintillation Properties of LAB-PPO
	
	G4Material* labSampleMaterial = nist->FindOrBuildMaterial("LAB_PPO");
	G4double rindexlab[2] = {1.525,1.525};
	G4MaterialPropertiesTable *labSample = new G4MaterialPropertiesTable();
	labSample->AddProperty("RINDEX", energy, rindexlab, 2);
	G4double labSample_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double labSample_abslen[3] = {26.8*m, 16.9*m, 25.8*m};
	labSample->AddProperty("ABSLENGTH", labSample_Energy, labSample_abslen, 3);
	labSample->AddConstProperty("SCINTILLATIONYIELD", 127. / MeV);
	labSample->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	labSample->AddConstProperty("RESOLUTIONSCALE",1.);
	
	//Material Properties of the Cuvette Glass (Quartz)
	
	G4Material* vesselMaterial = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	G4double rindexvessel[2] = {1.54425,1.55338};
	G4MaterialPropertiesTable *vessel = new G4MaterialPropertiesTable();
	vessel->AddProperty("RINDEX", energy, rindexvessel, 2);
	G4double vessel_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double vessel_abslen[3] = {0.2*m,0.2*m,0.2*m};
	vessel->AddProperty("ABSLENGTH", vessel_Energy, vessel_abslen,3);
	vesselMaterial->SetMaterialPropertiesTable(vessel);

	//Material Properties of The World Material (Air)
	
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4double rindexworld[2] = {1,1.1};
	G4MaterialPropertiesTable *world = new G4MaterialPropertiesTable();
	world->AddProperty("RINDEX", energy, rindexworld, 2);
	G4double world_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double world_abslen[3] = {0.2*m,0.2*m,0.2*m};
	world->AddProperty("ABSLENGTH", world_Energy, world_abslen,3);
	worldMaterial->SetMaterialPropertiesTable(world);
	
	//Material Properties of the Liquid Scintillator (DB)
	
	G4Material* dbMaterial = nist->FindOrBuildMaterial("DB");
	G4double rindexdb[1] = {1.461};
	G4MaterialPropertiesTable *db = new G4MaterialPropertiesTable();
	db->AddProperty("RINDEX", energy, rindexdb, 2);
	G4double db_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double db_abslen[3] = {0.2*m,0.2*m,0.2*m};
	db->AddProperty("ABSLENGTH", db_Energy, db_abslen,3);
	dbMaterial->SetMaterialPropertiesTable(db);
	
	//Material Properties of the Wavelength Shifter (PPO)
	
	G4Material* ppoMaterial = nist->FindOrBuildMaterial("PPO");
	G4double rindexppo[1] = {1.6231};
	G4MaterialPropertiesTable *ppo = new G4MaterialPropertiesTable();
	ppo->AddProperty("RINDEX", energy, rindexppo, 2);
	G4double ppo_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double ppo_abslen[3] = {0.2*m,0.2*m,0.2*m};
	ppo->AddProperty("ABSLENGTH", ppo_Energy, ppo_abslen, 3);
	ppoMaterial->SetMaterialPropertiesTable(ppo);
	
	
	//Material Properties of DB_PPO_WATER
	/*
	G4Material* dbppowtrSample = nist->FindOrBuildMaterial("DB_PPO_WATER");
	G4double dbppowtrenergy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindexdbppowtr[2] = {1.33,1.33};
	G4MaterialPropertiesTable *dbppowtrsmpl = new G4MaterialPropertiesTable();
	dbppowtrsmpl->AddProperty("RINDEX", dbppowtrenergy, rindexdbppowtr, 2);
	G4double dbppowtrsmpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double dbppowtrsmpl_SCINT[3] = {0.1,1.0,0.1};
	G4double dbppowtrsmpl_abslen[3] = {227*m,227*m,227*m};
	dbppowtrsmpl->AddProperty("SCINTILLATIONCOMPONENT1", dbppowtrsmpl_Energy, dbppowtrsmpl_SCINT, 3);
	dbppowtrsmpl->AddConstProperty("SCINTILLATIONYIELD", 127. / MeV);
	dbppowtrsmpl->AddConstProperty("RESOLUTIONSCALE",1.);
	dbppowtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	dbppowtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	dbppowtrsmpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	dbppowtrsmpl->AddProperty("ABSLENGTH", dbppowtrsmpl_Energy, dbppowtrsmpl_abslen, 3);
	
	dbppowtrSample->SetMaterialPropertiesTable(dbppowtrsmpl);
	*/
	//Sample (Inside Cuvette)
	
	G4Material* water = nist->FindOrBuildMaterial("G4_WATER");
	
	G4Material* dbppo_wtr = nist->FindOrBuildMaterial("G4_WATER");
	
	G4Box *solidEnv = new G4Box("Sample", 0.01125*m, 0.04375*m, 0.01125*m);
	
	G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv, water,"Sample");
    		
    	//G4VPhysicalVolume *physSample = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicEnv, "physSample", 0, false, 0, true);
    
    	new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Sample", logicVessel, false, 0);         
	

	G4VisAttributes* blue = new G4VisAttributes(G4Colour(0/255., 0/255., 255/255.));
        
        blue->SetVisibility(true);
        blue->SetForceSolid(true);
       
        logicEnv->SetVisAttributes(blue);	
	
	//Fiber at 0 Degrees with Lens
	
	G4double innerRadius = 0.0002*m;
	//G4double innerRadius = 0*m;
	//G4double outerRadius = 0.0015*m;
	G4double outerRadius = 0.002*m;
	G4double cylinderHeight = 0.00005*m;
	G4double startAngle = 0*deg;
	G4double spanAngle = 360.*deg;
	
	G4Tubs *absorbCylinder = new G4Tubs("absorbCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicTransmission = new G4LogicalVolume(absorbCylinder, worldMat, "logicTransmission");
	
	//G4VPhysicalVolume *physTransmission = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTransmission, "physCylinder", 0, false, 0, true);
	
	new G4PVPlacement(0, G4ThreeVector(0.,0.,-0.02025*m), logicTransmission, "TransmissionDetector", logicWorld, false, 0); 
	
	G4VisAttributes* orange = new G4VisAttributes(G4Colour(255/255., 165/255., 0/255.));
        
        orange->SetVisibility(true);
        orange->SetForceSolid(true);
       
        logicTransmission->SetVisAttributes(orange);
	
	
	//Fiber at 90 Degrees to Lens
	
	G4Tubs *scatterCylinder = new G4Tubs("scatterCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicScattering = new G4LogicalVolume(scatterCylinder, worldMat, "logicScattering");
	
	//G4VPhysicalVolume *physScattering = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScattering, "physCylinder", 0, false, 0, true);
	
	G4RotationMatrix *rot = new G4RotationMatrix;
	rot->rotateY(90*deg);
	
	new G4PVPlacement(rot, G4ThreeVector(-0.02025*m,0.,0.), logicScattering, "ScatteringDetector", logicWorld, false, 0); 
	
	G4VisAttributes* green = new G4VisAttributes(G4Colour(0/255., 255/255., 0/255.));
        
        green->SetVisibility(true);
        green->SetForceSolid(true);
       
        logicScattering->SetVisAttributes(green);
	
	//CVH100 Lens
	
	G4double iRadius = 0*m;
	//G4double iRadius = 0.001*m;
	G4double oRadius = 0.008*m;
	G4double cHeight = 0.00005*m;
	G4double sAngle = 0*deg;
	G4double spAngle = 360.*deg;
	
	G4Tubs *detectingLens = new G4Tubs("detectingLens", iRadius, oRadius, cHeight, sAngle, spAngle);
	
	logicLens = new G4LogicalVolume(detectingLens, worldMat, "logicLens");
	
	//G4VPhysicalVolume *physLens = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicLens, "physLens", 0, false, 0, true);
	
	new G4PVPlacement(rot, G4ThreeVector(0.02480*m,0.,0.), logicLens, "DetectingLens", logicWorld, false, 0); 
       
	G4VisAttributes* quitewhite = new G4VisAttributes(G4Colour(255/255., 255/255., 255/255.));
        
        quitewhite->SetVisibility(true);
        quitewhite->SetForceSolid(true);
       
        logicLens->SetVisAttributes(quitewhite);
	
	return physWorld;
}

//Assigning Sensitive Detectors to Detector Logical Volumes

void MyDetectorConstruction::ConstructSDandField()
{
	MyLensDetector *lensDet = new MyLensDetector("LensDetector");
	
	logicLens->SetSensitiveDetector(lensDet);
}
