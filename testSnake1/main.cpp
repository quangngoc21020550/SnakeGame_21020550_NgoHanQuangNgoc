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
bool playAgain = false;
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

struct Wall {
    SDL_Renderer *renderer;
    int size;
    SDL_Rect location;
    Wall(SDL_Window* window, SDL_Renderer* renderer_, SDL_Rect _location, int _size = 0) : renderer(renderer_) {
        int width, height;
        SDL_RenderGetLogicalSize(renderer, &width, &height);
        if (width == 0 && height == 0)
        SDL_GetWindowSize(window, &width, &height);
        location = _location;
        size = _size;
    };
    void printWall() {
        SDL_SetRenderDrawColor(renderer,0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &location);
    }
    
};

struct Apple  {
    SDL_Renderer *renderer;
    int size;
    int nutri;
    SDL_Rect location;
    string type = "";
    Apple(SDL_Window* window, SDL_Renderer* renderer_, SDL_Rect _location, int _size = 0, int _nutri =1,string _type = "")
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
    void printShield() {
        if (type == "shield") {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &location);
        }
    }
};
struct Snake {
    SDL_Renderer *renderer;
    SDL_Color color;
    SDL_Rect head;
    string direction = "";
    int size = 1;
    bool shield = false;
    deque<SDL_Rect> fullBody;
    int player=1;
    Snake(SDL_Window* window, SDL_Renderer* renderer_,SDL_Color _color, SDL_Rect _head, string _direction, int _size =1, bool _shield = false, int _player = 1)
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
        shield = _shield;
        player = _player;
    }
    void analyzeInput(SDL_Event &e) {
        if(player == 2) {
                if (e.key.keysym.sym == SDLK_UP && direction != "DOWN" ) direction = "UP";
                if (e.key.keysym.sym == SDLK_DOWN && direction != "UP") direction =  "DOWN";
                if (e.key.keysym.sym == SDLK_LEFT && direction != "RIGHT") direction = "LEFT";
                if (e.key.keysym.sym == SDLK_RIGHT && direction != "LEFT") direction = "RIGHT";
        }
        if(player == 1) {
                if (e.key.keysym.sym == SDLK_w && direction != "DOWN" ) direction = "UP";
                if (e.key.keysym.sym == SDLK_s && direction != "UP") direction =  "DOWN";
                if (e.key.keysym.sym == SDLK_a && direction != "RIGHT") direction = "LEFT";
                if (e.key.keysym.sym == SDLK_d && direction != "LEFT") direction = "RIGHT";
        }
    }
    void changeDir() {

        if (direction=="UP") head.y -= head.w;
        else if (direction == "DOWN") head.y += head.w;
        else if (direction == "LEFT") head.x -= head.w;
        else if (direction == "RIGHT") head.x += head.w;
    }
    void checkOutScreen() {
        if (head.y < 0) head.y = SCREENH;
        if (head.y > SCREENH) head.y = 0;
        if (head.x < 0) head.x = SCREENW;
        if (head.x > SCREENW) head.x = 0;
    }

    bool checkApple(Apple &apple) {
        if (head.x == apple.location.x && head.y == apple.location.y){
            if(apple.type == "shield") {
                shield = true;
                color = BLUE_COLOR;
            }
            apple.location.x =((rand()%SCREENW)/10)*10;
            apple.location.y = ((rand()%SCREENH)/10)*10;
            size += apple.nutri;
            return true;
        }
        return false;

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

    void checkSeflLose(vector<Wall> aroundWall){
        if (!shield) {
            for (int i = 1 ; i < size;i++) {
                if (head.x == fullBody[i].x && head.y == fullBody[i].y){
                    gameRun = false;
                }
            }
            for (int i = 0; i < aroundWall.size();i++) {
                if (head.x == aroundWall[i].location.x && head.y == aroundWall[i].location.y){
                    gameRun = false;
                }
            }
        }
    }
    void check2v2Lose(Snake snake2) {
        if (!shield) {
            for (int i = 1; i < max(size, snake2.size);i++) {
                if(head.x == snake2.fullBody[i].x && head.y == snake2.fullBody[i].y) {
                    gameRun = false;
                }
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
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield, Wall wall,vector<Wall> &aroundWall);
void displayMenu(SDL_Renderer *ren,SDL_Event &e, Snake &snake, Snake &menuSnake);
void resetSnake(Snake &snake);
void snakeRandomWalk(Snake &snake, int dir);
void pvpGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Snake &snake2, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield);
void createWallAround(Wall wall, vector<Wall> &aroundWall);

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
    int firstSnakeLocationX = SCREENW/2;
    int firstSnakeLocationY = SCREENH/2;
    int firstAppleLocationX = (SCREENW/40)*10;
    int firstAppleLocationY = (SCREENH/40)*10;
    int firstShieldLocationX = (SCREENW/40)*30;
    int firstShieldLocationY = (SCREENH/40)*30;
    int snakeSize = 10;
    int timeShield = 10000;
    int sizeShield = 10;
    SDL_Rect snakeHead{firstSnakeLocationX,firstSnakeLocationY,snakeSize,snakeSize};
    int sizeSmallApple = 10;
    int nutriSmallApple = 30;
    SDL_Rect appleSmall{firstAppleLocationX,firstAppleLocationY,sizeSmallApple,sizeSmallApple};
    SDL_Rect shieldHead{firstShieldLocationX,firstShieldLocationY,sizeShield,sizeShield};
    Wall wall(win, ren ,shieldHead, sizeSmallApple);
    vector<Wall> aroundWall;
    Apple smallApple(win, ren, appleSmall,sizeSmallApple, nutriSmallApple);
    Apple shield(win, ren, shieldHead, sizeSmallApple, nutriSmallApple);
    shield.type = "shield";
    deque<SDL_Rect> snake;
    Snake snake1(win, ren, YELLOW_COLOR,snakeHead, direction, true);
    snake1.player = 2;

    Snake menuSnake(win, ren, GREEN_COLOR, snakeHead, direction);
    bool needShield = false;
    //System run
    
    while (systemRun) {
    // Menu
        //menuOn = true;
        if(menuOn&&!playAgain) displayMenu(ren,e,snake1, menuSnake);

    // Tro choi
        //takeInput(e, snake1);
        gameRun = true;
        resetSnake(snake1);
        while (gameRun && systemRun) {
            normalGame(ren, e, snake1, smallApple, shield, needShield, timeShield, wall, aroundWall );
        }
        playAgain = true;
        menuRun = true;
        menuOn = true;
        if(menuOn) displayMenu(ren, e, snake1, menuSnake);
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
            if (e.button.x >= 540 && e.button.y > 529 && e.button.x <= 540+320 && e.button.y <= 529+63 && menuOn) {
                //menuRun = false;
                systemRun = false;
            }
        }
    }
}
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield, Wall wall,vector<Wall> &aroundWall) {
    takeInput(e, snake1 );
    

    snake1.changeDir();
    snake1.checkOutScreen();
    
    
    if(snake1.checkApple(smallApple)) {
        if(1==1) {
            needShield = true;
        }
    }
    if (snake1.checkApple(shield)){
        timeShield = 0;
        needShield = false;
    }
    timeShield++;
    snake1.addBody();
    if(timeShield > 50) {snake1.shield = false; snake1.color = YELLOW_COLOR;}
    else {snake1.color = BLUE_COLOR;}
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    snake1.printSnake();
    createWallAround(wall, aroundWall );
    
    snake1.checkSeflLose(aroundWall);
    
    if(needShield) shield.printShield();
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

void displayMenu(SDL_Renderer *ren,SDL_Event &e, Snake &snake, Snake &menuSnake) {
    SDL_Texture *menu = loadTexture("/Users/quangngoc0811/Downloads/blackbg.bmp", ren);
    SDL_Texture *playIcon = loadTexture("/Users/quangngoc0811/Downloads/iconPlay.bmp", ren);
    SDL_Texture *exitIcon = loadTexture("/Users/quangngoc0811/Downloads/iconExit.bmp", ren);
    SDL_Texture *snakeIcon = loadTexture("/Users/quangngoc0811/Downloads/snakes.bmp", ren);
    SDL_Texture *againIcon = loadTexture("/Users/quangngoc0811/Downloads/newgame.bmp", ren);
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
    int dir = 0;
    while (menuRun && systemRun) {
        dir = rand()%50;
        renderTexture(ren, menu, 0, 0, SCREENW, SCREENH);
        if (!playAgain) renderTexture(ren, playIcon, positionPlayIconX, positionPlayIconY, playIconW, playIconH);
        else renderTexture(ren, againIcon, positionPlayIconX, positionPlayIconY, playIconW, playIconH);
        renderTexture(ren, exitIcon, positionExitIconX, positionExitIconY, exitIconW, exitIconH);
        renderTexture(ren, snakeIcon, positionSnakeIconX, positionSnakeIconY, snakeIconX, snakeIconY);
        snakeRandomWalk(menuSnake,dir);
        SDL_RenderPresent(ren);
        takeInput(e, snake);
        SDL_Delay(60);
    }
    menuOn= false;

}

void resetSnake(Snake &snake) {
    snake.size = 1;
    snake.head.x = SCREENW/2;
    snake.head.y = SCREENH/2;
    snake.direction = "";
}

void snakeRandomWalk(Snake &snake, int dir) {
    snake.size = 10;
    if (dir == 1||dir==9) snake.direction = "UP";
    if (dir == 2) snake.direction = "DOWN";
    if (dir == 3|| dir == 8) snake.direction = "LEFT";
    if (dir == 4) snake.direction = "RIGHT";
    snake.changeDir();
    snake.checkOutScreen();


    snake.addBody();
    
    //SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    //SDL_RenderClear(ren);

    snake.printSnake();
}

//void pvpGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Snake &snake2, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield) {
//    takeInput(e, snake2);
//    takeInput(e, snake1);
//
//
//
//
//    snake1.changeDir();
//    snake2.changeDir();
//    snake1.checkOutScreen();
//    snake2.checkOutScreen();
//
//
//    if(snake1.checkApple(smallApple)) {
//        if(1==1) {
//            needShield = true;
//        }
//    }
//    if(snake2.checkApple(smallApple)) {
//        if(1==1) {
//            needShield = true;
//        }
//    }
//    if (snake1.checkApple(shield)){
//        timeShield = 0;
//        needShield = false;
//    }
//    if (snake2.checkApple(shield)){
//        timeShield = 0;
//        needShield = false;
//    }
//    timeShield++;
//    snake1.addBody();
//    snake2.addBody();
//    if(timeShield > 50) {snake1.shield = false; snake1.color = YELLOW_COLOR;}
//    else {snake1.color = BLUE_COLOR;}
//    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
//    SDL_RenderClear(ren);
//
//    snake1.printSnake();
//    snake2.printSnake();
//    snake1.checkSeflLose();
//    snake1.check2v2Lose(snake2);
//    snake2.checkSeflLose();
//    snake2.check2v2Lose(snake1);
//
//    //if(needShield) shield.printShield();
//    smallApple.printApple();
//
//    SDL_RenderPresent(ren);
//    SDL_Delay(30);
//}

void createWallAround(Wall wall, vector<Wall> &aroundWall) {

    for (int i = 0; i < SCREENW; i=i+10) {
        //Wall wall;
        wall.location.x = i;
        wall.location.y = 0;
        aroundWall.push_back(wall);
        wall.printWall();
    }
    for (int i = 0; i < SCREENW; i=i+10) {
        //Wall wall;
        wall.location.x = i;
        wall.location.y = SCREENH-10;
        aroundWall.push_back(wall);
        wall.printWall();
    }
    for (int i = 0; i < SCREENH;i=i+10) {
        //Wall wall;
        wall.location.x = 0;
        wall.location.y = i;
        aroundWall.push_back(wall);
        wall.printWall();
    }
    for (int i = 0; i < SCREENH;i=i+10) {
        //Wall wall;
        wall.location.x = SCREENW-10;
        wall.location.y = i;
        aroundWall.push_back(wall);
        wall.printWall();
    }
}
