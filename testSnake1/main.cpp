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

const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};
const SDL_Color DEFAULT_COLOR = BLACK_COLOR;
int SCREENW = 1400;
int SCREENH = 700;
void logErr(string mess, bool err = false);
void initSDL(SDL_Window* &win, SDL_Renderer* &ren);
void quitSDL(SDL_Window* win, SDL_Renderer* ren);
void closePrg(int &dir, bool &run , SDL_Event e);
int makeRand(int a, int b);
void printApple(SDL_Renderer* ren, SDL_Rect &apple);

struct Apple  {
    SDL_Renderer *renderer;
    int size;
    int nutri;
    SDL_Rect location;
    Apple(SDL_Window* window, SDL_Renderer* renderer_, SDL_Rect _location, int _size = 0, int _nutri =1)
     : renderer(renderer_)
    {
         int width, height;
         SDL_RenderGetLogicalSize(renderer, &width, &height);
         if (width == 0 && height == 0)
         SDL_GetWindowSize(window, &width, &height);
         location = _location;
        size = _size;
        nutri = _nutri;
    }
    void printApple() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &location);
    }
};
struct Snake {
    SDL_Renderer *renderer;
    SDL_Color color;
    SDL_Rect head;
    int dir = 0;
    int size = 1;
    deque<SDL_Rect> fullBody;
    Snake(SDL_Window* window, SDL_Renderer* renderer_,SDL_Color _color, SDL_Rect _head, int _dir = 0, int _size =1)
     : renderer(renderer_)
    {
         int width, height;
         SDL_RenderGetLogicalSize(renderer, &width, &height);
         if (width == 0 && height == 0)
         SDL_GetWindowSize(window, &width, &height);
         head = _head;
         dir = _dir;
        size = _size;
        color = _color;
    }
    void takeInput(SDL_Event &e) {

                if (e.key.keysym.sym == SDLK_UP && dir !=2 ) dir = 1;
                if (e.key.keysym.sym == SDLK_DOWN && dir != 1) dir =  2;
                if (e.key.keysym.sym == SDLK_LEFT && dir != 4) dir =  3;
                if (e.key.keysym.sym == SDLK_RIGHT && dir !=3) dir =  4;
                
            
        
    }
    void changeDir() {
        switch (dir) {
            case 1:head.y -= 10; break;
            case 2:head.y += 10; break;
            case 3:head.x -= 10; break;
            case 4:head.x += 10; break;
        }
    }
    void checkOutScreen() {
        if (head.y < 0) head.y = SCREENH;
        if (head.y > SCREENH) head.y = 0;
        if (head.x < 0) head.x = SCREENW;
        if (head.x > SCREENW) head.x = 0;
    }

    void checkApple(Apple &apple) {
        if (head.x == apple.location.x && head.y == apple.location.y){
            apple.location.x =((rand()%SCREENW)/10)*10;
            apple.location.y = ((rand()%SCREENH)/10)*10;
            size += apple.nutri;
        }
    }
    void addBody() {
        fullBody.push_front(head);
    }
    void printSnake() {
        SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
        for (int i = 0 ; i < size;i++) {
            SDL_Rect *s = & fullBody[i];
            SDL_RenderFillRect(renderer, s);
        }
    }
    void checkSeflLose(){
        for (int i = 1 ; i < size;i++) {
            if (head.x == fullBody[i].x && head.y == fullBody[i].y){
                SDL_Quit();
            }
        }
    }
};

int main(int argc, const char * argv[]) {
    srand(time(0));
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    initSDL(win, ren);
    //-------------
    SDL_Event e;
    bool run = true;
    int dir = 0;
    SDL_Rect head1{500,500,10,10};
    SDL_Rect apple{100,100,10,10};
    int sizeSmallApple = 10;
    int nutriSmallAplle = 10;
    Apple smallApple(win, ren, apple,sizeSmallApple, nutriSmallAplle);
    deque<SDL_Rect> snake;

    Snake snake1(win, ren, YELLOW_COLOR,head1, dir);
    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
            run = false;
        }
            if (e.type == SDL_KEYDOWN) {
                snake1.takeInput(e);
            }
        }
        snake1.changeDir();
        snake1.checkOutScreen();


        snake1.checkApple(smallApple);

        snake1.addBody();

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        snake1.printSnake();
        snake1.checkSeflLose();
        smallApple.printApple();
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
 



