typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//Species Traits
uint16 SpeciesCuttingPoint = 100;

uint32 CurrentSpecies = 0;

uint32 Species = 0;
uint64* SpeciesPawns; //How many Pawns in each species

//Measured in mm for lengths and mm³ for volumes
uint32* BodyPointsCount;
uint32*** BodyPoints; //xyz
uint32 SelectedBodyPoints[4];
uint32 SelectedBodyShapes[32];
uint32* BodyShapeCount;
uint32*** BodyShape; //Connect 4 points to make triangular pyramids, then get volume from those
uint16** BodyShapeType; //What is the shape, muscle, fat, ect...
uint16** BodyParts; //Connect Shapes to make body part
uint64* BodyVolume;

uint32* JuvenileVolume; //mm³
uint32* EggDiameter; //mm

void PlaceBodyPoint(uint32 SelectedSpecies, int X, int Y, int Z) {
    uint32 n = BodyPointsCount[SelectedSpecies];
    BodyPointsCount[SelectedSpecies]++;
    BodyPoints[SelectedSpecies] = (uint32**)realloc(BodyPoints[SelectedSpecies], BodyPointsCount[SelectedSpecies] * sizeof(uint32*));
    BodyPoints[SelectedSpecies][n] = (uint32*)malloc(3 * sizeof(uint32));
    BodyPoints[SelectedSpecies][n][0] = X;
    BodyPoints[SelectedSpecies][n][1] = Y;
    BodyPoints[SelectedSpecies][n][2] = Z;
}

void CreateBodyShapeFromPoints(uint32 SelectedSpecies) {
    uint32 n = BodyShapeCount[SelectedSpecies];
    BodyShapeCount[SelectedSpecies]++;
    BodyShape[SelectedSpecies] = (uint32**)realloc(BodyShape[SelectedSpecies], BodyPointsCount[SelectedSpecies] * sizeof(uint32*));
    BodyShape[SelectedSpecies][n] = (uint32*)malloc(4 * sizeof(uint32));
    for(int i = 0; i < 4; i++) {
        BodyShape[SelectedSpecies][n][i] = SelectedBodyPoints[i];
    }
}

void CreateSpecies() {
    uint32 n = Species;
    Species++;
    BodyPoints = (uint32***)realloc(BodyPoints, Species * sizeof(uint32**));
    BodyPointsCount = (uint32*)realloc(BodyPointsCount, Species * sizeof(uint32));
    BodyPointsCount[n] = 0;
    BodyPoints[n] = (uint32**)malloc(sizeof(uint32*));

    BodyShape = (uint32***)realloc(BodyShape, Species * sizeof(uint32**));
    BodyShapeCount = (uint32*)realloc(BodyShapeCount, Species * sizeof(uint32));
    BodyShapeCount[n] = 0;
    BodyShape[n] = (uint32**)malloc(sizeof(uint32*));
}

void InitiateSpecies() {
    BodyPoints = (uint32***)malloc(sizeof(uint32**));
    BodyPointsCount = (uint32*)malloc(sizeof(uint32));
    BodyShape = (uint32***)malloc(sizeof(uint32**));
    BodyShapeCount = (uint32*)malloc(sizeof(uint32));
}