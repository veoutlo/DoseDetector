#ifndef EFFECTIVE_DOSE_HH
#define EFFECTIVE_DOSE_HH

#include "G4VPrimitivePlotter.hh"
#include "G4THitsMap.hh"

#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VScoreHistFiller.hh"

#include <iostream>
#include <map>
#include <utility>

// ICRP, 2010. Conversion Coefficients for Radiological Protection Quantities for External Radiation Exposures. ICRP Publication 116, Ann. ICRP 40(2-5).
// Effective dose per fluence, in units of pSv cm 2, for mono-energetic particles incident in various geometries.
// geometry: AP, antero-posterior; PA, postero-anterior; ISO, isotropic

// geometry 0 for AP, 1 for PA, 2 for ISO

enum {AP = 1, PA = 2, ISO = 3};

static constexpr double sievert = joule/kilogram ;
static constexpr double millisievert = 1.e-3*sievert;
static constexpr double microsievert = 1.e-6*sievert;
static constexpr double nanosievert  = 1.e-9*sievert;
static constexpr double picosievert  = 1.e-12*sievert;

class EffectiveDose : public G4VPrimitivePlotter {
public:
	EffectiveDose(G4String name, G4int geometry = 2, G4int depth = 0);
	~EffectiveDose() override = default	;
	
	void Initialize(G4HCofThisEvent*);
	double ConvertDim(double pdg, double energy_MeV, int geometry); // convert unit flux to effective dose [pSv/cm2]
	double Convert(double pdg, double energy, int geometry); // convert unit flux to effective dose [pSv/cm2] 

private:
  G4double ComputeVolume(G4Step*, G4int);
	G4int HCID;
	G4THitsMap<G4double>* EvtMap;
  G4double vergeA;
  G4int detectorGeometry;

protected:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

  double photons[4][55]={
    {0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.511, 0.6, 0.662, 0.8, 1.0, 1.117, 1.33, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 6.129, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000},
    {0.0685, 0.156, 0.225, 0.312, 0.350, 0.369, 0.389, 0.411, 0.443, 0.518, 0.747, 1.00, 1.51, 2.00, 2.47, 2.52, 2.91, 3.17, 3.73, 4.49, 4.90, 5.60, 6.12, 7.48, 9.75, 11.7, 13.4, 15.0, 15.1, 17.8, 20.5, 26.1, 30.8, 37.9, 43.2, 47.1, 50.1, 54.5, 57.8, 63.2, 67.2, 72.3, 75.4, 77.4, 78.7, 80.4, 81.6, 83.7, 85.0, 86.6, 87.8, 88.6, 89.1, 89.9, 90.4},
    {0.0184, 0.0155, 0.0261, 0.0946, 0.163, 0.209, 0.243, 0.273, 0.302, 0.363, 0.543, 0.745, 1.16, 1.58, 1.99, 2.03, 2.39, 2.63, 3.14, 3.84, 4.23, 4.90, 5.41, 6.77, 9.13, 11.2, 13.2, 15.0, 15.2, 18.6, 22.1, 30.4, 38.2, 51.3, 61.8, 70.1, 76.5, 86.2, 92.7, 103, 110, 118, 123, 127, 130, 134, 137, 141, 144, 147, 149, 151, 152, 153, 154},
    {0.0288, 0.0560, 0.0813, 0.127, 0.158, 0.180, 0.198, 0.218, 0.238, 0.286, 0.429, 0.589, 0.932, 1.28, 1.63, 1.66, 1.97, 2.17, 2.62, 3.25, 3.60, 4.21, 4.67, 5.91, 8.08, 10.0, 11.8, 13.5, 13.7, 16.6, 19.7, 26.8, 33.8, 46.1, 56.9, 66.1, 74.1, 87.1, 97.5, 116, 129, 147, 159, 167, 174, 185, 193, 208, 218, 232, 242, 251, 258, 268, 276} };

  double electrons[4][49]={
    {0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000}, 
    {0.0269, 0.0404, 0.0539, 0.0810, 0.108, 0.135, 0.163, 0.218, 0.275, 0.418, 0.569, 0.889, 1.24, 1.63, 2.05, 4.04, 7.10, 15.0, 22.4, 36.1, 48.2, 59.3, 70.6, 97.9, 125, 188, 236, 302, 329, 337, 341, 346, 349, 355, 359, 365, 369, 372, 375, 379, 382, 387, 391, 397, 401, 405, 407, 411, 414}, 
    {0.0268, 0.0402, 0.0535, 0.0801, 0.107, 0.133, 0.160, 0.213, 0.267, 0.399, 0.530, 0.787, 1.04, 1.28, 1.50, 1.68, 1.68, 1.62, 1.62, 1.95, 2.62, 3.63, 5.04, 9.46, 18.3, 53.1, 104, 220, 297, 331, 344, 358, 366, 379, 388, 399, 408, 414, 419, 428, 434, 446, 455, 468, 477, 484, 490, 499, 507}, 
    {0.0188, 0.0283, 0.0377, 0.0567, 0.0758, 0.0948, 0.114, 0.152, 0.191, 0.291, 0.393, 0.606, 0.832, 1.08, 1.35, 1.97, 2.76, 4.96, 7.24, 11.9, 16.4, 21.0, 25.5, 35.5, 46.7, 76.9, 106, 164, 212, 249, 275, 309, 331, 363, 383, 410, 430, 445, 457, 478, 495, 525, 549, 583, 608, 628, 646, 675, 699} };

  double positrons[4][49]={
    {0.01, 0.015, 0.02, 0.03, 0.04, 0.05, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000}, 
    {3.28, 3.29, 3.30, 3.33, 3.36, 3.39, 3.42, 3.47, 3.53, 3.67, 3.84, 4.16, 4.52, 4.90, 5.36, 7.41, 10.5, 18.3, 25.7, 39.1, 51.0, 61.7, 72.9, 99.0, 126, 184, 229, 294, 320, 327, 333, 339, 342, 349, 354, 362, 366, 369, 372, 376, 379, 385, 389, 395, 399, 402, 404, 408, 411}, 
    {1.62, 1.64, 1.65, 1.68, 1.71, 1.73, 1.76, 1.82, 1.87, 2.01, 2.14, 2.40, 2.65, 2.90, 3.12, 3.32, 3.37, 3.44, 3.59, 4.19, 5.11, 6.31, 8.03, 14.0, 23.6, 59.0, 111, 221, 291, 321, 334, 349, 357, 371, 381, 393, 402, 409, 415, 424, 430, 443, 451, 465, 473, 480, 486, 495, 503}, 
    {1.39, 1.40, 1.41, 1.43, 1.45, 1.47, 1.49, 1.53, 1.57, 1.67, 1.77, 1.98, 2.21, 2.45, 2.72, 3.38, 4.20, 6.42, 8.70, 13.3, 18.0, 22.4, 26.9, 36.7, 47.6, 75.5, 104, 162, 209, 243, 268, 302, 323, 356, 377, 405, 425, 440, 453, 474, 491, 522, 545, 580, 605, 627, 645, 674, 699} };

  double neutrons[4][68]={
    {1.0E-9, 1.0E-8, 2.5E-8, 1.0E-7, 2.0E-7, 5.0E-7, 1.0E-6, 2.0E-6, 5.0E-6, 1.0E-5, 2.0E-5, 5.0E-5, 1.0E-4, 2.0E-4, 5.0E-4, 0.001, 0.002, 0.005, 0.01, 0.02, 0.03, 0.05, 0.07, 0.1, 0.15, 0.2, 0.3, 0.5, 0.7, 0.9, 1.0, 1.2, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 14.0, 15.0, 16.0, 18.0, 20.0, 21.0, 30.0, 50.0, 75.0, 100, 130, 150, 180, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 5000, 10000}, 
    {3.09, 3.55, 4.00, 5.20, 5.87, 6.59, 7.03, 7.39, 7.71, 7.82, 7.84, 7.82, 7.79, 7.73, 7.54, 7.54, 7.61, 7.97, 9.11, 12.2, 15.7, 23.0, 30.6, 41.9, 60.6, 78.8, 114, 177, 232, 279, 301, 330, 365, 407, 458, 483, 494, 498, 499, 499, 500, 500, 499, 495, 493, 490, 484, 477, 474, 453, 433, 420, 402, 382, 373, 363, 359, 363, 389, 422, 457, 486, 508, 524, 537, 612, 716, 933}, 
    {1.85, 2.11, 2.44, 3.25, 3.72, 4.33, 4.73, 5.02, 5.30, 5.44, 5.51, 5.55, 5.57, 5.59, 5.60, 5.60, 5.62, 5.95, 6.81, 8.93, 11.2, 15.7, 20.0, 25.9, 34.9, 43.1, 58.1, 85.9, 112, 136, 148, 167, 195, 235, 292, 330, 354, 371, 383, 392, 398, 404, 412, 417, 419, 420, 422, 423, 423, 422, 428, 439, 444, 446, 446, 447, 448, 464, 496, 533, 569, 599, 623, 640, 654, 740, 924, 1.17E+3}, 
    {1.29, 1.56, 1.76, 2.26, 2.54, 2.92, 3.15, 3.32, 3.47, 3.52, 3.54, 3.55, 3.54, 3.52, 3.47, 3.46, 3.48, 3.66, 4.19, 5.61, 7.18, 10.4, 13.7, 18.6, 26.6, 34.4, 49.4, 77.1, 102, 126, 137, 153, 174, 203, 244, 271, 290, 303, 313, 321, 327, 332, 339, 344, 346, 347, 350, 352, 353, 358, 371, 387, 397, 407, 412, 421, 426, 455, 488, 521, 553, 580, 604, 624, 642, 767, 1.01E+3, 1.32E+3} };
  
  double pion_minus[4][43]={
    {1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000, 150000, 200000}, 
    {406, 422, 433, 458, 491, 528, 673, 965, 1.09E+3, 1.25E+3, 1.28E+3, 1.77E+3, 1.92E+3, 1.93E+3, 1.68E+3, 1.14E+3, 995, 927, 902, 848, 844, 869, 901, 947, 977, 1.03E+3, 1.05E+3, 1.03E+3, 1.03E+3, 1.06E+3, 1.09E+3, 1.14E+3, 1.17E+3, 1.21E+3, 1.24E+3, 1.30E+3, 1.35E+3, 1.39E+3, 1.42E+3, 1.48E+3, 1.54E+3, 1.67E+3, 1.78E+3}, 
    {194, 201, 210, 225, 233, 237, 208, 181, 178, 197, 244, 547, 1.02E+3, 1.70E+3, 1.99E+3, 1.31E+3, 991, 889, 871, 843, 850, 880, 917, 976, 1.02E+3, 1.08E+3, 1.12E+3, 1.11E+3, 1.13E+3, 1.18E+3, 1.22E+3, 1.29E+3, 1.34E+3, 1.41E+3, 1.47E+3, 1.56E+3, 1.63E+3, 1.70E+3, 1.75E+3, 1.86E+3, 1.95E+3, 2.15E+3, 2.33E+3}, 
    {176, 189, 198, 215, 232, 251, 271, 317, 361, 439, 508, 676, 868, 1.02E+3, 1.15E+3, 1.15E+3, 1.03E+3, 857, 815, 794, 807, 838, 875, 935, 979, 1.05E+3, 1.09E+3, 1.11E+3, 1.15E+3, 1.20E+3, 1.26E+3, 1.36E+3, 1.43E+3, 1.55E+3, 1.64E+3, 1.79E+3, 1.91E+3, 2.02E+3, 2.11E+3, 2.29E+3, 2.46E+3, 2.80E+3, 3.04E+3} };

  double pion_plus[4][43]={
    {1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000, 150000, 200000}, 
    {314, 324, 340, 379, 429, 489, 540, 717, 819, 1000, 1.10E+3, 1.52E+3, 1.75E+3, 1.83E+3, 1.66E+3, 1.22E+3, 1.13E+3, 1.22E+3, 1.25E+3, 1.07E+3, 969, 943, 952, 999, 1.04E+3, 1.10E+3, 1.10E+3, 1.06E+3, 1.06E+3, 1.07E+3, 1.10E+3, 1.14E+3, 1.17E+3, 1.22E+3, 1.25E+3, 1.30E+3, 1.34E+3, 1.38E+3, 1.42E+3, 1.48E+3, 1.54E+3, 1.67E+3, 1.78E+3}, 
    {121, 125, 133, 151, 170, 183, 185, 177, 179, 201, 247, 494, 906, 1.48E+3, 1.82E+3, 1.38E+3, 1.12E+3, 1.15E+3, 1.23E+3, 1.10E+3, 998, 970, 980, 1.04E+3, 1.09E+3, 1.16E+3, 1.19E+3, 1.16E+3, 1.16E+3, 1.20E+3, 1.24E+3, 1.31E+3, 1.35E+3, 1.42E+3, 1.48E+3, 1.57E+3, 1.64E+3, 1.70E+3, 1.75E+3, 1.84E+3, 1.94E+3, 2.14E+3, 2.33E+3}, 
    {151, 160, 168, 183, 198, 216, 233, 265, 296, 367, 439, 602, 787, 953, 1.09E+3, 1.16E+3, 1.10E+3, 1.05E+3, 1.08E+3, 1.02E+3, 953, 930, 938, 993, 1.05E+3, 1.13E+3, 1.16E+3, 1.16E+3, 1.18E+3, 1.23E+3, 1.28E+3, 1.37E+3, 1.43E+3, 1.55E+3, 1.64E+3, 1.79E+3, 1.90E+3, 2.01E+3, 2.10E+3, 2.27E+3, 2.42E+3, 2.76E+3, 3.07E+3} };

  double muon_minus[4][41]={
    {1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000}, 
    {180, 180, 184, 188, 193, 205, 242, 293, 332, 414, 465, 657, 735, 755, 628, 431, 382, 340, 326, 319, 320, 321, 325, 327, 333, 331, 333, 336, 337, 337, 337, 337, 338, 338, 340, 338, 339, 343, 339, 339, 345}, 
    {75.2, 76.8, 78.3, 81.4, 84.8, 87.7, 86.7, 86.8, 88.6, 100, 122, 251, 457, 703, 775, 485, 402, 345, 329, 321, 321, 324, 326, 332, 337, 338, 341, 344, 345, 346, 346, 347, 347, 347, 347, 348, 348, 348, 349, 349, 350}, 
    {78.7, 79.5, 80.9, 83.7, 87.1, 91.5, 98.1, 113, 127, 161, 191, 275, 363, 446, 496, 498, 432, 354, 332, 321, 321, 323, 326, 331, 337, 338, 341, 344, 346, 347, 347, 348, 348, 348, 349, 350, 350, 350, 351, 351, 352} };

  double muon_plus[4][41]={
    {1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000}, 
    {194, 196, 198, 202, 207, 216, 251, 300, 340, 425, 481, 674, 751, 768, 635, 431, 381, 339, 326, 318, 319, 320, 322, 325, 327, 331, 333, 336, 337, 337, 337, 337, 339, 338, 338, 338, 339, 343, 339, 339, 345}, 
    {82.6, 84.1, 85.7, 88.9, 92.1, 94.3, 92.5, 92.8, 94.8, 108, 133, 265, 473, 721, 787, 483, 399, 345, 328, 320, 321, 323, 325, 330, 333, 339, 341, 344, 345, 346, 346, 347, 347, 347, 347, 348, 348, 348, 349, 349, 350}, 
    {85.2, 86.2, 87.5, 90.3, 93.6, 97.7, 103, 117, 132, 167, 199, 284, 373, 456, 506, 502, 432, 354, 332, 320, 320, 322, 324, 329, 333, 338, 341, 344, 346, 347, 347, 348, 348, 348, 349, 350, 350, 350, 351, 351, 352} };

  double helium[4][21]={
    {1.0, 2.0, 3.0, 5.0, 10.0, 20.0, 30.0, 50.0, 100, 150, 200, 300, 500, 1000, 2000, 3000, 5000, 10000, 20000, 50000, 100000}, 
    {219, 438, 656, 1.09E+3, 2.19E+3, 1.72E+4, 3.01E+4, 4.75E+4, 1.01E+5, 9.25E+4, 6.74E+4, 5.14E+4, 4.27E+4, 4.00E+4, 4.02E+4, 4.08E+4, 4.12E+4, 4.56E+4, 5.12E+4, 6.12E+4, 7.14E+4}, 
    {219, 438, 656, 1.09E+3, 2.19E+3, 1.74E+3, 1.44E+3, 2.88E+3, 4.84E+4, 1.10E+5, 7.29E+4, 5.33E+4, 4.49E+4, 4.47E+4, 4.80E+4, 5.01E+4, 5.18E+4, 6.26E+4, 6.10E+4, 8.14E+4, 1.01E+5}, 
    {141, 281, 419, 689, 1.82E+3, 5.46E+3, 9.86E+3, 1.78E+4, 4.55E+4, 6.95E+4, 7.01E+4, 5.25E+4, 4.27E+4, 4.09E+4, 4.31E+4, 4.50E+4, 4.76E+4, 5.73E+4, 7.10E+4, 9.67E+4, 1.24E+5} };

  double protons[4][33]={
    {1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 8000, 10000}, 
    {5.46, 8.20, 10.9, 16.4, 21.9, 27.3, 32.8, 43.7, 54.9, 189, 428, 750, 1.02E+3, 1.18E+3, 1.48E+3, 2.16E+3, 2.51E+3, 2.38E+3, 1.77E+3, 1.38E+3, 1.23E+3, 1.15E+3, 1.16E+3, 1.11E+3, 1.09E+3, 1.15E+3, 1.12E+3, 1.23E+3, 1.27E+3, 1.23E+3, 1.37E+3, 1.45E+3, 1.41E+3}, 
    {5.47, 8.21, 10.9, 16.4, 21.9, 27.3, 32.8, 43.7, 54.6, 56.1, 43.6, 36.1, 45.5, 71.5, 156, 560, 1.19E+3, 2.82E+3, 1.93E+3, 1.45E+3, 1.30E+3, 1.24E+3, 1.23E+3, 1.23E+3, 1.23E+3, 1.25E+3, 1.28E+3, 1.34E+3, 1.40E+3, 1.45E+3, 1.53E+3, 1.65E+3, 1.74E+3}, 
    {3.52, 5.28, 7.02, 10.5, 13.9, 17.3, 20.5, 26.8, 45.8, 80.1, 136, 249, 358, 451, 551, 837, 1.13E+3, 1.79E+3, 1.84E+3, 1.42E+3, 1.25E+3, 1.18E+3, 1.17E+3, 1.17E+3, 1.15E+3, 1.21E+3, 1.22E+3, 1.31E+3, 1.40E+3, 1.43E+3, 1.57E+3, 1.71E+3, 1.78E+3} };

};

#endif