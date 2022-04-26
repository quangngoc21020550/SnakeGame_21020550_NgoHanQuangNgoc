//
//  main.cpp
//  testSnake1
//
//  Created by Ngô Hán Quang Ngọc on 18/04/2022.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <ctime>
#include <vector>
#include <deque>

using namespace std;

int SCREENW = 1400;
int SCREENH = 700;
void logErr(string mess, bool err = false);
void initSDL(SDL_Window* &win, SDL_Renderer* &ren);
void quitSDL(SDL_Window* win, SDL_Renderer* ren);
void closePrg(int &dir, bool &run , SDL_Event e);



int main(int argc, const char * argv[]) {
    srand(time(0));
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    initSDL(win, ren);
    //-------------
    SDL_Event e;
    bool run = true;
    int dir = 0;
    SDL_Rect head{500,500,10,10};
    SDL_Rect apple{100,100,10,10};
    deque<SDL_Rect> snake;
    int size = 1;
    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
            run = false;
        }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP && dir !=2 ) dir = 1;
                if (e.key.keysym.sym == SDLK_DOWN && dir != 1) dir = 2;
                if (e.key.keysym.sym == SDLK_LEFT && dir != 4) dir = 3;
                if (e.key.keysym.sym == SDLK_RIGHT && dir !=3) dir = 4;
                
            }
        }
        switch (dir) {
            case 1:head.y -= 10; break;
            case 2:head.y += 10; break;
            case 3:head.x -= 10; break;
            case 4:head.x += 10; break;
        }
        if (head.y < 0) head.y = SCREENH;
        if (head.y > SCREENH) head.y = 0;
        if (head.x < 0) head.x = SCREENW;
        if (head.x > SCREENW) head.x = 0;
            
            
        if (head.x == apple.x && head.y == apple.y){
            apple.x =((rand()%SCREENW)/10)*10;
            apple.y = ((rand()%SCREENH)/10)*10;
            size+=20;
        }
        
        snake.push_front(head);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        for (int i = 0 ; i <size;i++) {
            SDL_Rect *s = & snake[i];
            SDL_RenderFillRect(ren, s);
        }
        for (int i = 1 ; i <size;i++) {
            if (head.x == snake[i].x && head.y == snake[i].y){
                quitSDL(win, ren);
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderFillRect(ren, &apple);
        SDL_RenderPresent(ren);
        SDL_Delay(30);
    }
    //-------------
    //SDL_RenderPresent(ren);
    quitSDL(win, ren);
}

void logErr(string mess, bool err) {
    cout << mess << "ERR: " << SDL_GetError() << endl;
    if (err) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &win, SDL_Renderer* &ren) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logErr("INIT ", true);
    }
    win = SDL_CreateWindow("TEST PAINT", 0, 0, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
    if (win == nullptr) logErr("WIN ", true);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                             SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) logErr("REN ", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(ren, SCREENW, SCREENH);
}
void quitSDL(SDL_Window* win, SDL_Renderer* ren) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void closePrg(int &dir, bool &run, SDL_Event e) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
        run = false;
    }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP && dir !=2 ) dir = 1;
            if (e.key.keysym.sym == SDLK_DOWN && dir != 1) dir = 2;
            if (e.key.keysym.sym == SDLK_LEFT && dir != 4) dir = 3;
            if (e.key.keysym.sym == SDLK_RIGHT && dir !=3) dir = 4;
            
        }
    }
}




