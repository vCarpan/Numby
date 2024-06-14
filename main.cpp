#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 800, HEIGHT = 600;
const int FPS = 120;
const int FRAME_DELAY = 1000 / FPS;

struct player {
    int x, y, z; // player position
    int a; // player angle
    int l;
}; player P;

struct math {
    float cos[360];
    float sin[360];
}; math M;

void drawPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int opacity);

void keyPressHandler(int key);

void init();

void draw3D(SDL_Renderer *renderer);

void drawWall(SDL_Renderer *renderer, int x, int y, int b1, int b2, int t1, int t2);

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("DOOM!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event windowEvent;
    bool keepRunning = true;
    bool draw = false;

    Uint32 frameStart;
    Uint32 frameTime;
    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();
    double fps = 0;

    init();

    while (keepRunning) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&windowEvent)) {
            cout<<"Player: "<<P.x << " " << P.y << " " << P.z<<endl;
            switch (windowEvent.type) {
                case SDL_QUIT:
                    keepRunning = false;
                    break;
                case SDL_KEYDOWN:
                    keyPressHandler(windowEvent.key.keysym.sym);
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 5, 61, 125, 255);
        SDL_RenderClear(renderer);


        draw3D(renderer);

        SDL_RenderPresent(renderer);

        frameCount++;
        frameTime = SDL_GetTicks() - frameStart;

        if (SDL_GetTicks() - startTime >= 1000) {
            fps = frameCount / ((SDL_GetTicks() - startTime) / 1000.0f);
            cout << "FPS: " << fps << endl;
            frameCount = 0;
            startTime = SDL_GetTicks();
        }

        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void init() {

    //store sin and cos in degrees

    for(int i = 0; i < 360; i++){
        M.cos[i]=cos(i/180.0*M_PI);
        M.sin[i]=sin(i/180.0*M_PI);
    }

    P.x=199; P.y=-171; P.z=0; P.a=0; P.l=0; 
}

void drawPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int opacity) {
    
    SDL_SetRenderDrawColor(renderer, r, g, b, opacity);
    SDL_Rect rect = {x, y, 4, 4}; // 4x4 pixel block
    SDL_RenderFillRect(renderer, &rect);
}

void keyPressHandler(int key) {
    int dx=M.sin[P.a]*10.0;
    int dy=M.cos[P.a]*10.0;

    switch (key) {
        case SDLK_w: // up
            P.x+=dx/2;
            P.y+=dy/2;
            break;
        case SDLK_a: // left
            P.a-=8; if(P.a<0)P.a+=360;
            break;
        case SDLK_s: // down
            P.x-=dx/2;
            P.y-=dy/2;
            break;
        case SDLK_d: // right
            P.a+=8; if(P.a>359)P.a-=360;
            break;
        case SDLK_q: // strafe left
            P.x-=dy;
            P.y+=dx;
            break;
        case SDLK_e: // strafe right
            P.x+=dy;
            P.y-=dx;
            break;
        case SDLK_z: // strafe left
            P.z+=4;
            break;
        case SDLK_x: // strafe left
            P.z-=4;
            break;
        case SDLK_h: // look up
            P.l-=1;
            break;
        case SDLK_j: // look down
            P.l+=1;
            break;
        default:
            cout << "unknown" << "\n";
            break;
    }
}

void draw3D(SDL_Renderer *renderer) {
    int wx[4], wy[4], wz[4];
    float CS = M.cos[P.a], SN = M.sin[P.a];

    int x1 = 100 - P.x, y1 = 10 - P.y;
    int x2 = 100 - P.x, y2 = 30 - P.y;

    wx[0] = x1 * CS - y1 * SN;
    wx[1] = x2 * CS - y2 * SN;

    wx[2] = wx[0];
    wx[3] = wx[1];

    wy[0] = y1 * CS - x1 * SN;
    wy[1] = y2 * CS - x2 * SN;
    
    wy[2] = wy[0];
    wy[3] = wy[1];

    wz[0] = 0 - P.z+((P.l*wy[0])/32);
    wz[1] = 0 - P.z+((P.l*wy[1])/32);

    wz[2] = wz[0] + 60;
    wz[3] = wz[1] + 60;

    wx[0] = wx[0] * 200 / wy[0] + (WIDTH / 2);
    wy[0] = wz[0] * 200 / wy[0] + (HEIGHT / 2);

    wx[1] = wx[1] * 200 / wy[1] + (WIDTH / 2);
    wy[1] = wz[1] * 200 / wy[1] + (HEIGHT / 2);

    wx[2] = wx[2] * 200 / wy[2] + (WIDTH / 2);
    wy[2] = wz[2] * 200 / wy[2] + (HEIGHT / 2);

    wx[3] = wx[3] * 200 / wy[3] + (WIDTH / 2);
    wy[3] = wz[3] * 200 / wy[3] + (HEIGHT / 2);

    if (wx[0] > 0 && wx[0] < WIDTH && wy[0] > 0 && wy[0] < HEIGHT){
        drawPixel(renderer, wx[0], wy[0], 255, 0, 0, 255);
        //cout << "pixel en: x:" << wx[0] << " y:" << wy[0] << "\n"; 
    }
    if (wx[1] > 0 && wx[1] < WIDTH && wy[1] > 0 && wy[1] < HEIGHT){
        drawPixel(renderer, wx[1], wy[1], 255, 0, 0, 255);
        //cout << "pixel en: x:" << wx[1] << " y:" << wy[1] << "\n"; 
    }

    drawWall(renderer, wx[0], wx[1], wy[0], wy[1], wy[2], wy[3]);
}

void drawWall(SDL_Renderer *renderer, int x1, int x2, int b1, int b2, int t1, int t2) {
    int x,y;

    int dyb = b2-b1;
    int dyt = t2-t1;
    int dx = x2 - x1; if( dx==0) dx = 1;
    int xs = x1;

    for(int x=x1; x<x2;x++){
        int y1 = dyb*(x-xs+0.5)/dx+b1;
        int y2 = dyt*(x-xs+0.5)/dx+t1;
        for(y=y1; y<y2; y++){
            if (x > 10 && x < WIDTH-10 && y > 10 && y < HEIGHT-10){
                drawPixel(renderer, x, y, 255, 255, 0, 255);
            }
        }
    }

}
