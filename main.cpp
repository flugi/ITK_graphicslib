#include "graphics.hpp"
#include <iostream>
#include <sstream>

using namespace genv;
using namespace std;

int main()
{
  gout.open(640, 480);


  gout << move_to(10,10) << color(255,255,255) << box(600,400);
  event ev;
  gin.timer(1000);

  if (!gout.load_font("MagyarLinBiolinumB.ttf", 16)) {
    cout << "Font megnyitasa sikertelen! Ellenorizd az eleresi utat!\n";
    return 1;
  }
  string szoveg = "Árvíztűrő tükörfúrógép";
  int w = gout.twidth(szoveg);
  stringstream out;
  int fonth = gout.cascent() + gout.cdescent();
  int lineh = fonth + 5;
  out << "Szöveg hossza: " << w << "; font ascent: " << gout.cascent() << "; font descent: " << gout.cdescent();

  gout << move_to(20,20) << color(0,0,0) << text("Kiírandó szöveg: " + szoveg);
  gout << move_to(20,20 + lineh) << color(0,0,0) << text(out.str());
  gout << move_to(20,20 + lineh*2) << text("Szöveg, alatta (ascent+descent) vonal:");
  gout << move_to(40,20 + lineh*3) << color(10,150,10) << text(szoveg);
  gout << move_to(40,20 + lineh*3 + fonth) << color(255,0,0) << line(w,0);
  gout << move_to(20,20 + lineh*4) << color(0,0,0) << text("Szöveg, vonal a baseline-on:");
  gout << move_to(40,20 + lineh*5) << color(10,150,10) << text(szoveg);
  gout << move_to(40,20 + lineh*5 + gout.cascent()) << color(255,0,0) <<  line(w,0);
  gout.load_font("LiberationMono-Bold.ttf", 26);
  gout << move_to(21,22 + lineh*6) << color(170,170,170) << text("Másik font");
  gout << move_to(20,20 + lineh*6) << color(0,0,0) << text("Másik font");
    gin.timer(500);
    int c=0;
  while (gin >> ev)
  {

    if (ev.keycode == key_escape)
      break;

    if (ev.type == ev_timer) {
        c=255-c;
        gout.set_antialias(true);
        gout << move_to(10,200) << color (c,c,c) << text ("fedés teszt antialias");
        gout.set_antialias(false);
        gout << move_to(10,240) << color (c,c,c) << text ("fedés teszt solid");
    }
    gout << refresh;
  }
  return 0;
}
