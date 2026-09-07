#include "Visuals.c"
uint32 SelectedPage = 0;
uint8 PageValue = 0;

void MAINMENUPAGE(uint8 CREATEPAGE);
void INGAMEPAGE(uint8 CREATEPAGE);
void SPECIESCREATORPAGE(uint8 CREATEPAGE);

void SetCamera(float X, float Y, float Z, float Zoom);

void PAGES() {
    filledPolygonRGBA(Renderer, 
    (Sint16[4]){DesktopWidth, DesktopWidth, 0, 0}, 
    (Sint16[4]){DesktopHeight, 0, 0, DesktopHeight}, 4, 0, 0, 0, 255);
    switch (SelectedPage)
    {
    case 0:
        MAINMENUPAGE(0);
        break;
    
    case 1:
        INGAMEPAGE(0);
        break;

    case 2:
        SPECIESCREATORPAGE(0);
        break;
    }
    SHOWTPS();
}

void MAINMENUPAGE(uint8 CREATEPAGE) {
    if(CREATEPAGE) {
        ResetBtns();
        CreateBtn(0, 120, 40, 960-60, 270);
        CreateBtn(1, 120, 40, 900, 810);
        CreateBtn(2, 120, 40, 900, 710);
    }

    /*
    for(int i = 0; i < DesktopWidth; i+=7) {
        for(int l = 0; l < DesktopHeight; l+=25) {
            AZL_DRAWLETTER(0, 1, 20, i, l, AZL_WHITE);
        }
    }*/
    //AZL_DRAWLETTER(0, 1, 200, 100, 100, AZL_WHITE);
    //AZL_DRAWLETTER(0, 0, 200, 180, 100, AZL_WHITE);

    switch (PageValue)
    {
    case 0:
        DrawBtn(0, AZL_WHITE);
        DrawBtn(2, AZL_BLUE);
        DrawBtn(1, AZL_RED);
        break;
    
    case 1:
        DrawBtn(100, AZL_WHITE);
        DrawBtn(101, AZL_WHITE);
        DrawBtn(900, AZL_RED);
        break;
    }
}
void MAINMENUBTNS(uint16 BID) {
    switch (BID)
    {
    case 0: //Enter Game
        SelectedPage = 2;
        SPECIESCREATORPAGE(1);
        break;

    case 1: //Exit
        Exit(0);
        break;

    case 2: //Settings
        PageValue = 1;
        ResetBtns();
        CreateBtn(100, 120, 40, 900, 300);
        CreateBtn(101, 120, 40, 900, 400);
        CreateBtn(900, 120, 40, 900, 810);
        break;

    case 900:
        PageValue = 0;
        MAINMENUPAGE(1);
        break;

    case 100:
        break;

    case 101:
        break;

    default:
        break;
    }
}

void SPECIESCREATORPAGE(uint8 CREATEPAGE) {
    if(CREATEPAGE) {
        InitiateSpecies();
        CreateSpecies();
        PlaceBodyPoint(0, 0, 0, 0);
        PlaceBodyPoint(0, 0, 1, 0);

        SetCamera(0,0,0,1);

        Ingame = 1;
        ResetBtns();
        CreateBtn(0, 75, 50, 0, 75);
    }

    for(int i = 1; i < BodyPointsCount[0]; i++) {
        for(int l = 0; l < 4; l++) {
            AZL_CREATE_CUSTOM_COLOR(255, 255, 255, 255);
            if(i == SelectedBodyPoints[l]) {
                AZL_CREATE_CUSTOM_COLOR(255, 0, 0, 255);
                l = 5;
            }
        }
        Point3DCircle(BodyPoints[0][i][0], BodyPoints[0][i][1], BodyPoints[0][i][2], AZL_CUSTOM_COLOR);
    }

    DrawBtn(0, AZL_RED);
}
void SPECIESCREATORBTNS(uint16 BID) {
    switch (BID)
    {
    case 0: 
        Ingame = 0;
        SelectedPage = 0;
        MAINMENUPAGE(1); 
        break;
    }
}

void INGAMEPAGE(uint8 CREATEPAGE) {
    if(CREATEPAGE) {
        CREATE_AO((uint16[1]){0}, 198900000000000000000000000000.0, 0, 0.01, 
        1000000, 1000, (double[3]){0.0, 0.0, 0.0}, (double[3]){0.0, 0.0, 1000.0});
        /**/
        CREATE_AO((uint16[1]){0}, 77882000000000000000000.0, 0, 0.01, 
        50, 1000, (double[3]){5000000000.0, 0.0, 0.0}, (double[3]){0.0, 50000.0, 0.0});
        
        CREATE_AO((uint16[1]){0}, 597220000000000000000000000.0, 0, 0.01, 
        300, 1000, (double[3]){0.0, 0.0, 149597900000.0}, (double[3]){8000.0, 0.0, 0.0});

        AO_Angle__Rotation[0][3] = 0.01;

        SetCamera(0,0,-20000000,10000000);

        Ingame = 1;
        ResetBtns();
        CreateBtn(0, 20, 20, DesktopWidth-20, 0);
        CreateBtn(1, 20, 20, DesktopWidth-40, 0);
        CreateBtn(2, 20, 20, DesktopWidth-60, 0);
    }
    Rectangle(DesktopWidth, DesktopHeight, 0, 0, AZL_BLACK);
    WorldTilePoint3D(1);
    
    if(Pause) { DrawBtn(0, AZL_RED);} else {DrawBtn(0, AZL_GREEN);}
    DrawBtn(1, AZL_WHITE);
    DrawBtn(2, AZL_GRAY);

    {
        float Tw;
        {
            double CZ = camera_anglef[2];
            //printf("%f\n", CZ);
            Tw = (CZ == 0 ? 1 : (int)(log10(fabs(CZ))+1)) * 10;
            char TextBuffer[32];
            snprintf(TextBuffer, 32, "%f", CZ);
            Text(OpenSans, TextBuffer, 64, Tw+50, 60, 10, 80, AZL_WHITE);
        }
        /*
        {
            double CZ = camera_zoom;
            Tw = (CZ == 0 ? 1 : (int)(log10(CZ)+1)) * 10;
            char TextBuffer[32];
            snprintf(TextBuffer, 32, "Speed: %f Km/s", CZ);
            Text(OpenSans, TextBuffer, 64, Tw+120, 60, 10, 80, AZL_WHITE);
        }

        {
            Tw = (SelectedAO == 0 ? 1 : (int)(log10(SelectedAO)+1)) * 10;
            char TextBuffer[6];
            if(SelectedAO) {
                snprintf(TextBuffer, 6, "%d", SelectedAO);
                Text(OpenSans, TextBuffer, 64, Tw, 60, 10, 150, AZL_WHITE);
            }
        }

        if(SelectedAO) {
            float AOSPEED = sqrt(pow(AO_Position__Velocity[SelectedAO-1][3] ,2) + pow(AO_Position__Velocity[SelectedAO-1][4] ,2)+ pow(AO_Position__Velocity[SelectedAO-1][5] ,2));
            
            Tw = (AOSPEED == 0 ? 1 : (int)(log10(AOSPEED)+1)) * 10;
            char TextBuffer[32];
            snprintf(TextBuffer, 32, "%.0f", AOSPEED);
            Text(OpenSans, TextBuffer, 64, Tw, 60, 10, 220, AZL_WHITE);
        }*/
    }
}
void INGAMEBTNS(uint16 BID) {
    switch (BID)
    {
    case 0: if(Pause) {Pause = 0;} else {Pause = 1;} break;
    case 1: if(GameSpeed < 10000) {GameSpeed*=2;} break;
    case 2: if(GameSpeed > 0) {GameSpeed*=0.5;} break;
    case 10: if(DebugMode) {DebugMode = 0;} else {DebugMode = 1;}; break; 
    }
}