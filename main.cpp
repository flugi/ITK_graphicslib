#include "graphics.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace genv;
using namespace std;

const int X=640;
const int Y=480;

template <typename T>
string to_str(const T& a) {
    stringstream ss;
    ss << a;
    return ss.str();
}

void circle(int rx, int ry, int r, int _col, int _type){
    int _x,_y;
    if(_col == 1){gout << color(0,0,0);}
    if(_col == 2){gout << color(150,150,150);}
    if(_col == 3){gout << color(50,50,50);}
    if(_col == 4){gout << color(150,150,0);}
    if(_col == 5){gout << color(255,255,0);}
    if(_col == 6){gout << color(150,0,0);}
    if(_col == 7){gout << color(255,0,0);}
    if(_col == 8){gout << color(0,0,150);}
    if(_col == 9){gout << color(0,0,255);}
    if(_col == 10){gout << color(0,150,0);}
    if(_col == 11){gout << color(0,255,0);}

    if(_type == 0){
        for(_x=rx-r; _x<=rx+r; _x++){
            for(_y=ry-r; _y<=ry+r; _y++){
                int dx = abs(rx-_x);
                int dy = abs(ry-_y);
                if (sqrt((dx*dx)+(dy*dy)) < r){
                    gout << move_to(_x,_y) << dot;
                }
            }
        }
    }

    if(_type == 1){
        for(_x=rx; _x<=rx+r; _x++){
            for(_y=ry-r; _y<=ry; _y++){
                int dx = abs(rx-_x);
                int dy = abs(ry-_y);
                if (sqrt((dx*dx)+(dy*dy)) < r){
                    gout << move_to(_x,_y) << dot;
                }
            }
        }
    }

    if(_type == 2){
        for(_x=rx; _x<=rx+r; _x++){
            for(_y=ry; _y<=ry+r; _y++){
                int dx = abs(rx-_x);
                int dy = abs(ry-_y);
                if (sqrt((dx*dx)+(dy*dy)) < r){
                    gout << move_to(_x,_y) << dot;
                }
            }
        }
    }

    if(_type == 3){
        for(_x=rx-r; _x<=rx; _x++){
            for(_y=ry; _y<=ry+r; _y++){
                int dx = abs(rx-_x);
                int dy = abs(ry-_y);
                if (sqrt((dx*dx)+(dy*dy)) < r){
                    gout << move_to(_x,_y) << dot;
                }
            }
        }
    }
    if(_type == 4){
        for(_x=rx-r; _x<=rx; _x++){
            for(_y=ry-r; _y<=ry; _y++){
                int dx = abs(rx-_x);
                int dy = abs(ry-_y);
                if (sqrt((dx*dx)+(dy*dy)) < r){
                    gout << move_to(_x,_y) << dot;
                }
            }
        }
    }
}

int main()
{
    gout.open(X, Y);
    //gout << move_to(10,10) << color(255,255,255) << box(X-20,Y-20);
    event ev;
    gin.timer(30);
    if (!gout.load_font("LiberationSans-Regular.ttf", 16))
    {
        cout << "Font megnyitasa sikertelen! Ellenorizd az eleresi utat!\n";
        cout << "download: http://users.itk.ppke.hu/~flugi/bevprog2_1112/graph/Fonts.zip";
        return 1;
    }
    string szoveg = "Árvíztűrő tükörfúrógép";
    int w = gout.twidth(szoveg);
    stringstream out;
    int fonth = gout.cascent() + gout.cdescent();
    int lineh = fonth + 5;  //egyes sorok között 5 pixelt hagyunk
    out << "Szöveg hossza: " << w << "; font ascent: " << gout.cascent() << "; font descent: " << gout.cdescent();

    gout << move_to(20,20) << color(0,0,0) << text("Kiírandó szöveg: " + szoveg);
    gout << move_to(20,20 + lineh) << color(0,0,0) << text(out.str());
    gout << move_to(20,20 + lineh*2) << text("Szöveg, alatta (ascent+descent) vonal:");
    gout << move_to(40,20 + lineh*3) << color(10,150,10) << text(szoveg);
    gout << move_to(40,20 + lineh*3 + fonth) << color(255,0,0) << line(w,0);
    gout << move_to(20,20 + lineh*4) << color(0,0,0) << text("Szöveg, vonal a baseline-on:");
    gout << move_to(40,20 + lineh*5) << color(10,150,10) << text(szoveg);
    gout << move_to(40,20 + lineh*5 + gout.cascent()) << color(255,0,0) <<  line(w,0);
    gout.load_font("LiberationSans-Regular.ttf", 20);
    fonth = gout.cascent() + gout.cdescent();
    lineh = fonth + 5;
    w = gout.twidth(szoveg);
    gout << move_to(320,20 + lineh*2) << text("Szöveg, méretváltás után");
    gout << move_to(340,20 + lineh*3) << color(10,150,10) << text(szoveg);
    gout << move_to(340,20 + lineh*3 + fonth) << color(255,0,0) << line(w,0);
    gout << move_to(320,20 + lineh*4) << color(0,0,0) << text("Szöveg, vonal a baseline-on:");
    gout << move_to(340,20 + lineh*5) << color(10,150,10) << text(szoveg);
    gout << move_to(340,20 + lineh*5 + gout.cascent()) << color(255,0,0) <<  line(w,0);
//  gout.load_font("LiberationMono-Bold.ttf", 26);
    gin.timer(500);
    int c=0;
    while (gin >> ev && ev.keycode != key_space)
    {
        if (ev.type == ev_timer)
        {
            c=255-c;
            gout.set_antialias(true);
            gout << move_to(10,200) << color (c,c,c) << text ("fedés teszt antialias");
            gout.set_antialias(false);
            gout << move_to(10,240) << color (c,c,c) << text ("fedés teszt solid");
        }
        gout << refresh;
    }



    gout.load_font("LiberationSans-Regular.ttf", 36);
    gout << move_to(0,0) << color(0,0,0) << box(X,Y);
    for (int i=0; i<1000; i++)
    {
        gout << move_to(0,0) << color(rand(), rand(), rand()) << line_to(rand()%X, rand()%Y);
    }
    gout << move_to(X/2-100,Y/2)<<color(255,255,255) << text("stamp test - press space");
    gin.timer(20);
    while (gin >> ev && ev.keycode != key_space)
    {
        if (ev.type == ev_timer)
        {
            for (int i=0;i<10;i++) {
                int px=rand()%X-40;
                int py=rand()%Y-40;
                gout << stamp(gout, px, py, 40,40, px+rand()%3-1, py+rand()%3-1);
            }
            gout << refresh;
        }
    }

    gout.load_font("LiberationSans-Regular.ttf", 24);
    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255);
    gout << move_to(X/2-100,Y/2)<<color(255,255,255) << text("mouse test - press space");
    gout << move_to(X/2-100,Y/2+50)<<color(255,255,255) << text("mouse cursor should blink");
    gout << refresh;
    gin.timer(500);
    bool show = true;
    while (gin >> ev && ev.keycode != key_space)
    {
        if (ev.type == ev_mouse) {
            gout << move_to(ev.pos_x, ev.pos_y) << dot;
            if (ev.button==btn_left) gout <<genv::move(-10,-10) << box(20,20);
            gout << refresh;
        }
        if (ev.type==ev_timer) {
            show=!show;
            gout.showmouse(show);
        }
    }


    gout.load_font("LiberationSans-Regular.ttf", 24);
    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255);
    for (int i=0;i<40;i++) {
        gout << move_to(X/40*i, 0) << line(0,Y);
        gout << move_to(0,Y/40*i) << line(X,0);
    }
    gout << move_to(X/2-100,Y/2)<<color(255,255,255) << text("canvas test - press space");
    gout << refresh;
    gin.timer(40);
    while (gin >> ev && ev.keycode != key_space)
    {
        if (ev.type == ev_timer)
        {
            vector<canvas> cv;
            for (int i=0;i<10;i++) {
                canvas c(40,40);
                int px=rand()%X-40;
                int py=rand()%Y-40;
                c << stamp(gout, px, py, 40,40,0,0);
                cv.push_back(c);
            }
            for (int i=0;i<cv.size();i++) {
                gout << stamp(cv[i], rand()%X-40,rand()%Y-40);
            }
            gout << refresh;
        }
    }

    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255) << refresh;
    while (gin>>ev && ev.keycode != key_space)
    {
        if (ev.type == ev_key) {
            gout << stamp(gout, 0,0,X,Y-40, 0, 40);
            gout << color(0,0,0) << move_to(0,0) << box(X,40) << color(255,255,255);
            gout << move_to(30,20) << text(to_str(ev.keycode));
            gout << refresh;
        }
    }

    while (gin>>ev && ev.keycode != key_escape)
    {

        if(ev.type == controller){
            gout << move_to(0,0) << color(0,0,0) << box(X,Y);
            circle(X/5,1.5*Y/5,45,2,0);
            circle(X/5,1.5*Y/5,40,1,0);
            circle(X-250,Y/2,45,2,0);
            circle(X-250,Y/2,40,1,0);
            circle(X-125,100,20,5,0);
            circle(X-75,150,20,7,0);
            circle(X-175,150,20,9,0);
            circle(X-125,200,20,11,0);
            circle(237,Y/2+2,39,2,0);
            gout << move_to(230,150) << color(200,200,200) << text("BACK");
            gout << move_to(330,150) << color(200,200,200) << text("START");
            if(ev.hat == hat_rup){circle(237,Y/2+2,39,3,1);}
            if(ev.hat == hat_rdown){circle(237,Y/2+2,39,3,2);}
            if(ev.hat == hat_ldown){circle(237,Y/2+2,39,3,3);}
            if(ev.hat == hat_lup){circle(237,Y/2+2,39,3,4);}
            gout << move_to(200,Y/2-10) << color(200,200,200) << box(75,25);
            gout << move_to(225,Y/2-35) << color(200,200,200) << box(25,75);
            gout << move_to(136,420) << color(200,200,200) << box(64,40);
            gout << move_to(440,420) << color(200,200,200) << box(64,40);

            circle(ev.move_lx/13+X/5,ev.move_ly/13+1.5*Y/5,20,2,0); //Bal joystick mozgásának reprezentálása
            circle(X-250+ev.move_rx/13,ev.move_ry/13+Y/2,20,2,0); //Jobb joystick mozgásának reprezentálása

            for(int i = 0; i < 64-ev.trig_l/4; ++i){
                gout << color(200,200,200) << move_to(200,350) << line_to(200-i,400);
            }
            for(int i = 0; i < 64-ev.trig_r/4; ++i){
                gout << color(200,200,200) << move_to(440,350) << line_to(440+i,400);
            }

            if(ev.joy_btn == jbutton_left_click){circle(X/5,1.5*Y/5,20,3,0);}
            if(ev.joy_btn == jbutton_right_click){circle(X-250,Y/2,20,3,0);}
            if(ev.joy_btn == jbutton_y){circle(X-125,100,20,4,0);}
            if(ev.joy_btn == jbutton_b){circle(X-75,150,20,6,0);}
            if(ev.joy_btn == jbutton_x){circle(X-175,150,20,8,0);}
            if(ev.joy_btn == jbutton_a){circle(X-125,200,20,10,0);}
            if(ev.joy_btn == jbutton_back){gout << move_to(230,150) << color(100,100,100) << text("BACK");}
            if(ev.joy_btn == jbutton_start){gout << move_to(330,150) << color(100,100,100) << text("START");}
            if(ev.joy_btn == jbutton_lb){gout << move_to(136,420) << color(100,100,100) << box(64,40);}
            if(ev.joy_btn == jbutton_rb){gout << move_to(440,420) << color(100,100,100) << box(64,40);}
            if(ev.hat == hat_up){gout << move_to(225,Y/2-35) << color(100,100,100) << box(25,25);}
            if(ev.hat == hat_right){gout << move_to(250,Y/2-10) << color(100,100,100) << box(25,25);}
            if(ev.hat == hat_down){gout << move_to(225,Y/2+15) << color(100,100,100) << box(25,25);}
            if(ev.hat == hat_left){gout << move_to(200,Y/2-10) << color(100,100,100) << box(25,25);}

        }
        gout<<refresh;
    }
return 0;
}
