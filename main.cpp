#include <SDL2/SDL.h>
#include <iostream>
#include<cmath>
#define WIDTH 1200
#define HEIGHT 800
#define SPEED 9
#define BALL_SPEED 6
#define SIZE 16
#define COL 8
#include <SDL2/SDL_mixer.h>
#define PI 3.14
#define FONT_SIZE 32
#define SPACING 16
#include<string>
#include <SDL2/SDL_image.h>
#include <fstream>
#include<SDL2/SDL_ttf.h>
using namespace std;

TTF_Font* font;
SDL_Window *s_window;
SDL_Renderer *s_renderer;
bool running;
int frameCount, timerFPS, lastFrame, fps;
SDL_Color color;
SDL_Rect paddle, ball, lives, brick,ball_2;
float velY, velX,velX_2,velY_2;
int liveCount;
bool bricks[1000*COL];
void write(std::string text, int x, int y)
 {
SDL_Surface *surface;
SDL_Texture *texture;
const char* t=text.c_str();
surface = TTF_RenderText_Solid(font, t, color);
texture = SDL_CreateTextureFromSurface(s_renderer, surface);
lives.w=surface->w;
lives.h=surface->h;
lives.x=x-lives.w;
lives.y=y-lives.h;
SDL_FreeSurface(surface);
SDL_RenderCopy(s_renderer, texture, NULL, &lives);
SDL_DestroyTexture(texture);
}




void save(string name);


void resetBricks(bool level,int ROW)
{
for(int i=0; i<COL*ROW; i++)
{
    bricks[i]=1;
}
liveCount=3;
paddle.x=(WIDTH/2)-(paddle.w/3);
ball.y=paddle.y-(paddle.h*4);
ball.x=paddle.x;
velY=BALL_SPEED/2;
velX=0;
if(level)
{
    ball_2.x=WIDTH-(SIZE/2);
ball_2.y=paddle.y;
ball_2.x=paddle.x+20;
velY_2=BALL_SPEED/2;
velX_2=0;
ball_2.x=WIDTH/2-(SIZE/2);
}

}

void setBricks(int i,int ROW)
 {
 brick.x=(((i%COL)+1)*SPACING)+((i%COL)*brick.w)-(SPACING/2);
 brick.y=brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*brick.h)-(SPACING/2);
}


void update(bool level,int ROW) {
 if(liveCount<=0)
 {     resetBricks(level,ROW);


 }
 if(SDL_HasIntersection(&ball, &paddle))
{
  double rel=(paddle.x+(paddle.w/2))-(ball.x+(SIZE/2));
  double norm=rel/(paddle.w/2);
  double bounce = norm* (5*PI/12);
  velY=-BALL_SPEED*cos(bounce);
  velX=BALL_SPEED*-sin(bounce);
 }
 if(ball.y<=0) velY=-velY;
 if(ball.y+SIZE>=HEIGHT) {velY=-velY;liveCount--;}
 if(ball.x<=0 || ball.x+SIZE>=WIDTH) velX=-velX;
 ball.x+=velX;
 ball.y+=velY;
 if(level)
 {
     if(SDL_HasIntersection(&ball_2, &paddle))
{
  double rel_2=(paddle.x+(paddle.w/2))-(ball_2.x+(SIZE/2));
  double norm_2=rel_2/(paddle.w/2);
  double bounce_2 = norm_2* (5*PI/12);
  velY_2=-BALL_SPEED*cos(bounce_2);
  velX_2=BALL_SPEED*-sin(bounce_2);
 }
 if(ball_2.y<=0) velY_2=-velY_2;
 if(ball_2.y+SIZE>=HEIGHT) {velY_2=-velY_2;liveCount--;}
 if(ball_2.x<=0 || ball_2.x+SIZE>=WIDTH) velX_2=-velX_2;
 ball_2.x+=velX_2;
 ball_2.y+=velY_2;
 }

 if(paddle.x<0) paddle.x=0;
 if(paddle.x+paddle.w>WIDTH) paddle.x=WIDTH-paddle.w;

 bool reset=1;
 for(int i=0; i<COL*ROW; i++) {
  setBricks(i,ROW);
  if(SDL_HasIntersection(&ball, &brick) && bricks[i])
    {
   bricks[i]=0;
   if(ball.x >= brick.x) {velX=-velX; ball.x-=20;}
   if(ball.x <= brick.x) {velX=-velX; ball.x+=20;}
   if(ball.y <= brick.y) {velY=-velY; ball.y-=20;}
   if(ball.y >= brick.y) {velY=-velY; ball.y+=20;}
  }
  if(level)
  {
      if(SDL_HasIntersection(&ball_2, &brick) && bricks[i])
    {
   bricks[i]=0;
   if(ball_2.x >= brick.x) {velX_2=-velX_2; ball_2.x-=20;}
   if(ball_2.x <= brick.x) {velX_2=-velX_2; ball_2.x+=20;}
   if(ball_2.y <= brick.y) {velY_2=-velY_2; ball_2.y-=20;}
   if(ball_2.y >= brick.y) {velY_2=-velY_2; ball_2.y+=20;}
  }
  }

  if(bricks[i]==1)
  {reset=0;}
 }
 if(reset)
 {    running=0;
     //resetBricks(level,ROW);
 }


}

void input() {
SDL_Event e;
const Uint8 *keystates = SDL_GetKeyboardState(NULL);
while(SDL_PollEvent(&e)) if(e.type==SDL_QUIT) running=false;
if(keystates[SDL_SCANCODE_ESCAPE]) running=false;
if(keystates[SDL_SCANCODE_A]) paddle.x-=SPEED;
if(keystates[SDL_SCANCODE_D]) paddle.x+=SPEED;

}
void backG()
{
frameCount++;
timerFPS=SDL_GetTicks()-lastFrame;
if(timerFPS<(1000/60))
    {
 SDL_Delay((1000/60)-timerFPS);
}

SDL_SetRenderDrawColor(s_renderer, 0,0 ,0 , 255);
SDL_RenderClear(s_renderer);
SDL_Texture*img;
SDL_Rect texr;

img=IMG_LoadTexture(s_renderer,"space2.jpg");
    int w_2,h_2;
    SDL_QueryTexture(img,NULL,NULL,&w_2,&h_2);

    texr.x=0;
    texr.y=0;
    texr.w=1200;
    texr.h=800;
    SDL_RenderCopy(s_renderer,img,NULL,&texr);
     SDL_RenderPresent(s_renderer);

}




void render(int r_ball,int g_ball,int b_ball,bool level,int ROW)
{
frameCount++;
timerFPS=SDL_GetTicks()-lastFrame;
if(timerFPS<(1000/60))
    {
 SDL_Delay((1000/60)-timerFPS);
}


//SDL_RenderClear(s_renderer);
backG();

SDL_SetRenderDrawColor(s_renderer, r_ball, g_ball,b_ball, 255);
SDL_RenderFillRect(s_renderer, &ball);
if(level)
    {
SDL_SetRenderDrawColor(s_renderer, r_ball, g_ball,b_ball, 255);
        SDL_RenderFillRect(s_renderer, &ball_2);
}
write(std::to_string(liveCount), WIDTH/2+FONT_SIZE/2, FONT_SIZE*1.5);
SDL_SetRenderDrawColor(s_renderer, 255, 255,255, 255);
SDL_RenderFillRect(s_renderer, &paddle);

for(int i=0; i<COL*ROW; i++) {
 SDL_SetRenderDrawColor(s_renderer,14,58, 79, 255);
 if(i%2==0)SDL_SetRenderDrawColor(s_renderer,127,140, 130, 255);
 if(bricks[i]) {

  setBricks(i,ROW);
  SDL_RenderFillRect(s_renderer, &brick);
 }
}
SDL_RenderPresent(s_renderer);
}
void run(int r_ball,int  g_ball,int b_ball,bool level,int ROW)
{
     update(level,ROW);
 input();
 render(r_ball, g_ball,b_ball,level,ROW);
}
void present_3()
{
      SDL_SetRenderDrawColor(s_renderer, 0,0 ,0 , 255);
SDL_RenderClear(s_renderer);
        SDL_Texture*img;
    img=IMG_LoadTexture(s_renderer,"3_3.png");
    int w,h;
    SDL_QueryTexture(img,NULL,NULL,&w,&h);
    SDL_Rect texr;
    texr.x=0;
    texr.y=0;
    texr.w=1200;
    texr.h=800;
    SDL_RenderCopy(s_renderer,img,NULL,&texr);
    SDL_RenderPresent(s_renderer);
    SDL_Delay(1000);
}
void present_2()
{
      SDL_SetRenderDrawColor(s_renderer, 0,0 ,0 , 255);
SDL_RenderClear(s_renderer);
        SDL_Texture*img;
    img=IMG_LoadTexture(s_renderer,"2_2.png");
    int w,h;
    SDL_QueryTexture(img,NULL,NULL,&w,&h);
    SDL_Rect texr;
    texr.x=0;
    texr.y=0;
    texr.w=1200;
    texr.h=800;
    SDL_RenderCopy(s_renderer,img,NULL,&texr);
    SDL_RenderPresent(s_renderer);
    SDL_Delay(1000);
}
void present_1()
{
      SDL_SetRenderDrawColor(s_renderer, 0,0 ,0 , 255);
SDL_RenderClear(s_renderer);
        SDL_Texture*img;
    img=IMG_LoadTexture(s_renderer,"1_1.png");
    int w,h;
    SDL_QueryTexture(img,NULL,NULL,&w,&h);
    SDL_Rect texr;
    texr.x=0;
    texr.y=0;
    texr.w=1200;
    texr.h=800;
    SDL_RenderCopy(s_renderer,img,NULL,&texr);
    SDL_RenderPresent(s_renderer);
    SDL_Delay(1000);
}





int main(int argv, char** args)
{int r_ball, g_ball,b_ball,row;
bool level;
TTF_Init();
font = TTF_OpenFont("font.ttf", FONT_SIZE);
string ballcolour,Level,name;
cout<<"please enter your name:"<<endl;
cin>>name;
cout<<"please choose a colour(red/green/blue)"<<endl;
cin>>ballcolour;
cout<<endl<<"easy/medium/hard:"<<endl;
cin>>Level;


if(ballcolour=="red")
{
    r_ball=255;
    g_ball=0;
    b_ball=0;
}
if(ballcolour=="green")
{
    r_ball=0;
    g_ball=255;
    b_ball=0;
}
if(ballcolour=="blue")
{
    r_ball=0;
    g_ball=0;
    b_ball=255;
}
if(Level=="easy")
{
    level=0;
    paddle.w=WIDTH/6;
}
if(Level=="medium")
{
    paddle.w=WIDTH/8;
}
else if(Level=="hard")
{
    level=1;
    paddle.w=WIDTH/8;
}
if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &s_window, &s_renderer) < 0) std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
SDL_SetWindowTitle(s_window, "mina salehani-soraya charkas");
running = 1;
static int lastTime=0;
color.r=color.g=color.b=255;
paddle.h=12;
paddle.y=HEIGHT-paddle.h-32;
ball.w=ball.h=SIZE;
ball_2.w=ball_2.h=SIZE;
brick.w=(WIDTH-(SPACING*COL))/COL;
brick.h=40;
int ROW=3;
bool run_2=true;
resetBricks(level,ROW);
bool quit =false;
present_3();
present_2();
present_1();
while(run_2)
{
            if(running)
{
lastFrame=SDL_GetTicks();
if(lastFrame>=lastTime+1000)
{
lastTime=lastFrame;
fps=frameCount;
frameCount=0;}
run(r_ball, g_ball,b_ball,level,ROW);
}
else
{
    ROW++;
    running=true;
    resetBricks(level,ROW);

}
}
SDL_DestroyRenderer(s_renderer);
SDL_DestroyWindow(s_window);
SDL_Quit();
return 0;
}


