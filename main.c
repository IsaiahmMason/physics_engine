#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define ARGB(r,g,b,a) ((((uint32_t)(r)) << 24) | (((uint32_t)(g)) << 16) | (((uint32_t)(b)) << 8) | ((uint32_t)(a)))

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    float restitution;
} CircleBody;

void update_physics(CircleBody* body, float dt){
    const float gravity = 500.0f;

    body->vy += gravity * dt;

    body->x += body->vx * dt;
    body->y += body->vy *dt;

    if(body->y + body->radius > SCREEN_HEIGHT){
        body->y = SCREEN_HEIGHT - body->radius;
        body->vy = -body->vy * body->restitution;
    }

    if(body->x - body->radius < 0){
        body->x = body->radius;
        body->vx = -body->vx * body->restitution;
    }

    if(body->x + body->radius > SCREEN_WIDTH){
        body->x = SCREEN_WIDTH - body->radius;
        body->vx = -body->vx * body->restitution;
    }
}

void draw_circle(SDL_Renderer* renderer, CircleBody* body){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(int w = 0; w < body->radius * 2; w++){
        for(int h = 0; h < body->radius * 2; h++){
            float dx = body->radius - w;
            float dy = body->radius - h;
            if((dx*dx + dy*dy) <= (body->radius * body->radius)){
                SDL_RenderDrawPoint(renderer, body->x + dx, body->y + dy);
            }
        }
    }
}

int main(int argc, char* argv[]){
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Physics Engine 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    CircleBody ball = {SCREEN_WIDTH / 2.0f, 50.0f, 150.0f, 0.0f, 20.0f, 0.75f};

    uint64_t last_time = SDL_GetPerformanceCounter();
    float frequency = (float)SDL_GetPerformanceFrequency();

    bool running = true;
    SDL_Event event;

    while(running){
        uint64_t current_time = SDL_GetPerformanceCounter();
        float dt = (current_time - last_time) / frequency;
        last_time = current_time;

        if(dt > 0.1f) dt = 0.1f;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
        }

        update_physics(&ball, dt);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_circle(renderer, &ball);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}