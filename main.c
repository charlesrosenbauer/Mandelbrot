#include <SDL/SDL.h>










typedef struct{
  float r, i;
}Cpx;










Cpx mul(Cpx a, Cpx b){
  Cpx c;
  c.r = (a.r * b.r) - (a.i * b.i);
  c.i = 2 * a.i * b.i;
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
  return ((c.r * c.r) + (c.i * c.i)) - (m * m);
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










int main(){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, 0);

  int n = 128;
  int scl = 32;

  Cpx fns [512][512];
  for(int x = 0; x < 512; x++)
    for(int y = 0; y < 512; y++)
      fns[x][y] = cpx((x / 128.0)-2, (y / 128.0)-2);

  uint32_t* px = (uint32_t*)screen->pixels;
  int pnum = 0;
  for(int x = 0; x < 512; x++){
    for(int y = 0; y < 512; y++){

      Cpx val = cpx(0, 0);
      for(int it = 0; it < n; it++){
        val = add(mul(val, val), fns[x][y]);
        if(difMag(val, 2) < 0){
          px[pnum] = getColor(it * scl);
          break;
        }
      }
      pnum++;
    }
  }

  SDL_Flip(screen);
  SDL_Delay(5000);
  SDL_Quit();
  return 0;
}
