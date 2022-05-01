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

bool systemRun = true;
bool menuRun = true;
bool gameRun = true;
bool menuOn = true;
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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &location);
    }
};
struct Snake {
    SDL_Renderer *renderer;
    SDL_Color color;
    SDL_Rect head;
    string direction = "";
    int size = 1;
    deque<SDL_Rect> fullBody;
    Snake(SDL_Window* window, SDL_Renderer* renderer_,SDL_Color _color, SDL_Rect _head, string _direction, int _size =1)
     : renderer(renderer_)
    {
         int width, height;
         SDL_RenderGetLogicalSize(renderer, &width, &height);
         if (width == 0 && height == 0)
         SDL_GetWindowSize(window, &width, &height);
         head = _head;
        direction = _direction;
        size = _size;
        color = _color;
    }
    void analyzeInput(SDL_Event &e) {
                if (e.key.keysym.sym == SDLK_UP && direction != "DOWN" ) direction = "UP";
                if (e.key.keysym.sym == SDLK_DOWN && direction != "UP") direction =  "DOWN";
                if (e.key.keysym.sym == SDLK_LEFT && direction != "RIGHT") direction = "LEFT";
                if (e.key.keysym.sym == SDLK_RIGHT && direction != "LEFT") direction = "RIGHT";
    }
    void changeDir() {

        if (direction=="UP") head.y -= 10;
        else if (direction == "DOWN") head.y +=10;
        else if (direction == "LEFT") head.x -=10;
        else if (direction == "RIGHT") head.x += 10;
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
                gameRun = false;
            }
        }
    }
};
void logErr(string mess, bool err = false);
void initSDL(SDL_Window* &win, SDL_Renderer* &ren);
void quitSDL(SDL_Window* win, SDL_Renderer* ren);
void closePrg(int &dir, bool &run , SDL_Event e);
SDL_Texture *loadTexture(string fileName, SDL_Renderer *ren);
void renderTexture(SDL_Renderer* ren, SDL_Texture* tex, int x, int y, int w, int h);
void takeInput(SDL_Event &e, Snake &snake);
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple);
void displayMenu(SDL_Renderer *ren,SDL_Event &e, Snake &snake);
void resetSnake(Snake &snake);

int main(int argc, const char * argv[]) {
    // Khoi tao chuong trinh
    
    srand(time(0));
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    initSDL(win, ren);
    //Khoi tao cac bien tro choi
    string playerChoice = "";
    SDL_Event e;
    string direction = "";
    SDL_Rect head1{500,500,10,10};
    SDL_Rect apple{100,100,10,10};
    int sizeSmallApple = 10;
    int nutriSmallAplle = 30;
    Apple smallApple(win, ren, apple,sizeSmallApple, nutriSmallAplle);
    deque<SDL_Rect> snake;
    Snake snake1(win, ren, YELLOW_COLOR,head1, direction);
    while (systemRun) {
    // Menu
        //menuOn = true;
        displayMenu(ren,e,snake1);

    // Tro choi
        //takeInput(e, snake1);
        gameRun = true;
        resetSnake(snake1);
        while (gameRun && systemRun) {
            normalGame(ren,e,snake1,smallApple);
        }
        menuRun = true;
        displayMenu(ren, e, snake1);
    }
    // Ket thuc
    
    
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
        gameRun = false;
    }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP && dir !=2 ) dir = 1;
            if (e.key.keysym.sym == SDLK_DOWN && dir != 1) dir = 2;
            if (e.key.keysym.sym == SDLK_LEFT && dir != 4) dir = 3;
            if (e.key.keysym.sym == SDLK_RIGHT && dir !=3) dir = 4;
            
        }
    }
}
void takeInput(SDL_Event &e, Snake &snake) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            systemRun = false;
        }
        if (e.type == SDL_KEYDOWN) {
            snake.analyzeInput(e);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.x >= 540 && e.button.y > 434 && e.button.x <= 540+320 && e.button.y <= 434+65 && menuOn)
                menuRun = false;
            if (e.button.x >= 540 && e.button.y > 529 && e.button.x <= 540+320 && e.button.y <= 529+63) {
                //menuRun = false;
                systemRun = false;
            }
        }
    }
}
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple) {
    takeInput(e, snake1 );
    
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

SDL_Texture *loadTexture(string fileName, SDL_Renderer *ren) {
    SDL_Texture *tex = nullptr;
    SDL_Surface *img = SDL_LoadBMP(fileName.c_str());
    if (img != nullptr) {
        tex = SDL_CreateTextureFromSurface(ren, img);
    }
    return tex;
}
void renderTexture(SDL_Renderer* ren, SDL_Texture* tex, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void displayMenu(SDL_Renderer *ren,SDL_Event &e, Snake &snake) {
    SDL_Texture *menu = loadTexture("/Users/quangngoc0811/Downloads/blackbg.bmp", ren);
    SDL_Texture *playIcon = loadTexture("/Users/quangngoc0811/Downloads/iconPlay.bmp", ren);
    SDL_Texture *exitIcon = loadTexture("/Users/quangngoc0811/Downloads/iconExit.bmp", ren);
    SDL_Texture *snakeIcon = loadTexture("/Users/quangngoc0811/Downloads/snakes.bmp", ren);
    int playIconW;
    int playIconH;
    SDL_QueryTexture(playIcon, NULL, NULL, &playIconW, &playIconH);
    int exitIconW;
    int exitIconH;
    SDL_QueryTexture(exitIcon, NULL, NULL, &exitIconW, &exitIconH);
    int positionPlayIconX =  SCREENW/2 - playIconW/2;
    int positionPlayIconY = 2*SCREENH/3 - playIconH/2;
    int positionExitIconX =  SCREENW/2 - exitIconW/2;
    int positionExitIconY = 4*SCREENH/5 - exitIconH/2;
    int snakeIconX, snakeIconY;
    SDL_QueryTexture(snakeIcon, NULL, NULL, &snakeIconX, &snakeIconY);
    int positionSnakeIconX = SCREENW/2 - snakeIconX/2;
    int positionSnakeIconY = SCREENH/3 - snakeIconY/2;
    cout << playIconW << endl;
    cout << playIconH << endl;
    cout << positionPlayIconX << endl;
    cout << positionPlayIconY << endl;
    cout << exitIconW << endl;
    cout << exitIconH << endl;
    cout << positionExitIconX << endl;
    cout << positionExitIconY << endl;
    while (menuRun && systemRun) {
        renderTexture(ren, menu, 0, 0, SCREENW, SCREENH);
        renderTexture(ren, playIcon, positionPlayIconX, positionPlayIconY, playIconW, playIconH);
        renderTexture(ren, exitIcon, positionExitIconX, positionExitIconY, exitIconW, exitIconH);
        renderTexture(ren, snakeIcon, positionSnakeIconX, positionSnakeIconY, snakeIconX, snakeIconY);
        SDL_RenderPresent(ren);
        takeInput(e, snake);
    }
    //menuOn= false;

}

void resetSnake(Snake &snake) {
    snake.size = 1;
    snake.head.x = 500;
    snake.head.y = 500;
    snake.direction = "";
}
