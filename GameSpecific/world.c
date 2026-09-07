typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Ores.c"
#define PI 3.14159265359
#define PI2 6.28318530718
#define GC 0.000000000066743015
#define IGC 8.31446261815324
#define GOLDENANGLE 2.39996
#define CC 8987551787.0

void OrderAORendering();

//
uint16 SelectedAO = 0;
uint32 SelectedTile = 0;

uint16* AO_Order;
uint8* AO_Do_Render;
float* AO_Interaction_Radius;
float** AO_Interaction_Position;

/*/AO values/*/

uint32 AO = 0;
//Purely aesthetic
uint16** AO_Name;
//X𖦏, Y𖦏, Z𖦏, X⇢, Y⇢, Z⇢
double** AO_Position__Velocity;
//Dist, solid dist, gassious dist
float* AO_Distance;
float* AO_Distance_t;
//X∠, Y∠, Z∠, X🗘, Y🗘, Z🗘
float** AO_Angle__Rotation;
uint32** AO_Tile;
//TileID(X𖦏, Y𖦏, Z𖦏, Distance, Mass)
float**** AO_Tile_Data;
double* AO_Mass;
//Body layers, Atmospheric layers
uint8* AO_Layers;
//Mass, Volume, Radius min, Radius max, LayerVelocity, Temp
double*** AO_Layers_Data;
float*** AO_Layer_Comp;
uint16*** AO_Layer_Comp_Type;

void Freeeeeetile() {
    free(AO_Order);
    free(AO_Do_Render);
    free(AO_Interaction_Radius);
    free(AO_Interaction_Position);
    free(AO_Name);
    free(AO_Position__Velocity);
    free(AO_Distance);
    free(AO_Angle__Rotation);
    free(AO_Tile);
    free(AO_Tile_Data);
    free(AO_Mass);
    free(AO_Layers);
    free(AO_Layers_Data);
    free(AO_Layer_Comp);
    free(AO_Layer_Comp_Type);

    AO_Order = NULL;
    AO_Do_Render = NULL;
    AO_Interaction_Radius = NULL;
    AO_Interaction_Position = NULL;
    AO_Name = NULL;
    AO_Position__Velocity = NULL;
    AO_Distance = NULL;
    AO_Angle__Rotation = NULL;
    AO_Tile = NULL;
    AO_Tile_Data = NULL;
    AO_Mass = NULL;
    AO_Layers = NULL;
    AO_Layers_Data = NULL;
    AO_Layer_Comp = NULL;
    AO_Layer_Comp_Type = NULL;
}

void ALLOCATE_AO() {
    if(AO) {
        uint32 Num = AO+1;
        AO_Order = (uint16*)realloc(AO_Order, Num*sizeof(uint16));
        AO_Do_Render = (uint8*)realloc(AO_Do_Render, Num*sizeof(uint8));
        AO_Interaction_Radius = (float*)realloc(AO_Interaction_Radius, Num*sizeof(float));
        AO_Interaction_Position = (float**)realloc(AO_Interaction_Position, Num*sizeof(float*));

        AO_Interaction_Position[AO] = (float*)malloc(3*sizeof(float));

        AO_Name = (uint16**)realloc(AO_Name, Num*sizeof(uint16*));

        AO_Position__Velocity = (double**)realloc(AO_Position__Velocity, Num*sizeof(double*));
        AO_Position__Velocity[AO] = (double*)malloc(6*sizeof(double));

        AO_Distance = (float*)realloc(AO_Distance, Num*sizeof(float));
        AO_Distance_t = (float*)realloc(AO_Distance_t, Num*sizeof(float));

        AO_Angle__Rotation = (float**)realloc(AO_Angle__Rotation, Num*sizeof(float*));
        AO_Angle__Rotation[AO] = (float*)malloc(6*sizeof(float));

        AO_Tile = (uint32**)realloc(AO_Tile, Num*sizeof(uint32*));
        AO_Tile[AO] = (uint32*)malloc(2*sizeof(uint32));
        AO_Tile_Data = (float****)realloc(AO_Tile_Data, Num*sizeof(float***));
        AO_Tile_Data[AO] = (float***)malloc(1*sizeof(float**));

        AO_Mass = (double*)realloc(AO_Mass, Num*sizeof(double));

        AO_Layers = (uint8*)realloc(AO_Layers, Num*sizeof(uint8));
        
        AO_Layers_Data = (double***)realloc(AO_Layers_Data, Num*sizeof(double**));
        AO_Layer_Comp = (float***)realloc(AO_Layer_Comp, Num*sizeof(float**));
        AO_Layer_Comp_Type = (uint16***)realloc(AO_Layer_Comp_Type, Num*sizeof(uint16**));
        AO_Layers_Data[AO] = (double**)malloc(2*sizeof(double*));
        AO_Layer_Comp[AO] = (float**)malloc(2*sizeof(float*));
        AO_Layer_Comp_Type[AO] = (uint16**)malloc(2*sizeof(uint16*));
        AO_Layers_Data[AO][0] = (double*)malloc(6 * sizeof(double));
    }
    else {
        AO_Order = (uint16*)malloc(sizeof(uint16));
        AO_Do_Render = (uint8*)malloc(sizeof(uint8));
        AO_Interaction_Radius = (float*)malloc(sizeof(float));

        AO_Interaction_Position = (float**)malloc(sizeof(float*));
        AO_Interaction_Position[0] = (float*)malloc(3*sizeof(float));

        AO_Name = (uint16**)malloc(sizeof(uint16*));

        AO_Position__Velocity = (double**)malloc(sizeof(double*));
        AO_Position__Velocity[0] = (double*)malloc(6*sizeof(double));

        AO_Distance = (float*)malloc(sizeof(float));
        AO_Distance_t = (float*)malloc(sizeof(float));

        AO_Angle__Rotation = (float**)malloc(sizeof(float*));
        AO_Angle__Rotation[0] = (float*)malloc(6*sizeof(float));

        AO_Tile = (uint32**)malloc(sizeof(uint32*));
        AO_Tile[0] = (uint32*)malloc(1*sizeof(uint32));
        AO_Tile_Data = (float****)malloc(sizeof(float***));
        AO_Tile_Data[0] = (float***)malloc(1*sizeof(float**));

        AO_Mass = (double*)malloc(sizeof(double));

        AO_Layers = (uint8*)malloc(sizeof(uint8));
        
        AO_Layers_Data = (double***)malloc(sizeof(double**));
        AO_Layers_Data[0] = (double**)malloc(1*sizeof(double*));
        AO_Layer_Comp = (float***)malloc(sizeof(float**));
        AO_Layer_Comp_Type = (uint16***)malloc(sizeof(uint16**));
        AO_Layer_Comp[0] = (float**)malloc(1*sizeof(float*));
        AO_Layer_Comp_Type[0] = (uint16**)malloc(1*sizeof(uint16*));
        AO_Layers_Data[0][0] = (double*)malloc(6 * sizeof(double));
    }
}

void CREATE_AO(uint16* NAME, float MASS, uint16 COMPOSITION, float TILE_FACTOR, 
    float TEMPERATURE, float DENSITY, double POSITION[3], double VELOCITY[3]) {
    ALLOCATE_AO();

    {
        uint16 NL = 0;
        for(int i = 0; i < 65536; i++) {
            NL++;
            if(!NAME[i]) {
                i = 65536;
                AO_Name[AO] = (uint16*)malloc(NL * sizeof(uint16));
            }
        }

        for(int i = 0; i < NL; i++) {
            AO_Name[AO][i] = NAME[i];
        }
    }

    AO_Order[AO] = 0;

    AO_Mass[AO] = MASS;

    AO_Position__Velocity[AO][0] = POSITION[0];
    AO_Position__Velocity[AO][1] = POSITION[1];
    AO_Position__Velocity[AO][2] = POSITION[2];
    AO_Position__Velocity[AO][3] = VELOCITY[0];
    AO_Position__Velocity[AO][4] = VELOCITY[1];
    AO_Position__Velocity[AO][5] = VELOCITY[2];

    AO_Angle__Rotation[AO][0] = 0;
    AO_Angle__Rotation[AO][1] = 0;
    AO_Angle__Rotation[AO][2] = 0;
    AO_Angle__Rotation[AO][3] = 0;
    AO_Angle__Rotation[AO][4] = 0;
    AO_Angle__Rotation[AO][5] = 0;

    AO_Layers[AO] = 1;

    uint16* Comp;
    float* CompPerc;
    uint16 CompLength;
    switch (COMPOSITION)
    {
    default:
        CompLength = 2;
        Comp = (uint16*)malloc(CompLength * sizeof(uint16));
        CompPerc = (float*)malloc(CompLength * sizeof(float));
        Comp[0] = 1;
        Comp[1] = 2;
        CompPerc[0] = 0.75;
        CompPerc[1] = 0.25;
        break;
    }

    AO_Layer_Comp[AO][0] = (float*)malloc(CompLength * sizeof(float));
    AO_Layer_Comp_Type[AO][0] = (uint16*)malloc(CompLength * sizeof(uint16));

    for(int i = 0; i < CompLength; i++) {
        AO_Layer_Comp[AO][0][i] = CompPerc[i];
        AO_Layer_Comp_Type[AO][0][i] = Comp[i];
    }

    double gravity = 100;
    AO_Layers_Data[AO][0][0] = MASS;
    AO_Layers_Data[AO][0][1] = MASS/DENSITY;
    AO_Layers_Data[AO][0][3] = cbrt(AO_Layers_Data[AO][0][1]/4.18888);
    AO_Layers_Data[AO][0][5] = TEMPERATURE;
    //printf("%f\n", AO_Layers_Data[AO][0][3]/1000);
    double InternalPressure = 0;
    for(int i = 0; i < 1000000; i++) {
        gravity = GC*(pow(MASS/2, 2)/pow(AO_Layers_Data[AO][0][3], 2));
        //InternalPressure = 0;
        InternalPressure = CC*(1/pow((1/((MASS/AO_Layers_Data[AO][0][1])*TEMPERATURE))*1000000000000.0, 2));
        //InternalPressure = CC*(1/(1/((MASS/AO_Layers_Data[AO][0][1])*TEMPERATURE)));
        /*for(int k = 0; k < CompLength; k++) {
            InternalPressure += CC*(1/((1/(MASS/AO_Layers_Data[AO][0][1]*TEMPERATURE))*1000000000000.0));
            
            switch ((uint32)GetOrePhase(AO_Layer_Comp_Type[AO][0][k], TEMPERATURE, gravity/MASS, 0))
            {
            case GAS:
                InternalPressure += (((MASS/AO_Layers_Data[AO][0][1]) * AO_Layer_Comp[AO][0][k] * IGC * TEMPERATURE)/OreInfo[AO_Layer_Comp_Type[AO][0][k]][0])/MASS;
                break;
            
            case LIQUID:
            case SOLID:
                InternalPressure += GetOreDensity(AO_Layer_Comp_Type[AO][0][k], TEMPERATURE, MASS/AO_Layers_Data[AO][0][1], 1) - GetOreDensity(AO_Layer_Comp_Type[AO][0][k], TEMPERATURE, gravity, 0);
                break;
            }
        }*/
        AO_Layers_Data[AO][0][3] += InternalPressure - gravity/MASS;
        //printf("\n%f\n", gravity/MASS);
        AO_Layers_Data[AO][0][1] = (4*PI*pow(AO_Layers_Data[AO][0][3], 3)/3);
        /*
        if(fmod(log10(i), 1) == 0) {
            printf("\n%f\n", AO_Layers_Data[AO][0][3]/1000);
        }*/
    }
    //printf("\nVolume\n%e\n\nMass\n%e\n", AO_Layers_Data[AO][0][1], MASS);
    //printf("\n%f\n", AO_Layers_Data[AO][0][3]/1000);

    if(TILE_FACTOR == 0) {
        AO_Tile[AO][0] = 0;
    }
    else if(TILE_FACTOR < 0.1) {
        AO_Tile[AO][0] = (uint32)(pow(4*PI*log(AO_Layers_Data[AO][0][3]), 2));
    }
    else {
        AO_Tile[AO][0] = (uint32)(pow(4*PI*(AO_Layers_Data[AO][0][3]/1000), 2)/TILE_FACTOR);
    }
    
    //printf("TileCount $%d\n", AO_Tile[AO][0]);
    AO_Tile_Data[AO][0] = (float**)malloc(AO_Tile[AO][0] * sizeof(float*));
    float x, y, z, r, t;
    for(int i = 0; i < AO_Tile[AO][0]; i++) {
        AO_Tile_Data[AO][0][i] = (float*)malloc(4 * sizeof(float));
        y = 1-(((float)i/(float)(AO_Tile[AO][0]-1))*2);
        r = sqrt(1-(y*y)) * AO_Layers_Data[AO][0][3];
        t = GOLDENANGLE*i;
        x = cos(t)*r;
        z = sin(t)*r;

        AO_Tile_Data[AO][0][i][0] = x;
        AO_Tile_Data[AO][0][i][1] = y*AO_Layers_Data[AO][0][3];
        AO_Tile_Data[AO][0][i][2] = z;
    }

    AO++;
}

void ResizeAO() {
    double gravity = 0;
    double InternalPressure = 0;
    float x, y, z, r, t, v;
    for(int l = 0; l < AO; l++) {
        gravity = GC*(pow(AO_Mass[l]/2, 2)/pow(AO_Layers_Data[l][0][3], 2));
        InternalPressure = CC*(1/pow((1/((AO_Mass[l]/AO_Layers_Data[l][0][1])*AO_Layers_Data[l][0][5]))*1000000000000.0, 2));

        v = (InternalPressure - gravity/AO_Mass[l]) * GameSpeed;
        if(v) {
            AO_Layers_Data[l][0][3] += v;
            AO_Layers_Data[l][0][1] = (4*PI*pow(AO_Layers_Data[l][0][3], 3)/3);

            for(int i = 0; i < AO_Tile[l][0]; i++) {
                y = 1-(((float)i/(float)(AO_Tile[l][0]-1))*2);
                r = sqrt(1-(y*y)) * AO_Layers_Data[l][0][3];
                t = GOLDENANGLE*i;
                x = cos(t)*r;
                z = sin(t)*r;

                AO_Tile_Data[l][0][i][0] = x;
                AO_Tile_Data[l][0][i][1] = y*AO_Layers_Data[l][0][3];
                AO_Tile_Data[l][0][i][2] = z;
            }
        }
    }
}

int comp(const void* a,const void* b) {
  	return *(int*)b - *(int*)a;
}

void OrderAORendering() {
    float Temp[AO];
    for(int i = 0; i < AO; i++) {
        Temp[i] = AO_Distance[i];
    }
    qsort(Temp, AO, sizeof(float), comp);

    for(int i = 0; i < AO; i++) {
        for(int l = 0; l < AO; l++) {
            if(AO_Distance[i] == Temp[l]) {AO_Order[l] = i; l = AO;}
        }
    }
}

void GetTileDist(float Cx, float Cy, float Cz) {
    uint32 l;
    float X, Y, Z;
    for(int k = 0; k < AO; k++) {
        l = AO_Order[k];
        X = AO_Position__Velocity[l][0] + Cx;
        Y = AO_Position__Velocity[l][1] + Cy;
        Z = AO_Position__Velocity[l][2] + Cz;
        AO_Distance[l] = sqrt((X*X)+(Y*Y)+(Z*Z));
        if(AO_Do_Render[l]) {
            for(int ly = 0; ly < AO_Layers[l]; ly++) {
                if(AO_Layers[l]) {
                    for(int i = 0; i < AO_Tile[l][ly]; i++) {
                        AO_Tile_Data[l][ly][i][3] = sqrt(pow(AO_Tile_Data[l][ly][i][0] + X,2) + pow(AO_Tile_Data[l][ly][i][1] + Y,2) + pow(AO_Tile_Data[l][ly][i][2] + Z,2));
                    }
                }
            }
        }
    }
}

void MoveAO() {
    for(int l = 0; l < AO; l++) {
        AO_Position__Velocity[l][0] += AO_Position__Velocity[l][3] * GameSpeed;
        AO_Position__Velocity[l][1] += AO_Position__Velocity[l][4] * GameSpeed;
        AO_Position__Velocity[l][2] += AO_Position__Velocity[l][5] * GameSpeed;
        /*
        if(SelectedAO-1 == l) {
            camera_position[0] += AO_Position__Velocity[l][3] * GameSpeed;
            camera_position[1] += AO_Position__Velocity[l][4] * GameSpeed;
            camera_position[2] += AO_Position__Velocity[l][5] * GameSpeed;
        }*/
    }
}

void RotateAO() {
    float Xn, Yn, Znn, C4, S4, C3, S3, C5, S5;
    for(int l = 0; l < AO; l++) {
        if(AO_Angle__Rotation[l][3] || AO_Angle__Rotation[l][4] || AO_Angle__Rotation[l][5]) {
            C4 = cos(AO_Angle__Rotation[l][4] * GameSpeed);
            S4 = sin(AO_Angle__Rotation[l][4] * GameSpeed);
            C3 = cos(AO_Angle__Rotation[l][3] * GameSpeed);
            S3 = sin(AO_Angle__Rotation[l][3] * GameSpeed);
            C5 = cos(AO_Angle__Rotation[l][5] * GameSpeed);
            S5 = sin(AO_Angle__Rotation[l][5] * GameSpeed);
            for(int ly = 0; ly < AO_Layers[l]; ly++) {
                for(long i = 0; i < AO_Tile[l][ly]; i++) {
                    //Z Rotation
                    Xn = AO_Tile_Data[l][ly][i][0]*C4 - AO_Tile_Data[l][ly][i][1]*S4;
                    Yn = AO_Tile_Data[l][ly][i][0]*S4 + AO_Tile_Data[l][ly][i][1]*C4;
                    AO_Tile_Data[l][ly][i][0] = Xn;
                    AO_Tile_Data[l][ly][i][1] = Yn;
                    //Y Rotation
                    Xn = AO_Tile_Data[l][ly][i][0]*C3 - AO_Tile_Data[l][ly][i][2]*S3;
                    Znn = AO_Tile_Data[l][ly][i][0]*S3 + AO_Tile_Data[l][ly][i][2]*C3;
                    AO_Tile_Data[l][ly][i][0] = Xn;
                    AO_Tile_Data[l][ly][i][2] = Znn;
                    //X Rotation
                    Yn = AO_Tile_Data[l][ly][i][1]*C5 - AO_Tile_Data[l][ly][i][2]*S5;
                    Znn = AO_Tile_Data[l][ly][i][1]*S5 + AO_Tile_Data[l][ly][i][2]*C5;
                    AO_Tile_Data[l][ly][i][1] = Yn;
                    AO_Tile_Data[l][ly][i][2] = Znn;
                }
            }

            if(AO_Angle__Rotation[l][3] > PI2) {
                AO_Angle__Rotation[l][3] -= PI2;
            }

            if(AO_Angle__Rotation[l][4] > PI2) {
                AO_Angle__Rotation[l][4] -= PI2;
            }

            if(AO_Angle__Rotation[l][5] > PI2) {
                AO_Angle__Rotation[l][5] -= PI2;
            }
        }
    }
}

void PullAO() {
    double Pull, dist, X, Y, Z, k;

    for(int i = 0; i < AO; i++) {
        for(int l = 0; l < AO; l++) {
            if(i != l) {
                X = AO_Position__Velocity[i][0] - AO_Position__Velocity[l][0];
                Y = AO_Position__Velocity[i][1] - AO_Position__Velocity[l][1];
                Z = AO_Position__Velocity[i][2] - AO_Position__Velocity[l][2];
                //printf("%d, %d\n%f\n", i, l, X);
                k = fabs(X)+fabs(Y)+fabs(Z);
                dist = sqrt((X*X) + (Y*Y) + (Z*Z));
                Pull = GC*((AO_Mass[i]*AO_Mass[l])/(pow(dist,2)+1));

                AO_Position__Velocity[i][3] -= (X/k)*(Pull/AO_Mass[i]) * GameSpeed;
                AO_Position__Velocity[i][4] -= (Y/k)*(Pull/AO_Mass[i]) * GameSpeed;
                AO_Position__Velocity[i][5] -= (Z/k)*(Pull/AO_Mass[i]) * GameSpeed;
            }
        }
    }
}
