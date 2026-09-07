char* OpenSans = "IO/SDL/Fonts/OpenSans-Regular.ttf";
char* OpenSansBold = "IO/SDL/Fonts/OpenSans-Bold.ttf";
#include "../AzanaLib.h"
#include "Settings.h"
#include "../GameSpecific/world.c"
#include "../GameSpecific/Pawn.c"

void DrawBtns();
void SHOWTPS();
void WorldTilePoint3D(uint16 TileType);
void PAGES();

int STPSW = 0;
int RT = 0;
void SHOWTPS() {
    /**/
    if(ShowTPS) {
        if(RT != TE) {
            RT = TE;
            STPSW = (TE == 0 ? 1 : (int)(log10(TE)+1)) * 10;
        }
        char TextBuffer[6];
        snprintf(TextBuffer, 6, "%d", TE);
        if(10 > TE) {
            AZL_CREATE_CUSTOM_COLOR(255, 0, 0, 255);
        }
        else if(30 > TE) {
            AZL_CREATE_CUSTOM_COLOR(255, 128, 0, 255);
        }
        else if(60 > TE) {
            AZL_CREATE_CUSTOM_COLOR(255, 255, 0, 255);
        }
        else if(TE >= 60) {
            AZL_CREATE_CUSTOM_COLOR(255, 255, 255, 255);
        }
        Text(OpenSans, TextBuffer, 64, STPSW, 60, 10, 10, AZL_CUSTOM_COLOR);
    }
}

void* Render() {
    SDL_RenderClear(Renderer);
    PAGES();
    SDL_RenderPresent(Renderer);
    return NULL;
}

void DrawBtns() {
    for(int i = 0; i < BtnCount; i++) {
        DrawBtn(i, AZL_WHITE);
    }
}

void* DrawWorldSection(void* arg) {
    uint32 *DWSk = (uint32*)arg;
    uint32 Ci, Cik;
    uint8 v;
    char color[4];
    float X, Y, Z;
    Ci = DWSk[0]*DWSk[3];
    //printf("%d\n", DWSk[0]);
    if(DWSk[3] == ThreadUsage) {
        DWSk[0] += DWSk[1];
    }
    for(int k = 0; k < DWSk[0]; k++) {
        Cik = Ci+k;
        if(AO_Distance[DWSk[2]] > AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][3]) {
            X = AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][0] + AO_Position__Velocity[DWSk[2]][0];
            Y = AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][1] + AO_Position__Velocity[DWSk[2]][1];
            Z = AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][2] + AO_Position__Velocity[DWSk[2]][2];
            //printf("%f, %f, %f\n", X, Y, Z);
            switch (DWSk[4])
            {
            case 0: //Tile distance from cam
                v = AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][3]/1000;
                color[0] = -v;
                color[1] = -v;
                color[2] = -v;
                color[3] = 255;
                break;

            case 1: //World distance from cam
                v = log(AO_Tile_Data[DWSk[2]][DWSk[5]][Cik][3] - 1)*10;
                color[0] = -v;
                color[1] = -v;
                color[2] = -v;
                color[3] = 255;
                break;

            default:
                uint32 n = k;
                color[0] = (n % 255);
                color[1] = (n % 255);
                color[2] = (n % 255);
                color[3] = 255;
                break;
            }
            Point3D(X, Y, Z, color);
        }
    }
    return NULL;
}

void WorldTilePoint3D(uint16 TileType) {
    pthread_t WorldThread[ThreadUsage];
    pthread_mutex_lock(&lock);
    uint32 DWS[6];
    DWS[4] = TileType;
    uint32 l;
    
    for(int k = 0; k < AO; k++) {
        for(int ly = 0; ly < AO_Layers[k]; ly++) {
            l = AO_Order[k];
            GetScreenPosition(AO_Position__Velocity[l][0], AO_Position__Velocity[l][1], AO_Position__Velocity[l][2]);
            //printf("%d, %d, %f\n", l, AOOrder[l], AOPosition[AOOrder[l]][3]);
            AO_Interaction_Position[l][0] = Sx;
            AO_Interaction_Position[l][1] = Sy;
            AO_Interaction_Position[l][2] = Sz;
            AO_Interaction_Radius[l] = (float)DesktopWidth*(AO_Layers_Data[l][0][3]/AO_Distance[l]);
            //printf("%f, %f, %f\n", AO_Interaction_Radius[l], AO_Distance[l][0], AO_Position__Velocity[l][0]);
            if(Sz > 0 && !(Sx > (DesktopWidth + AO_Interaction_Radius[l]) || -AO_Interaction_Radius[l] > Sx) && !(Sy > (DesktopHeight + AO_Interaction_Radius[l]) || -AO_Interaction_Radius[l] > Sy)) {
                if(AO_Interaction_Radius[l] > DrawDistance) {
                    AO_Do_Render[l] = 1;
                    DWS[0] = floor(AO_Tile[l][ly]/ThreadUsage);
                    //printf("%d\n", DWS[0]);
                    DWS[1] = AO_Tile[l][ly] - (DWS[0] * ThreadUsage);
                    DWS[2] = l;
                    DWS[5] = ly;
                    for(int i = 0; i < ThreadUsage; i++) {
                        DWS[3] = i;
                        pthread_create(&WorldThread[i], NULL, DrawWorldSection, &DWS);
                        pthread_join(WorldThread[i], NULL);
                    }
                }
                else {
                    AO_Do_Render[l] = 0;
                    filledCircleRGBA(Renderer, Sx, Sy, AO_Interaction_Radius[l], 255, 255, 255, 255);
                }
                if(DebugMode == 1) {
                    circleRGBA(Renderer, Sx, Sy, AO_Interaction_Radius[l], 255, 0, 0, 255);
                }
            }
        }

    }
    pthread_mutex_unlock(&lock);
}
