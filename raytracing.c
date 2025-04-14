#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 900
#define COLOR 0xffffffff
#define BLACK 0x00000000
#define NUM 100

struct Circle {
    double x;
    double y;
    double r;
};

struct Ray {
    double x_str ,y_str;
    double angle;
    
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    double radius_sq = pow(circle.r, 2);
    for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
            double distance_sq = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_sq <= radius_sq) {
                if (x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[(y * surface->w) + x] = color;
                }
            }
        }
    }
}

void gen_ray(struct Circle circle, struct Ray rays[NUM])
{
   for(int i=0;i<NUM;i++)
   {
      double angle=((double)i/NUM)*2* M_PI;
      struct Ray ray={circle.x,circle.y,angle};
      rays[i]=ray;
   }
}

void FillRays(SDL_Surface* surface, struct Ray rays[NUM],Uint32 color, struct Circle object)
{ 
    double r_sq=pow(object.r,2);
    for(int i=0; i<NUM ;i++)
    {
	struct Ray ray =rays[i];
	int eos=0;
	int oh=0;
	double step =1;
	double x_draw=ray.x_str;
        double y_draw=ray.y_str;
	while(!eos && !oh)
	{
	    x_draw += step*cos(ray.angle);
            y_draw += step*sin(ray.angle);
	    SDL_Rect pixel =(SDL_Rect){x_draw,y_draw,1,1};
	    SDL_FillRect(surface, &pixel,color);
	    if(x_draw<0||x_draw>WIDTH)
		eos=1;
	    if(y_draw<0|| y_draw >HEIGHT)
		eos=0;
            double distance_sq = pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);
            if (distance_sq < r_sq){
		break;
	    }
	}
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    struct Circle circle = {200, 200, 80};
    struct Circle shadow = {650,300,140};
    struct Ray rays[NUM];
    SDL_Rect erase={0,0,WIDTH,HEIGHT};
    gen_ray(circle ,rays);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
	    if(e.type == SDL_MOUSEMOTION && e.motion.state !=0)
	    {
	    	circle.x=e.motion.x;
                circle.y=e.motion.y;
		gen_ray(circle,rays);
	    }
        }
	SDL_FillRect(surface,&erase ,BLACK);
	FillCircle(surface, circle, COLOR);
	FillCircle(surface, shadow, COLOR);
	FillRays(surface, rays, COLOR,shadow);
	SDL_UpdateWindowSurface(window);
        SDL_Delay(10); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
