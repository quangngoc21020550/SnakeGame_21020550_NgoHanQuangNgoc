//
//  main.cpp
//  testSnake1
//
//  Created by Ngô Hán Quang Ngọc on 18/04/2022.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <ctime>
#include <vector>
#include <deque>

using namespace std;

const string WINDOW_TITLE = "SNAKE GAME";
bool systemRun = true;
bool menuRun = true;
bool gameRun = true;
bool modeMenuRun = false;
bool playAgain = false;
const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color WHITE_COLOR_FADED = {255, 255, 255, 128};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};
const SDL_Color DEFAULT_COLOR = BLACK_COLOR;
const int DEFAULT_SPEED = 40;
SDL_Color APPLE_COLOR = RED_COLOR;
SDL_Color SHIELD_COLOR = BLUE_COLOR;
SDL_Color WALL_COLOR = PURPLE_COLOR;
string playerChoice = "";
int gameSpeed = DEFAULT_SPEED;
int SCREENW = 1400;
int SCREENH = 700;
int firstSnakeLocationX = SCREENW/2;
int firstSnakeLocationY = SCREENH/2;
int firstAppleLocationX = SCREENW/2;
int firstAppleLocationY = (SCREENH/40)*30;
int firstShieldLocationX = (SCREENW/40)*30;
int firstShieldLocationY = (SCREENH/40)*30;
int snakeSize = 10;
int timeShield = 10000;
int sizeShield = 10;
int sizeSmallApple = 10;
int nutriSmallApple = 5;
int winCondition = SCREENW+SCREENH;
int increaseSpeedRate = 100;

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
        SDL_SetRenderDrawColor(renderer,0, WALL_COLOR.r, WALL_COLOR.g, WALL_COLOR.b);
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
        SDL_SetRenderDrawColor(renderer, APPLE_COLOR.r, APPLE_COLOR.g, APPLE_COLOR.b, 255);
        SDL_RenderFillRect(renderer, &location);
    }
    void printShield() {
        if (type == "shield") {
            SDL_SetRenderDrawColor(renderer, SHIELD_COLOR.r, SHIELD_COLOR.g, SHIELD_COLOR.b, 255);
            SDL_RenderFillRect(renderer, &location);
        }
    }
    void changeLocation() {
        location.x = ((rand()%SCREENW)/10)*10;
        location.y = ((rand()%SCREENH)/10)*10;
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
            apple.changeLocation();
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

    void checkSeflLose(vector<Wall> &aroundWall){
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
    void increaseSpeed() {
        if ((size-1)%increaseSpeedRate==0) gameSpeed--;
    }
    bool checkWin() {
        if(size >= winCondition) {
            SDL_Delay(100);
            return true;
        }
        return  false;
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
SDL_Texture *loadTexture(string fileName, SDL_Renderer *ren);
void renderTexture(SDL_Renderer* ren, SDL_Texture* tex, int x, int y, int w, int h);
void takeInput(SDL_Event &e, Snake &snake);
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield, Wall wall,vector<Wall> &aroundWall);
void displayMenu(SDL_Renderer *ren,SDL_Event &e, Snake &snake, Snake &menuSnake);
void resetSnake(Snake &snake, Apple &apple, bool &needShield);
void snakeRandomWalk(Snake &snake, int dir);
void pvpGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Snake &snake2, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield);
void createWallAround(Wall wall, vector<Wall> &aroundWall, Apple& apple, Apple& shield);
void printResult(SDL_Renderer* &renderer,Snake &snake, string text);
string getResult(Snake &snake);
//void printPointsOnScreen(SDL_Renderer* ren, Snake &snake);
void displayGameMode(SDL_Renderer* ren, SDL_Event &e, Snake &snake);
void printText(SDL_Renderer* ren, string text, SDL_Color textColor, int fontSize, int ratioX, int ratioY);
void resetGame(vector<Wall> &aroundWall);

int main(int argc, const char * argv[]) {
    // Khoi tao chuong trinh
    
    srand(time(0));
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    initSDL(win, ren);
    //Khoi tao cac bien tro choi
    
    SDL_Event e;
    string direction = "";
    SDL_Rect snakeHead{firstSnakeLocationX,firstSnakeLocationY,snakeSize,snakeSize};
    SDL_Rect appleSmall{firstAppleLocationX,firstAppleLocationY,sizeSmallApple,sizeSmallApple};
    SDL_Rect shieldHead{firstShieldLocationX,firstShieldLocationY,sizeShield,sizeShield};
    Wall wall(win, ren ,shieldHead, sizeSmallApple);
    vector<Wall> aroundWall;
    Apple smallApple(win, ren, appleSmall,sizeSmallApple, nutriSmallApple);
    Apple shield(win, ren, shieldHead, sizeSmallApple, 0);
    shield.type = "shield";
    deque<SDL_Rect> snake;
    Snake snake1(win, ren, YELLOW_COLOR,snakeHead, direction,true);
    snake1.player = 2;
    Snake menuSnake(win, ren, GREEN_COLOR, snakeHead, direction);
    bool needShield = false;
    //System run
    
    while (systemRun) {
    // Menu
        displayMenu(ren, e, snake1, menuSnake);
    // ChoseGameMode
        displayGameMode(ren,e,snake1);
        

    // Tro choi
        
        gameRun = true;
        resetSnake(snake1, smallApple, needShield);
        while (gameRun && systemRun && !snake1.checkWin()) {
            normalGame(ren, e, snake1, smallApple, shield, needShield, timeShield, wall, aroundWall );
        }
        
        resetGame(aroundWall);
    }
    // Ket thuc
    
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
    win = SDL_CreateWindow("SNAKE GAME", 0, 0, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
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
    TTF_Quit();
    SDL_Quit();
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
            if (e.button.x >= 540 && e.button.y >= 434 && e.button.x <= 540+320 && e.button.y <= 434+65 && menuRun)
                menuRun = false;
            if (e.button.x >= 540 && e.button.y >= 529 && e.button.x <= 540+320 && e.button.y <= 529+63 && menuRun) {
                systemRun = false;
            }
            if (e.button.x >= 235 && e.button.y >= 417 && e.button.x <= 235+231 && e.button.y <= 417+98 && modeMenuRun) {
                playerChoice = "No Wall";
                modeMenuRun = false;
            }
            if (e.button.x >= 623 && e.button.y >= 417 && e.button.x <= 623+154 && e.button.y <= 417+98 && modeMenuRun) {
                playerChoice = "Basic Wall";
                modeMenuRun = false;
            }
            if (e.button.x >= 924 && e.button.y >= 417 && e.button.x <= 924+252 && e.button.y <= 417+98 && modeMenuRun) {
                playerChoice = "Advanced Wall";
                modeMenuRun= false;
            }
        }
    }
}
void normalGame(SDL_Renderer *ren, SDL_Event &e, Snake &snake1, Apple &smallApple, Apple &shield, bool &needShield, int &timeShield, Wall wall,vector<Wall> &aroundWall) {
    takeInput(e, snake1 );
    

    snake1.changeDir();
    snake1.checkOutScreen();
    if (snake1.checkApple(smallApple)) snake1.increaseSpeed();
    
    if(snake1.checkApple(smallApple)) {
        if(rand()%3==1) {
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
    SDL_SetRenderDrawColor(ren, DEFAULT_COLOR.r, DEFAULT_COLOR.g, DEFAULT_COLOR.b, 0);
    SDL_RenderClear(ren);
    
    snake1.printSnake();
    printText(ren, to_string(snake1.size-1) + "/" +to_string(winCondition), WHITE_COLOR_FADED,40,SCREENW/2,SCREENH/4);
    if(playerChoice != "No Wall") {
        createWallAround(wall, aroundWall ,smallApple, shield);
    }
    snake1.checkSeflLose(aroundWall);
    
    if(needShield) shield.printShield();
    smallApple.printApple();
    
    SDL_RenderPresent(ren);
    SDL_Delay(gameSpeed);

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
    //SDL_Texture *menu = loadTexture("/Users/quangngoc0811/Downloads/blackbg.bmp", ren);
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
    int dir = 0;
    string text = getResult(snake);
    while (menuRun && systemRun) {
        dir = rand()%50;
        //renderTexture(ren, menu, 0, 0, SCREENW, SCREENH);
        SDL_SetRenderDrawColor(ren, DEFAULT_COLOR.r, DEFAULT_COLOR.g, DEFAULT_COLOR.b, 0);
        SDL_RenderClear(ren);
        if (!playAgain) renderTexture(ren, playIcon, positionPlayIconX, positionPlayIconY, playIconW, playIconH);
        else renderTexture(ren, againIcon, positionPlayIconX, positionPlayIconY, playIconW, playIconH);
        renderTexture(ren, exitIcon, positionExitIconX, positionExitIconY, exitIconW, exitIconH);
        snakeRandomWalk(menuSnake,dir);
        if (!playAgain) renderTexture(ren, snakeIcon, positionSnakeIconX, positionSnakeIconY, snakeIconX, snakeIconY);
        else printText(ren, text, WHITE_COLOR,40,SCREENW/2,SCREENH/3);
        SDL_RenderPresent(ren);
        takeInput(e, snake);
        SDL_Delay(60);
    }
    modeMenuRun = true;

}

void resetSnake(Snake &snake, Apple& apple, bool &needShield) {
    snake.size = 1;
    snake.head.x = SCREENW/2;
    snake.head.y = SCREENH/2;
    snake.direction = "";
    apple.location.x = firstAppleLocationX;
    apple.location.y = firstAppleLocationY;
    needShield = false;
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

    snake.printSnake();
}

void createWallAround(Wall wall, vector<Wall> &aroundWall, Apple &apple, Apple& shield) {

    for (int i = 0; i < SCREENW; i=i+10) {
        wall.location.x = i;
        wall.location.y = 0;
        aroundWall.push_back(wall);
        if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
        if(wall.location.x == shield.location.x && wall.location.y == shield.location.y) shield.changeLocation();
        wall.printWall();
    }
    for (int i = 0; i < SCREENW; i=i+10) {
        wall.location.x = i;
        wall.location.y = SCREENH-10;
        aroundWall.push_back(wall);
        if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
        wall.printWall();
    }
    for (int i = 0; i < SCREENH;i=i+10) {
        wall.location.x = 0;
        wall.location.y = i;
        aroundWall.push_back(wall);
        if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
        wall.printWall();
    }
    for (int i = 0; i < SCREENH;i=i+10) {
        wall.location.x = SCREENW-10;
        wall.location.y = i;
        aroundWall.push_back(wall);
        if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
        wall.printWall();
    }
    if(playerChoice == "Advanced Wall") {
        for (int i = 0; i < 50; i = i+10) {
            wall.location.x = (SCREENW/40)*10;
            wall.location.y = i +(SCREENH/40)*10;
            aroundWall.push_back(wall);
            if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
            wall.printWall();
        }
        for (int i = 0; i < 50; i = i+10) {
            wall.location.x = (SCREENW/40)*30;
            wall.location.y = i +(SCREENH/40)*10;
            aroundWall.push_back(wall);
            if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
            wall.printWall();
        }
        for (int i = 0; i < 50; i = i+10) {
            wall.location.x = (SCREENW/40)*10;
            wall.location.y = (SCREENH/40)*30 - i;
            aroundWall.push_back(wall);
            if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
            wall.printWall();
        }
        for (int i = 0; i < 50; i = i+10) {
            wall.location.x = (SCREENW/40)*30;
            wall.location.y = (SCREENH/40)*30-i;
            aroundWall.push_back(wall);
            if(wall.location.x == apple.location.x && wall.location.y == apple.location.y) apple.changeLocation();
            wall.printWall();
        }
    }
}

string getResult(Snake &snake) {
    string result ="";
    if (!snake.checkWin()) {
        result = "YOU LOST WITH " + to_string(snake.size-1) + " POINTS! TRY AGAIN!";
    } else {
        result = "CONGRATS! YOU ARE THE WINNER!";
    }
    return result;
}
//void printPointsOnScreen(SDL_Renderer* ren, Snake &snake) {
//    TTF_Init();
//    string points = to_string(snake.size-1);
//    TTF_Font* font = NULL;
//    font = font = TTF_OpenFont("/Users/quangngoc0811/Downloads/Pixeled.ttf", 40);
//    SDL_Surface *surface = NULL;
//    SDL_Texture *texture = NULL;
//    SDL_Color textColor = {255, 255, 255, 100};
//    surface = TTF_RenderText_Solid(font, points.c_str(), textColor);
//    texture = SDL_CreateTextureFromSurface(ren, surface);
//    SDL_FreeSurface(surface);
//    SDL_Rect srcRest;
//    SDL_Rect desRect;
//    TTF_SizeText(font,points.c_str(), &srcRest.w, &srcRest.h);
//
//    srcRest.x = 0;
//    srcRest.y =  0;
//    desRect.x = SCREENW/2-srcRest.w/2;
//    desRect.y = SCREENH/3-srcRest.h/2;
//
//    desRect.w = srcRest.w;
//    desRect.h = srcRest.h;
//    SDL_RenderCopy(ren, texture, &srcRest, &desRect);
//}

void displayGameMode(SDL_Renderer* ren, SDL_Event &e, Snake &snake) {
    while (modeMenuRun && systemRun) {
        SDL_SetRenderDrawColor(ren, DEFAULT_COLOR.r, DEFAULT_COLOR.g, DEFAULT_COLOR.b, 0);
        SDL_RenderClear(ren);
        printText(ren, "CHOOSE WALL MODE", WHITE_COLOR, 40, SCREENW/2, SCREENH/3);
        printText(ren,  "NO WALL", WHITE_COLOR, 35, SCREENW/4, 2*SCREENH/3);
        printText(ren,  "BASIC", WHITE_COLOR, 35, SCREENW/2, 2*SCREENH/3);
        printText(ren,  "ADVANCE", WHITE_COLOR, 35, 3*SCREENW/4, 2*SCREENH/3);
        takeInput(e, snake);
        SDL_RenderPresent(ren);
        SDL_Delay(60);
    }
    
}
void printText(SDL_Renderer *ren, string text, SDL_Color textColor, int fontSize, int ratioX, int ratioY) {
    TTF_Init();
    TTF_Font* font = NULL;
    font = font = TTF_OpenFont("/Users/quangngoc0811/Downloads/Pixeled.ttf", fontSize);
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    //SDL_Color textColor = {255, 255, 255, 100};
    surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    SDL_Rect srcRest;
    SDL_Rect desRect;
    TTF_SizeText(font,text.c_str(), &srcRest.w, &srcRest.h);

    srcRest.x = 0;
    srcRest.y =  0;
    desRect.x = ratioX-srcRest.w/2;
    desRect.y = ratioY-srcRest.h/2;
    desRect.w = srcRest.w;
    desRect.h = srcRest.h;
    SDL_RenderCopy(ren, texture, &srcRest, &desRect);
}

void resetGame(vector<Wall> &aroundWall) {
    playerChoice = "";
    playAgain = true;
    menuRun = true;
    modeMenuRun = true;
    aroundWall.erase(aroundWall.begin(), aroundWall.end());
    gameSpeed = DEFAULT_SPEED;
}
