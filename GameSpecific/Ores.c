typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

#define MOL 602214076000000000000000.0
#define AMU 0.000000000000000000000000001660539
#define BOLTZMANN 0.00000000000000000000001380649

#include "Materials.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void CreateOrePhase(uint16 OreID, uint16 PhaseType, uint8 Bounds, uint8 BoundType[Bounds], float n[Bounds], float Slope[Bounds], float Limits[4], float Offset[Bounds], float Density);
void CreateOre(uint16 OreID, float Mass, float Size, float HeatCapacity);

uint32 OreCount;

//{{{Bounds}, Bounds*{Sides, Phase Type, Bound Type}}, ...}
uint16*** OrePhaseExtra;
double** OreInfo;

//Ulx >= Temp >= Llx
//Temp^n * slope - Offset
//{{Bounds*{Llx, Lly, Ulx, Uly}, ...}, {Bounds*{n, slope, Offset}, ...}, ...}
float**** OrePhase;

void InitiateOres() {
    OreCount = 119;
    OrePhase = (float****)malloc(OreCount * sizeof(float***));
    OreInfo = (double**)malloc(OreCount * sizeof(double*));
    for(int i = 0; i < OreCount; i++) {
        OreInfo[i] = (double*)malloc(4 * sizeof(double));
    }
    OrePhaseExtra = (uint16***)malloc(OreCount * sizeof(uint16**));
    for(int i = 0; i < 119; i++) {
        OrePhaseExtra[i] = (uint16**)malloc(1 * sizeof(uint16*));
        OrePhaseExtra[i][0] = (uint16*)malloc(1 * sizeof(uint16));
        OrePhaseExtra[i][0][0] = 0;
    }

    CreateOre(0, 1.008664, 120, 14); //Neutron
    CreateOre(1, 1.00784, 120, 14.418);
    CreateOre(2, 4.002602, 140, 20.78);
    CreateOre(3, 6.938, 182, 24.860);
    CreateOre(4, 9.0121831, 153, 16.443);
    CreateOre(5, 10.806, 192, 11.087);
    CreateOre(6, 12.0096, 170, 8.517);
    CreateOre(7, 14.00643, 155, 14.562);
    CreateOre(8, 15.99903, 152, 14.689);
    CreateOre(9, 18.998403162, 135, 15.652);
    CreateOre(10, 20.1797, 154, 20.79);
    CreateOre(11, 22.98976928, 227, 28.230);
    CreateOre(12, 24.304, 173, 24.869);
    CreateOre(13, 26.9815384, 184, 24.20);
    CreateOre(14, 28.084, 210, 19.789);
    CreateOre(15, 30.973761998, 180, 23.824);
    CreateOre(16, 32.059, 180, 22.75);
    CreateOre(17, 35.446, 175, 16.9745);
    CreateOre(18, 39.792, 188, 20.85);
    CreateOre(19, 39.0983, 275, 29.6);
    CreateOre(20, 40.078, 231, 25.929);
    CreateOre(21, 44.955907, 211, 25.52);
    CreateOre(22, 47.867, 187, 25.060);
    CreateOre(23, 50.9415, 179, 24.89);
    CreateOre(24, 51.9961, 189, 23.35);
    CreateOre(25, 54.938043, 197, 26.32);
    CreateOre(26, 55.845, 194, 25.10);
    CreateOre(27, 58.933194, 192, 24.81);
    CreateOre(28, 58.6934, 163, 26.07);
    CreateOre(29, 63.546, 140, 24.440);
    CreateOre(30, 65.38, 139, 25.470);
    CreateOre(31, 69.723, 187, 25.86);
    CreateOre(32, 72.630, 211, 23.222);
    CreateOre(33, 74.921595, 185, 24.64);
    CreateOre(34, 78.971, 190, 25.363);
    CreateOre(35, 79.901, 183, 37.845);
    CreateOre(36, 83.798, 202, 20.95);
    CreateOre(37, 85.4678, 303, 31.060);
    CreateOre(38, 87.62, 249, 26.4);
    CreateOre(39, 88.905838, 219, 26.53);
    CreateOre(40, 91.222, 186, 25.36);
    CreateOre(41, 92.90637, 207, 24.60);
    CreateOre(42, 95.95, 209, 24.06);
    CreateOre(43, 97, 209, 24.27);
    CreateOre(44, 101.07, 207, 24.06);
    CreateOre(45, 102.90549, 195, 24.98);
    CreateOre(46, 106.42, 202, 25.98);
    CreateOre(47, 107.8682, 172, 25.350);
    CreateOre(48, 112.414, 158, 26.020);
    CreateOre(49, 114.818, 193, 26.74);
    CreateOre(50, 118.710, 217, 27.112);
    CreateOre(51, 121.760, 206, 25.23);
    CreateOre(52, 127.60, 206, 25.73);
    CreateOre(53, 126.90447, 198, 27.22);
    CreateOre(54, 131.293, 216, 21.01);
    CreateOre(55, 132.90545196, 343, 32.210);
    CreateOre(56, 137.327, 268, 28.07);
    CreateOre(57, 138.90547, 240, 27.11);
    CreateOre(58, 140.116, 235, 26.94);
    CreateOre(59, 140.90766, 239, 27.20);
    CreateOre(60, 144.242, 229, 27.45);
    CreateOre(61, 145, 236, 24.3);
    CreateOre(62, 150.36, 229, 29.54);
    CreateOre(63, 151.964, 233, 27.66);
    CreateOre(64, 157.249, 237, 37.03);
    CreateOre(65, 158.925354, 221, 28.91);
    CreateOre(66, 162.500, 229, 27.7);
    CreateOre(67, 164.930329, 216, 27.15);
    CreateOre(68, 167.259, 216, 28.12);
    CreateOre(69, 168.934219, 227, 27.03);
    CreateOre(70, 173.045, 242, 26.74);
    CreateOre(71, 174.96669, 221, 26.86);
    CreateOre(72, 178.486, 212, 25.73);
    CreateOre(73, 180.94788, 217, 25.36);
    CreateOre(74, 183.84, 210, 24.27);
    CreateOre(75, 186.207, 217, 25.48);
    CreateOre(76, 190.23, 216, 24.7);
    CreateOre(77, 192.217, 202, 25.10);
    CreateOre(78, 195.084, 209, 25.86);
    CreateOre(79, 196.966570, 166, 25.418);
    CreateOre(80, 200.592, 209, 27.983);
    CreateOre(81, 204.382, 196, 26.32);
    CreateOre(82, 206.14, 202, 26.650);
    CreateOre(83, 208.98040, 207, 25.52);
    CreateOre(84, 209, 197, 26.4);
    CreateOre(85, 210, 202, 20);
    CreateOre(86, 222, 220, 20.786);
    CreateOre(87, 223, 348, 20);
    CreateOre(88, 226, 283, 20);
    CreateOre(89, 227, 260, 27.2);
    CreateOre(90, 232.0377, 245, 26.230);
    CreateOre(91, 231.03588, 243, 99.1);
    CreateOre(92, 238.02891, 186, 27.665);
    CreateOre(93, 237, 239, 29.46);
    CreateOre(94, 244, 243, 35.5);
    CreateOre(95, 243, 244, 28);
    CreateOre(96, 247, 245, 20);
    CreateOre(97, 247, 244, 20);
    CreateOre(98, 251, 200, 20);
    CreateOre(99, 252, 200, 20);
    CreateOre(100, 257, 200, 20);
    CreateOre(101, 258, 200, 20);
    CreateOre(102, 259, 200, 20);
    CreateOre(103, 266, 200, 20);
    CreateOre(104, 267, 300, 20);
    CreateOre(105, 268, 300, 20);
    CreateOre(106, 267, 300, 20);
    CreateOre(107, 270, 300, 20);
    CreateOre(108, 271, 200, 20);
    CreateOre(109, 278, 200, 20);
    CreateOre(110, 281, 200, 20);
    CreateOre(111, 282, 200, 20);
    CreateOre(112, 285, 200, 20);
    CreateOre(113, 286, 300, 20);
    CreateOre(114, 290, 300, 20);
    CreateOre(115, 290, 300, 20);
    CreateOre(116, 293, 300, 20);
    CreateOre(117, 294, 300, 20);
    CreateOre(118, 294, 300, 20);

    CreateOrePhase(1, SOLID, 1, (uint8[1]){LOWERBOUND}, (float[2]){1}, (float[1]){714.285714286}, (float[4]){0, 0, 14, INFINITY}, (float[1]){0}, 4);
    CreateOrePhase(1, SOLID, 1, (uint8[1]){LOWERBOUND}, (float[1]){1}, (float[1]){370000}, (float[4]){14, 10000, 41, INFINITY}, (float[1]){-5170000}, 4);
    CreateOrePhase(1, SOLID, 1, (uint8[1]){LOWERBOUND}, (float[1]){1}, (float[1]){161001.788909}, (float[4]){41, 10000000, INFINITY, INFINITY}, (float[1]){3398930}, 4);
    CreateOrePhase(1, LIQUID, 3, (uint8[3]){UPPERBOUND, LOWERBOUND, LOWERBOUND}, (float[3]){1, 1, 1}, (float[3]){370000, 38076.9230769, 9000000}, (float[4]){14, 10000, 41, 10000000}, (float[3]){-5170000, -523076.924, -359000000}, 2);

    CreateOrePhase(2, SOLID, 2, (uint8[2]){LOWERBOUND, LOWERBOUND}, (float[2]){0.5, 11.3695}, (float[2]){52764.4853, 1}, (float[4]){0,2900000,INFINITY,INFINITY}, (float[2]){2900000, 2969381.47}, 4);
    CreateOrePhase(2, LIQUID, 3, (uint8[3]){LOWERBOUND, UPPERBOUND, UPPERBOUND}, (float[3]){4.385, 0.5, 19.18}, (float[3]){165, 52764.4853, -1}, (float[4]){0, 0, 2.2, 2900000}, (float[3]){0, 2900000, 3021742}, 2);
    CreateOrePhase(2, LIQUID, 3, (uint8[4]){LOWERBOUND, LOWERBOUND, LOWERBOUND, UPPERBOUND}, (float[4]){4.385, 4.8, 19.18, 11.3695}, (float[4]){165, 165, -1, 1}, (float[4]){1.7, 4998, INFINITY, INFINITY}, (float[4]){0, -222420, 2900000, 2969381.47}, 2);
    CreateOrePhase(2, LIQUIDGAS, 2, (uint8[2]){LOWERBOUND, UPPERBOUND}, (float[2]){4.385, 4.8}, (float[2]){165, 165}, (float[4]){5,226600,INFINITY,INFINITY}, (float[2]){0, -222420}, 1);
}

void CreateOre(uint16 OreID, float Mass, float Size, float HeatCapacity) {
    OreInfo[OreID][0] = Mass*AMU;
    OreInfo[OreID][1] = Size;
    OreInfo[OreID][2] = HeatCapacity;
    OreInfo[OreID][3] = Mass;
}

void CreateOrePhase(uint16 OreID, uint16 PhaseType, uint8 Bounds, uint8 BoundType[Bounds], float n[Bounds], float Slope[Bounds], float Limits[4], float Offset[Bounds], float Density) {
    uint32 OrePhaseID = OrePhaseExtra[OreID][0][0] + 1;

    if(OrePhaseExtra[OreID][0][0] == 0) {
        OrePhaseExtra[OreID] = (uint16**)realloc(OrePhaseExtra[OreID], 2 * sizeof(uint16*));
        OrePhaseExtra[OreID][1] = (uint16*)malloc((3+Bounds) * sizeof(uint16));
        OrePhaseExtra[OreID][1][0] = Bounds;
        OrePhaseExtra[OreID][1][1] = PhaseType;
        OrePhaseExtra[OreID][1][2] = Density;
        OrePhase[OreID] = (float***)malloc(2 * sizeof(float**));
        OrePhase[OreID][0] = (float**)malloc(1 * sizeof(float*));
        OrePhase[OreID][0][0] = (float*)malloc(4 * sizeof(float));
        for(int i = 0; i < 4; i++) {
            OrePhase[OreID][0][0][i] = Limits[i];
        }
        OrePhase[OreID][1] = (float**)malloc(Bounds * sizeof(float*));
        for(int i = 0; i < Bounds; i++) {
            OrePhase[OreID][1][i] = (float*)malloc(3 * sizeof(float));
            OrePhase[OreID][1][i][0] = n[i];
            OrePhase[OreID][1][i][1] = Slope[i];
            OrePhase[OreID][1][i][2] = Offset[i];
            OrePhaseExtra[OreID][1][3+i] = BoundType[i];
        }
    }
    else {
        OrePhaseExtra[OreID] = (uint16**)realloc(OrePhaseExtra[OreID], (OrePhaseID + 1) * sizeof(uint16*));
        OrePhaseExtra[OreID][OrePhaseID] = (uint16*)malloc((3+Bounds) * sizeof(uint16));
        OrePhaseExtra[OreID][OrePhaseID][0] = Bounds;
        OrePhaseExtra[OreID][OrePhaseID][1] = PhaseType;
        OrePhaseExtra[OreID][OrePhaseID][2] = Density;
        OrePhase[OreID] = (float***)realloc(OrePhase[OreID], (OrePhaseID+1) * sizeof(float**));
        OrePhase[OreID][0] = (float**)realloc(OrePhase[OreID][0], OrePhaseID * sizeof(float*));
        OrePhase[OreID][0][OrePhaseExtra[OreID][0][0]] = (float*)malloc(4 * sizeof(float));
        for(int i = 0; i < 4; i++) {
            OrePhase[OreID][0][OrePhaseExtra[OreID][0][0]][i] = Limits[i];
        }
        OrePhase[OreID][OrePhaseID] = (float**)malloc(Bounds * sizeof(float*));
        for(int i = 0; i < Bounds; i++) {
            OrePhase[OreID][OrePhaseID][i] = (float*)malloc(3 * sizeof(float));
            OrePhase[OreID][OrePhaseID][i][0] = n[i];
            OrePhase[OreID][OrePhaseID][i][1] = Slope[i];
            OrePhase[OreID][OrePhaseID][i][2] = Offset[i];
            OrePhaseExtra[OreID][OrePhaseID][3+i] = BoundType[i];
        }
    }
    OrePhaseExtra[OreID][0][0]++;
}

float GetOrePhase(uint16 OreID, double Temperature, double Pressure, uint8 ReturnType) {
    uint16 K;
    uint32 OrePhaseID = OrePhaseExtra[OreID][0][0] + 1;
    float X;
    for(int i = 1; i < OrePhaseID; i++) {
        uint16 ik = i-1;
        if(Temperature >= OrePhase[OreID][0][ik][0] && Pressure >= OrePhase[OreID][0][ik][1] && Temperature <= OrePhase[OreID][0][ik][2] && Pressure <= OrePhase[OreID][0][ik][3]) {
            K = 0;
            for(int l = 0; l < OrePhaseExtra[OreID][i][0]; l++) {
                X = pow(Temperature, OrePhase[OreID][i][l][0]) * OrePhase[OreID][i][l][1] + OrePhase[OreID][i][l][2];
                switch (OrePhaseExtra[OreID][i][3+l])
                {
                case UPPERBOUND:
                    if(X >= Pressure) {
                        K++;
                    }
                    else {
                        l = 10000;
                    }
                    break;
                
                case LOWERBOUND:
                    if(X <= Pressure) {
                        K++;
                    }
                    else {
                        l = 10000;
                    }
                    break;
                }
            }

            if(K == OrePhaseExtra[OreID][i][0]) {
                if(ReturnType) {
                    return OrePhaseExtra[OreID][i][2];
                }
                else {
                    return OrePhaseExtra[OreID][i][1];
                }
            }
        }
    }
    return 0;
}

double GetOreDensity(uint16 OreID, double Temperature, double Pressure, uint8 ReturnType) {
    double Density;
    switch (OreID)
    {
    case 1:
        if(!ReturnType) {
            Density = ((0.000000000084143764211*Pressure)/Temperature)*2758700.237362743;
            //(0.00...P/T)*2758700.237362743 = D
            //0.00...P/T = D/2758700.237362743
            //0.00...P = (D/2758700.237362743*T)/0.00...
        }
        else {
            Density = (Temperature*(Pressure/2758700.237362743))/0.000000000084143764211;
        }
        break;
    
    case 2:
        if(!ReturnType) {
            Density = ((0.000505159934418*Pressure)/Temperature);
        }
        else {
            Density = (Temperature*Pressure)/0.000505159934418;
        }
        //0.0005P/T = D
        //0.0005P = DT
        break;
    default:
        Density = 1;
        break;
    }
    if(!ReturnType) {
        Density += GetOrePhase(OreID, Temperature, Pressure, 1);
    }
    return Density;
}
