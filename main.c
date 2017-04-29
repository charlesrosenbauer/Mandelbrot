#include <SDL/SDL.h>










typedef struct{
  float r, i;
}Cpx;










Cpx mul(Cpx a, Cpx b){
  Cpx c;
  c.r = (a.r * b.r) - (a.i * b.i);
  c.i = (a.i * b.r) + (a.r * b.i);
  return c;
}










Cpx add(Cpx a, Cpx b){
  a.r += b.r;
  a.i += b.i;
  return a;
}










Cpx cpx (float r, float i){
  Cpx ret;
  ret.r = r;
  ret.i = i;
  return ret;
}










float difMag(Cpx c, float m){
  return (m * m) - ((c.r * c.r) + (c.i * c.i));
}










uint32_t getColor(int n){
  uint32_t r = 0, g = 0, b = 0;
  if(n < 256)
    r = n;
  else if(n < 512){
    n -= 256;
    g = n; r = 255 - n;
  }else if(n < 768){
    n -= 512;
    b = n; g = 255 - n;
  }else if(n < 1024){
    n -= 768;
    r = n; b = 255 - n;
  }
  return (b << 16) | (g << 8) | r;
}










int HEIGHT = 8192;
int WIDTH  = 8192;










int getIndex(int x, int y){
  return (x * HEIGHT) + y;
}










int main(){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);

  int n = 1024;
  int scl = 1;

  Cpx* fns = malloc(sizeof(Cpx) * HEIGHT * WIDTH);
  for(int x = 0; x < HEIGHT; x++)
    for(int y = 0; y < WIDTH; y++)
      fns[getIndex(x, y)] = cpx((x / (HEIGHT / 4.0))-2, (y / (WIDTH / 4.0))-2);

  uint32_t* px = (uint32_t*)screen->pixels;
  int pnum = 0;
  for(int x = 0; x < HEIGHT; x++){
    for(int y = 0; y < WIDTH; y++){

      Cpx val = cpx(0, 0);
      for(int it = 0; it < n; it++){
        val = add(mul(val, val), fns[getIndex(x, y)]);
        if(difMag(val, 2) < 0){
          px[pnum] = getColor(it * scl);
          break;
        }
      }
      pnum++;
    }
  }

  SDL_SaveBMP(screen, "mandelbrot.bmp");
  //SDL_Flip(screen);
  //SDL_Delay(5000);
  SDL_Quit();
  return 0;
}
