#include "graphics.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <thread>
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

int main()
{
    gout.open(X, Y);
	gout.set_title("asd");
    gout << move_to(10,10) << color(255,255,255) << box(X-20,Y-20);
    event ev;
    gin.timer(1000);
    string szoveg = "Árvíztűrő tükörfúrógép";
    int w = gout.twidth(szoveg);
    stringstream out;
    int fonth = gout.cascent() + gout.cdescent();
    int lineh = fonth + 5;  //egyes sorok között 5 pixelt hagyunk
    out << "Szöveg hossza: " << w << "; font ascent: " << gout.cascent() << "; font descent: " << gout.cdescent();

    gout << move_to(20,20) << color(0,0,0) << text("Kiírandó szöveg: " + szoveg);
    gout << move_to(20,20 + lineh) << color(0,0,0) << text(out.str());
    if (!gout.load_font("LiberationSans-Regular.ttf", 16))
    {
		gout.message("nincs meg a font");
        cout << "Font megnyitasa sikertelen! Ellenorizd az eleresi utat!\n";
        cout << "download: http://users.itk.ppke.hu/~flugi/bevprog2_1112/graph/Fonts.zip";
        return 1;
    }
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
            for (size_t i=0;i<cv.size();i++) {
                gout << stamp(cv[i], rand()%X-40,rand()%Y-40);
            }
            gout << refresh;
        }
    }

    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255) << refresh;
		string str="abc";
    while (gin>>ev && ev.keycode != key_escape) {
        if (ev.type == ev_key) {
			str+=ev.keyutf8;
/*			for (unsigned char c : ev.keyutf8) {
				std::cout <<  (int)(c) << " ";
			}
			*/
//			std::cout << std::endl;
			vector<int> index = utf8_character_index(str);
			vector<string> chars = utf8_character_split(str);
			if (ev.keycode==key_backspace && str!="") {
				str=str.substr(0,index[index.size()-2]);
				index = utf8_character_index(str);
				chars = utf8_character_split(str);
			}
//			std::cout << ev.keycode << endl;
//			std::cout << str << endl;
            gout << stamp(gout, 0,0,X,Y-40, 0, 40);
            gout << color(0,0,0) << move_to(0,0) << box(X,40) << color(255,255,255);
            gout << move_to(30,20) << text(to_str(ev.keycode)) << genv::move_to(150,20) << text(ev.keyname) << genv::move_to(300,20) << text(string("")+char(ev.keycode)) << genv::move_to(350,20) << text(ev.keyutf8); 
            gout << color(0,0,0) << move_to(0,Y-80) << box(X,80) << color(255,255,255);

			gout << move_to(0,Y-70) <<color(255,255,255)<< text(str);
			//for (int a : index) {
			//	std::cout << a << " ";
			//}
			//std::cout<<std::endl;
			gout << move_to(0,Y-50) << color(127,255,255);
			for (size_t i=0;i<index.size()-1;i++) {
				gout << text(str.substr(index[i],index[i+1]-index[i]));
			}
			gout << move_to(0,Y-30) << color(255,127,255);
			for (size_t i=0;i<index.size()-1;i++) {
				gout << text(chars[i]);
			}
            gout << refresh;
        }
    }

    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255) << refresh;
    vector<canvas> cv(10);
    for (size_t i=0;i<cv.size();i++) {
    	cv[i].open(100,100);
    	for (int j=0;j<10;j++) {
    		cv[i] << move_to(i*8,i*8) << line_to(100-j*7, j*7);
    	}
    	cv[i].transparent(true);
    }
    gin.timer(40);
    while (gin>>ev && ev.keycode != key_escape) {
    	if (ev.type == ev_timer) {
	    	gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255);
	    	for (size_t i=0; i<cv.size(); i++) {
	    		gout << stamp(cv[i],X/2+sin(ev.time/1000.0+i)*X/3,Y/2+sin(ev.time/1200.0+i)*Y/3);
	    	}
	    	gout << refresh;
    	}
    }
    gout << move_to(0,0) << color(0,0,0) << box(X,Y) << color(255,255,255) << refresh;
    bool stop=false;
    std::thread thr([&](){
        int j=0;    
        while(!stop) {
            j++;            
            gout.dot(sin(j/10000.0)*sin(j/1000000.0)*X/6+X/2,cos(j/10000.0)*Y/6+Y/2,j,j,j);
        }
    });
    thr.detach();

    std::thread thr2([&](){
        int j=0;    
        while(!stop) {
            j++;            
            gout.dot(sin(j/10000.0)*sin(j/1000000.0)*X/6+X/2+X/6,cos(j/10000.0)*Y/7+Y/2,255-j,255-j,j);
        }
    });
    thr2.detach();
    
    while (gin>>ev && ev.keycode != key_escape) {
        if (ev.type==ev_timer) {
            gout << refresh;
        }
    }
    stop=true;


    return 0;
}
