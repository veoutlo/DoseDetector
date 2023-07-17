#include "physicalConstruction.hh"

PhysicalConstruction::PhysicalConstruction(G4ThreeVector world) : worldSize(world) {
}

PhysicalConstruction::~PhysicalConstruction() {
}

G4VPhysicalVolume *PhysicalConstruction::Construct() {
	G4VPhysicalVolume *physWorld = SetupGeometry();
	return physWorld;
}

G4VPhysicalVolume* PhysicalConstruction::SetupGeometry() {
    G4VPhysicalVolume *physWorld = BuildMotherVolume();
    BuildWorldConstruction();

    return physWorld;
}

G4VPhysicalVolume* PhysicalConstruction::BuildMotherVolume() {
    G4NistManager *nist = G4NistManager::Instance();
	G4Material *vacuum = nist->FindOrBuildMaterial("G4_AIR");

    //G4Isotope* iso98 = new G4Isotope("Mo98", 42, 98, 98*CLHEP::g/mole);
    //
    //G4Element* Mo98 = new G4Element("isotope Mo98", "Mo" , 1);
    //Mo98->AddIsotope(iso98, 100*CLHEP::perCent);
    //
    //G4Material* matMo= new G4Material("Mo98 mat" , 10.22*CLHEP::g/cm3, 1, kStateSolid);
    //matMo->AddElement(Mo98, 1.00);

    G4Box *solidWorld = 
        new G4Box(
            "solidWorld", 
            worldSize.x(), 
            worldSize.y(), 
            worldSize.z());

	logicWorld = 
        new G4LogicalVolume(
            solidWorld, 
            vacuum, 
            "logicWorld");
	
	G4VPhysicalVolume *physWorld = 
        new G4PVPlacement(
            0, 
			G4ThreeVector(0., 0., 0.), 
			logicWorld, 
			"physWorld", 
			0, 
			false, 
			0, 
			false); 

    return physWorld;
}

void PhysicalConstruction::BuildWorldConstruction() {
    BuildSource();
    BuildTarget();
}

void PhysicalConstruction::BuildTarget() {
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    G4Box *waterBox = 
        new G4Box(
            "target", 
            10*CLHEP::cm, 
            5*CLHEP::cm, 
            5*CLHEP::cm);

    G4LogicalVolume *waterLogic = 
        new G4LogicalVolume(
            waterBox, 
            concrete, 
            "targetLogic");

    G4VPhysicalVolume *waterWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector(50*CLHEP::cm, 0., 0.), 
			waterLogic, 
			"physTarget", 
			logicWorld, 
			false, 
			0, 
			false); 
}

void PhysicalConstruction::BuildSource() {
    
}