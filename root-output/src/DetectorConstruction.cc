#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "DetectorConstruction.hh"
#include "VoxelParam.hh"
#include "VoxelSD.hh"

typedef G4LogicalVolume G4LV;
typedef G4PVPlacement G4PVP;
typedef G4VisAttributes G4VA;

DetectorConstruction::DetectorConstruction()
  : flv_voxel(NULL)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
	G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");
	
	// World volume
	G4Box* solidWorld = new G4Box("solidWorld", 100*cm, 100*cm, 100*cm);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0,
										G4ThreeVector(0., 0., 0.), 
										logicWorld, 
										"physWorld", 
										0, false, 0, true);



  G4double phantomXDimension = 20*cm;
  G4double phantomYDimension = 20*cm;
  G4double phantomZDimension = 20*cm;

  // water phantom
  const G4double DXY_PHANTOM = 20.*cm;
  const G4double DZ_PHANTOM = 50.*cm;

  G4Box* solidPhantom = 
    new G4Box("solidPhantom",
              phantomXDimension/2.0,
              phantomYDimension/2.0, 
              phantomZDimension/2.0);
  G4LogicalVolume* logicPhantom = 
    new G4LogicalVolume(solidPhantom, Water, "logicPhantom");
  // G4VPhysicalVolume* physPhantom =
    new G4PVPlacement(0, G4ThreeVector(), lv_phantom, "phantom", lv_world, false, 0);

  // Divide phantom into variety of XY planes
  G4double xyPlaneXDimension = 20.*cm;
  G4double xyPlaneYDimension = 20.*cm;
  G4double xyPlaneZDimension = 1.*mm;
  const G4double DXY_VXP = 20.*cm;
  const G4double DZ_VXP = 1.*mm;

  G4Box* solidVoxelPlane 

  G4Box* sld_vxp = new G4Box("vxplane", DXY_VXP/2., DXY_VXP/2., DZ_VXP/2.);
  G4LV* lv_vxp = new G4LV(sld_vxp, water, "vxplane");

  va = new G4VA(G4Color(1.,0.,0.));
  va-> SetVisibility(false);
  lv_vxp-> SetVisAttributes(va);

  for (G4int iz =0; iz < 500; iz++) {
    G4double z0 = -DZ_PHANTOM/2. + (iz+0.5)*DZ_VXP;
    new G4PVP(0, G4ThreeVector(0.,0.,z0),
              lv_vxp, "vxplane", lv_phantom, false, 1000+iz);
  }

  // voxel parameterized
  G4Box* sld_voxel = new G4Box("voxel",1.,1.,1.); // dummy
  flv_voxel = new G4LV(sld_voxel, water, "voxel");

  va = new G4VA(G4Color(0.,1.,1.));
  va-> SetVisibility(false);
  flv_voxel-> SetVisAttributes(va);

  const G4int nvoxels = 100*100;
  new G4PVParameterised("voxle", flv_voxel, lv_vxp, kUndefined, nvoxels,
                        new VoxelParam());

	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  //flv_voxel-> SetSensitiveDetector(new VoxelSD("voxel"));
}
