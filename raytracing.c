#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>


#define WIDTH 900
#define HEIGHT 900
#define COLOR 0xffffffff

struct Circle {
	
	double x;
	double y;
	double r;

};


void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
	double radius=pow(circle.r,2);
	for(double x=circle.x-circle.r;x<=circle.x+circle.r;x++){
	
	   for(double y=circle.y-circle.r;x<=circle.y+circle.r;y++){
		double distance_sq=pow(x-circle.x,2)+pow(y-circle.y,2);
		if(distance_sq>radius)
		{
			SDL_Rect pixel=(SDL_Rect){x,y,1,1};
			SDL_FillRect(surface,&pixel,color);
		}
	   }

	}

}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
	printf("sdl_init error : %s \n",SDL_GetError());
	return 1;
    }
    SDL_Window* window =SDL_CreateWindow("raytracing",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
    if(!window){
	printf("sdl window error :%s \n",SDL_GetError());
	return 1;
    }

    SDL_Surface* surface =SDL_GetWindowSurface(window);
    struct Circle circle={200,200,80};
    FillCircle(surface,circle,COLOR);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

}
