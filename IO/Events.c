#include "Page.c"
void EvExit();
void SetCamera(float X, float Y, float Z, float Zoom);
void MoveCamera(float X, float Y, float Z);
void ZoomCamera(float Zoom);
void ShiftCam();
int IsButtonClicked();
void OrbitAroundAO();
void ButtonActions(uint16 BID);

uint8 MouseType = 0;
uint8 DoRender = 0;
uint8 PauseDelay = 0;
void Events() {
    currentKeyStates = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();

    if(Ingame) {
        ShiftCam();
    }

    while(SDL_PollEvent(&Event)) {
        EvExit();
        Mouse = SDL_GetMouseState(&MouseX, &MouseY);
        if(Ingame) {
            if(currentKeyStates[SDL_SCANCODE_SPACE]) {
                if(PauseDelay) {
                    if(Pause) {
                        Pause = 0;
                    }
                    else {
                        Pause = 1;
                    }
                }
                PauseDelay = 0;
            }
            else {
                PauseDelay = 1;
            }
            
            if(Event.type == SDL_MOUSEWHEEL) { camera_zoom += Event.wheel.preciseY * (camera_zoom/10); }

            if(Event.type == SDL_MOUSEBUTTONDOWN) {
                switch(Event.button.button) {
                    case SDL_BUTTON_RIGHT:
                        Mouse = SDL_GetMouseState(&NewMouseX, &NewMouseY);
                        MouseType = 1;
                        break;

                    case SDL_BUTTON_LEFT:
                        MouseType = 2;
                        break;
                }
            }
        }
        else {
            if(Event.type == SDL_MOUSEBUTTONDOWN) {
                if(Event.button.button == SDL_BUTTON_LEFT) {
                    MouseType = 2;
                }
            }
        }

        if(MouseType == 1) {
            if(NewMouseY != MouseY || NewMouseX != Mouse) {
                
                if(SelectedAO) {
                    camera_anglef[0] -= (float)(NewMouseX - MouseX) * 0.0005 * camera_sensitivity;
                    camera_anglef[2] -= (float)(NewMouseY - MouseY) * 0.0005 * camera_sensitivity;
                }
                else 
                {
                    camera_anglef[0] += (float)(NewMouseX - MouseX) * 0.0005 * camera_sensitivity;
                    camera_anglef[2] += (float)(NewMouseY - MouseY) * 0.0005 * camera_sensitivity;
                }

                camera_anglef[0] = fmod(camera_anglef[0], PI2);
                camera_anglef[2] = fmod(camera_anglef[2], PI2);
            }
            Mouse = SDL_GetMouseState(&NewMouseX, &NewMouseY);
        }

        if(Event.type == SDL_MOUSEBUTTONUP) {
            if(MouseType == 2) {
                if(IsButtonClicked()) {
                    long A, B, C;
                    if(Ingame) {
                        for(int i = 1; i < BodyPointsCount[CurrentSpecies]; i++) { //CurrentSpecies
                            GetScreenPosition(BodyPoints[CurrentSpecies][i][0], BodyPoints[CurrentSpecies][i][1], BodyPoints[CurrentSpecies][i][2]);
                            A = pow(Sx - (float)MouseX, 2);
                            B = pow(Sy - (float)MouseY, 2);
                            C = sqrt(A+B);
                            printf("%ld, %.1f\n", C, E);
                            if(E >= (float)C) {
                                if(currentKeyStates[SDL_SCANCODE_LSHIFT]) {
                                    SelectedBodyPoints[3] = SelectedBodyPoints[2];
                                    SelectedBodyPoints[2] = SelectedBodyPoints[1];
                                    SelectedBodyPoints[1] = SelectedBodyPoints[0];
                                    SelectedBodyPoints[0] = i;
                                }
                                else {
                                    SelectedBodyPoints[0] = i;
                                    SelectedBodyPoints[1] = 0;
                                    SelectedBodyPoints[2] = 0;
                                    SelectedBodyPoints[3] = 0;
                                }
                                
                                i = BodyPointsCount[CurrentSpecies];
                            }
                            else {
                                if(currentKeyStates[SDL_SCANCODE_LSHIFT]) {
                                    
                                }
                                else {
                                    SelectedBodyPoints[0] = 0;
                                    SelectedBodyPoints[1] = 0;
                                    SelectedBodyPoints[2] = 0;
                                    SelectedBodyPoints[3] = 0;
                                }
                            }
                        }
                    }
                }
            }
            MouseType = 0;
        }
    }
}

void EvExit() {
    if(Event.window.event == SDL_WINDOWEVENT_CLOSE) {
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(WindowSDL);
        SDL_Quit();
        Exit(0);
    }
}

void SetCamera(float X, float Y, float Z, float Zoom) {
    camera_position[0] = X;
    camera_position[1] = Y;
    camera_position[2] = Z;
    camera_angle[0] = 0;
    camera_angle[1] = 0;
    camera_zoom = Zoom;
}

void MoveCamera(float X, float Y, float Z) {
    if(SelectedAO) {
        AO_Distance_t[SelectedAO-1] += Z;
    }
    else {
        float Xn,Yn,Zn;
        Xn = X*cos(-camera_anglef[1]) - Y*sin(-camera_anglef[1]);
        Yn = X*sin(-camera_anglef[1]) + Y*cos(-camera_anglef[1]);
        X = Xn;
        Y = Yn;
        Yn = Y*cos(-camera_anglef[2]) - Z*sin(-camera_anglef[2]);
        Zn = Y*sin(-camera_anglef[2]) + Z*cos(-camera_anglef[2]);
        Y = Yn;
        Z = Zn;
        Xn = X*cos(-camera_anglef[0]) - Z*sin(-camera_anglef[0]);
        Zn = X*sin(-camera_anglef[0]) + Z*cos(-camera_anglef[0]);
        X = Xn;
        Z = Zn;
        camera_position[0] += X;
        camera_position[1] += Y;
        camera_position[2] += Z;
    }
    
}

void ZoomCamera(float Zoom) {
    camera_zoom += Zoom;
}

void ShiftCam() {
    float zoom = camera_zoom;
    float X, Y, Z;
    uint8 Moving = 0;
    X = 0; Y = 0; Z = 0;
    if(currentKeyStates[SDL_SCANCODE_A]) {
        X += zoom;
        Moving = 1;
    }
    if(currentKeyStates[SDL_SCANCODE_D]) {
        X -= zoom;
        Moving = 1;
    }
    if(currentKeyStates[SDL_SCANCODE_S]) {
        Z += zoom;
        Moving = 1;
    }
    if(currentKeyStates[SDL_SCANCODE_W]) {
        Z -= zoom;
        Moving = 1;
    }
    if(currentKeyStates[SDL_SCANCODE_Q]) {
        Y += zoom;
        Moving = 1;
    }
    if(currentKeyStates[SDL_SCANCODE_E]) {
        Y -= zoom;
        Moving = 1;
    }

    if(Moving) {
        MoveCamera(X, Y, Z);
        camera_position[3] = sqrt(pow(camera_position[0],2)+pow(camera_position[1],2)+pow(camera_position[2],2));
    }
}

void OrbitAroundAO() {
    uint32 l = SelectedAO-1;
    float K = AO_Layers_Data[l][0][3];
    float C2 = AO_Tile_Data[l][0][SelectedTile][2]/K;
    float C1 = AO_Tile_Data[l][0][SelectedTile][1]/K;
    float C0 = AO_Tile_Data[l][0][SelectedTile][0]/K;
    camera_position[0] = (C0 * AO_Distance_t[l])-AO_Position__Velocity[l][0];
    camera_position[1] = (C1 * AO_Distance_t[l])-AO_Position__Velocity[l][1];
    camera_position[2] = (C2 * AO_Distance_t[l])-AO_Position__Velocity[l][2];
    
    
    camera_anglef[0] = atan2(C0,C2);
    camera_anglef[2] = atan2(C1,(C2*cos(camera_anglef[0]))+(C0*sin(camera_anglef[0])));
}

int IsButtonClicked() {
    for(int i = 0; i < BtnCount; i++) {
        if((Btns[i][0] < MouseX && Btns[i][1] < MouseY) && (MouseX < Btns[i][2] && MouseY < Btns[i][3])) {
            ButtonActions(Btns[i][4]);
            return 0;
        }
    }
    return 1;
}

void ButtonActions(uint16 BID) {
    switch (SelectedPage)
    {
    case 0:
        MAINMENUBTNS(BID);
        break;
    
    case 1:
        INGAMEBTNS(BID);
        break;

    case 2:
        SPECIESCREATORBTNS(BID);
        break;
    }
}