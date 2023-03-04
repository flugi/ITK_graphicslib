#include "graphics.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <algorithm>
#include <map>
#include <iostream>


genv::groutput& genv::gout = genv::groutput::instance();
genv::grinput& genv::gin = genv::grinput::instance();

namespace
{
#include "char2.h"
#include "keytbl.h"


    SDL_TimerID timer_id = 0;
    int timer_wait = 0;

    inline Uint32& pixel(SDL_Surface* screen, int x, int y)
    {
        return *((Uint32*)screen->pixels + y * screen->w + x);
    }

    inline void project(SDL_Surface* screen, int draw_clr, int x, int y, int val, int max)
    {
        Uint32& pix = pixel(screen, x, y);
        static const int c[] = {0, 8, 16};
        for (int i=0; i<3; ++i)
        {
            int col = ( ((pix >> c[i])      & 0xff) * (max-val) +
                        ((draw_clr >> c[i]) & 0xff) * val) / max;
            pix = (pix & (~(0xff << c[i]))) | (col << c[i]);
        }
    }

    int findkey(pairptr begin, pairptr end, int key)
    {
        while (begin < end)
        {
            pairptr med = begin + (end-begin)/2;
            int mk = (*med)[0];
            if (mk == key)
                return (*med)[1];
            else if (mk < key)
                begin = med+1;
            else
                end = med;
        }
        return 0;
    }

    int mkkeycode(SDL_Keycode sym, Uint16 unicode)
    {
        int mysym = findkey(keysym_tbl, keysym_end, sym);
        if (mysym != 0)
            return mysym;
        else if (unicode != 0)
        {
            int mycode = findkey(charcode_tbl, charcode_end, unicode);
            return mycode == 0 ? unicode : mycode;
        }
        else
            return sym;
    }
    const std::map<std::string, int> keycodes({
        {"Up",genv::keycode_t::key_up},
        {"Down",genv::keycode_t::key_down},
        {"Left",genv::keycode_t::key_left},
        {"Right",genv::keycode_t::key_right},
        {"Escape",genv::keycode_t::key_escape},
        {"Tab",genv::keycode_t::key_tab},
        {"Backspace",genv::keycode_t::key_backspace},
        {"Return",genv::keycode_t::key_enter},
        {"Insert",genv::keycode_t::key_insert},
        {"Delete",genv::keycode_t::key_delete},
        {"Home",genv::keycode_t::key_home},
        {"End",genv::keycode_t::key_end},
        {"PageUp",genv::keycode_t::key_pgup},
        {"PageDown",genv::keycode_t::key_pgdn},
        {"Left Ctrl",genv::keycode_t::key_lctrl},
        {"Left Shift",genv::keycode_t::key_lshift},
        {"Left Alt",genv::keycode_t::key_lalt},
        {"Right Ctrl",genv::keycode_t::key_rctrl},
        {"Right Shift",genv::keycode_t::key_rshift},
        {"Right Alt",genv::keycode_t::key_ralt},
        {"Left Windows",genv::keycode_t::key_lwin},
        {"Right Windows",genv::keycode_t::key_rwin},
        {"Menu",genv::keycode_t::key_menu},
        {"Numlock",genv::keycode_t::key_numl},
        {"CapsLock",genv::keycode_t::key_capsl},
        {"ScrollLock",genv::keycode_t::key_scrl},
        {"F1",genv::keycode_t::key_f1},
        {"F2",genv::keycode_t::key_f2},
        {"F3",genv::keycode_t::key_f3},
        {"F4",genv::keycode_t::key_f4},
        {"F5",genv::keycode_t::key_f5},
        {"F6",genv::keycode_t::key_f6},
        {"F7",genv::keycode_t::key_f7},
        {"F8",genv::keycode_t::key_f8},
        {"F9",genv::keycode_t::key_f9},
        {"F10",genv::keycode_t::key_f10},
        {"F11",genv::keycode_t::key_f11},
        {"F12",genv::keycode_t::key_f12},
        {"F13",genv::keycode_t::key_f13},
        {"F14",genv::keycode_t::key_f14},
        {"F15",genv::keycode_t::key_f15}
        });

    Uint32 timer_event(Uint32 interval, void*)
    {
        SDL_Event ev;
        ev.type = SDL_USEREVENT;
        ev.user.code = SDL_GetTicks();
        ev.user.data1 = 0;
        ev.user.data2 = 0;
        SDL_PushEvent(&ev);

        return interval;
    }
}

genv::canvas::canvas() {
    buf=0;
    font=0;
    transp=0;
    set_color(255,255,255);
}

genv::canvas& genv::canvas::operator=(const genv::canvas& c) {
    pt_x=c.pt_x;
    pt_y=c.pt_y;
    draw_clr = c.draw_clr;
    transp = c.transp;
    antialiastext = c.antialiastext;
	buf=0;

    if (c.buf) {
//        buf = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY, c.buf->w, c.buf->h, 32,0,0,0,0); //SDL1.2
		buf = SDL_CreateRGBSurface(0, c.buf->w, c.buf->h, 32, 0,0,0,0);
        SDL_Rect trg;
        trg.x = 0;
        trg.y = 0;
        SDL_BlitSurface( c.buf, NULL, buf, &trg);
    }

    font=0;

    if (c.font) {
        load_font(c.loaded_font_file_name, c.font_size);
    }
	return *this;

}

genv::canvas::canvas(const genv::canvas & c) {
    //az esetek nagy részében nem jó ötlet másoló konstruktorban értékadást használni, mert érdemes kihasználni, hogy a : operátorral örökíthetőek a mező konstruktorok. Ez a kód refaktorálásra szorulhat a jövőben, ha sok mező konstruktor-lefutása megspórolható lehet, jelenleg nincs ilyen mező, ezért használhatunk értékadást érdemi lassulás nélkül
	*this = c;
}

genv::canvas::canvas(int w, int h) {
    buf=0;
    font=0;
    loaded_font_file_name="";
    transp=0;
    set_color(255,255,255);
    open(w,h);
    memorybuf=0;
}


genv::groutput::groutput()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
        exit(1);
//    SDL_EnableUNICODE(1); //SDL1.2 code
    buf = 0;
    if (TTF_Init() < 0)
      exit(1);

}

genv::groutput::~groutput()
{
    if (buf) SDL_FreeSurface(buf);
    if (font) TTF_CloseFont(font);
    buf=0;
    font=0;
    antialiastext=0;
    SDL_Quit();
    TTF_Quit();
}


genv::canvas::~canvas() {
    if (buf) SDL_FreeSurface(buf);
    if (font) TTF_CloseFont(font);
}

bool genv::canvas::open(unsigned width, unsigned height)
{
    if (buf) SDL_FreeSurface(buf);
//    buf = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY, width, height, 32,0,0,0,0); //SDL1.2
    buf = SDL_CreateRGBSurface(0, width, height, 32,0,0,0,0);
    pt_x = width/2;
    pt_y = height/2;
    if (buf) {
        memorybuf = (int *)buf->pixels;
        memorypitch = buf->pitch/4;
    }
    return buf != 0;
}

bool genv::groutput::open(unsigned width, unsigned height, bool fullscreen)
{
	buf = SDL_CreateRGBSurface(0, width, height, 32,0,0,0,0);
    if (fullscreen) {
//        buf = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN); //SDL1.2
		window = SDL_CreateWindow("Graphics",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,SDL_WINDOW_FULLSCREEN);

    } else {
 //       buf = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);//SDL1.2
		window = SDL_CreateWindow("Graphics",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,0);

    }
    renderer = SDL_CreateRenderer(window,-1,0);
    pt_x = width/2;
    pt_y = height/2;
        if (buf) {
        memorybuf = (int *)buf->pixels;
        memorypitch = buf->pitch/4;
    }

    return buf != 0;
}


void genv::groutput::showmouse(bool toggle) {
    if (toggle)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}
void genv::groutput::movemouse(int x, int y) {
   // SDL_WarpMouse(x, y);//SDL1.2
	SDL_WarpMouseInWindow(window,x,y);

}

bool genv::canvas::save(const std::string& file) const
{
    return SDL_SaveBMP(buf, file.c_str()) == 0;
}

void genv::groutput::set_title(const std::string& title) {
//	SDL_WM_SetCaption(title.c_str(),0);//SDL1.2
	SDL_SetWindowTitle(window, title.c_str());

}

void genv::groutput::message(std::string errortext) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message",errortext.c_str(),NULL);
}

void genv::canvas::set_color(int r, int g, int b)
{
    draw_clr = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

bool genv::canvas::move_point(int x, int y)
{
    int nx = pt_x + x;
    int ny = pt_y + y;

    if (nx < 0 || ny < 0 || nx >= buf->w || ny >= buf->h)
        return false;

    pt_x = nx;
    pt_y = ny;
    return true;
}

void genv::canvas::draw_dot()
{
    pixel(buf, pt_x, pt_y) = draw_clr;
}

void genv::canvas::line(int x1, int y1, int x2, int y2,unsigned char r, unsigned char g, unsigned char b)
{
    int x=x1;
    int y=y1;
    if (x1<0 || y1<0 || x1>=buf->w || y1>=buf->h) {
        if (x2<0 || y2<0 || x2>=buf->w || y2>=buf->h) {
            return;
            
        } else {
            x=x2;
            y=y2;
            x2=x1;
            y2=y1;
            x1=x;
            y1=y;
        }
    }
    int color=b+256*g+65536*r;
    int xdir = (x2-x1 > 0 ? 1 : -1);
    int ydir = (y2-y1 > 0 ? 1 : -1);

    int xstep = 0, ystep = 0;
    int xshift = 0, yshift = 0;
    int steps, shifts;

    if (abs(x2-x1) >= abs(y2-y1))
    {
        xstep = xdir;
        yshift = ydir;
        steps = abs(x2-x1);
        shifts = abs(y2-y1);
    }
    else
    {
        ystep = ydir;
        xshift = xdir;
        steps = abs(y2-y1);
        shifts = abs(x2-x1);
    }

    int len=0;
    bool noclip = true;
    for (int i=0; noclip && i<steps; ++i)
    {
        unsigned int * pix = (((unsigned int *)buf->pixels)+(y*buf->pitch/4+x));
        *pix = color;
        if ((len+=shifts) >= steps)
        {
            x+=xstep + xshift;
            y+=ystep + yshift;
            len -= steps;
        }
        else{
            x+=xstep;
            y+=ystep;
        }
        noclip &= (x>=0&&y>=0&&x<buf->w&&y<buf->h);

    }
}
void genv::canvas::draw_line(int x, int y)
{
    int xdir = (x > 0 ? 1 : -1);
    int ydir = (y > 0 ? 1 : -1);

    int xstep = 0, ystep = 0;
    int xshift = 0, yshift = 0;
    int steps, shifts;

    if (abs(x) >= abs(y))
    {
        xstep = xdir;
        yshift = ydir;
        steps = abs(x);
        shifts = abs(y);
    }
    else
    {
        ystep = ydir;
        xshift = xdir;
        steps = abs(y);
        shifts = abs(x);
    }

    draw_dot();

    int len=0;
    bool noclip = true;
    for (int i=0; noclip && i<steps; ++i)
    {
        if ((len+=shifts) >= steps)
        {
            noclip = move_point(xstep + xshift, ystep + yshift);
            len -= steps;
        }
        else
            noclip = move_point(xstep, ystep);

        if (noclip)
                draw_dot();
    }
}

void genv::canvas::draw_box(int x, int y)
{
    SDL_Rect r = {pt_x, pt_y, 0, 0};
    if (!move_point(x, y))
        return;

    if (x > 0)
        r.w = x+1;
    else
    {
        r.x += x;
        r.w = -x+1;
    }
    if (y > 0)
        r.h = y+1;
    else
    {
        r.y += y;
        r.h = -y+1;
    }

    SDL_FillRect(buf, &r, draw_clr);
}

void genv::canvas::draw_text(const std::string& str)
{
	if (str=="") return;
    if (font == 0) {
        int left = pt_x;
        if (pt_y - cascent() < 0 || pt_y + cdescent() >= buf->h)
            return;
        for (unsigned i=0; i<str.length(); ++i)
        {
            if (str[i] == '\n')
            {
                if (!move_point(left - pt_x, charheight))
                    return;
                continue;
            }
            unsigned char code = str[i];
            for (int col = 0; col < charwidth; ++col)
            {
                pt_y -= cascent();
                for (int row = 0; row < charheight; ++row)
                {
                    unsigned px = (charfaces[code][row] >> 4*col) & 0xF;

                    project(buf, draw_clr, pt_x, pt_y, px, 15);
                    ++pt_y;
                }
                pt_y -= cdescent();

                if (pt_x < buf->w-1)
                    ++pt_x;
                else
                    return;
            }
        }
    }
    else {

		// SDL_ttf
        // get color from draw_clr:
        unsigned char rc = (draw_clr & 0xff0000) >> 16,
            gc = (draw_clr & 0x00ff00) >>  8,
            bc = (draw_clr & 0x0000ff);
        SDL_Color text_clr = {rc, gc, bc};
        SDL_Surface* t;
        // render text in blended mode (AA)
        if (antialiastext) {
            t = TTF_RenderUTF8_Blended(font, str.c_str(), text_clr);
        } else {
            t = TTF_RenderUTF8_Solid(font, str.c_str(), text_clr);
        }
        if (!t) return;
        SDL_Rect offset;
        offset.x = pt_x;
        offset.y = pt_y;
        pt_x += t->w;
        SDL_BlitSurface( t, NULL, buf, &offset);
        //std::cout << "DIMENSIONS: " << t->w << "," << t->h << std::endl;
        SDL_FreeSurface(t);
    }
}

void genv::canvas::blitfrom(const genv::canvas &c, short x1, short y1, int x2, int y2, short x3, short y3) {
    if (x1==-1) x1=0;
    if (y1==-1) y1=0;
    if (x2==-1) x2=c.buf->w;
    if (y2==-1) y2=c.buf->h;
    SDL_Rect sr={x1,y1,x2,y2};
    SDL_Rect tr={x3,y3,x2,y2};
    if (c.transp) {
        SDL_SetColorKey(c.buf, SDL_TRUE|SDL_RLEACCEL,SDL_MapRGB(c.buf->format, 0, 0 ,0));
    }
    SDL_BlitSurface(c.buf, &sr, buf, &tr);
}

bool genv::canvas::load_font(const std::string& fname, int fontsize, bool antialias)
{
  if (fontsize < 0)
    fontsize = 16;
  // loading font
  if(font) {
  	TTF_CloseFont(font);
  	font=0;
  }
  font = TTF_OpenFont( fname.c_str(), fontsize );
  if (font == 0) // loading error
    return false;
  loaded_font_file_name=fname;
  font_size=fontsize;
  antialiastext=antialias;

  return true;
}

void genv::groutput::refresh()
{
    //SDL_Flip(buf);//SDL1.2
	SDL_Texture * sftx = SDL_CreateTextureFromSurface(renderer,buf);
	SDL_RenderCopy(renderer,sftx,0,0);
	SDL_DestroyTexture(sftx);
	SDL_RenderPresent(renderer);

}

genv::groutput& genv::groutput::instance()
{
    static groutput single_inst;
    return single_inst;
}

void genv::grinput::timer(int wait)
{
    if (timer_id != 0)
    {
        SDL_RemoveTimer(timer_id);
        timer_id = 0;
    }

    if (wait > 0)
    {
        timer_wait = wait;
        timer_id = SDL_AddTimer(wait, timer_event, 0);
    }
}

int utf8charcount(std::string str) {
    char * s = &(str[0]);
    int len = 0;
    while (*s) len += (*s++ & 0xc0) != 0x80;
    return len;
}

std::vector<int> genv::utf8_character_index(std::string str) {
	std::vector<int> res;
    for (size_t i=0;i<str.length();i++) { 
		res.push_back(i);
		while ( (str[i+1] & 0xc0) == 0x80) { //c++11: all strings are null terminated
			i++;
		}
	}
	res.push_back(str.length());
	return res;
}

std::vector<std::string> genv::utf8_character_split(std::string str) {
	std::vector<std::string> res;
    for (size_t i=0;i<str.length();i++) { 
		int len=0;
		do{
			len++;
		} while( (str[i+len] & 0xc0) == 0x80) ;  //c++11: all strings are null terminated
		res.push_back(str.substr(i,len));
		i+=len-1;
		
	}
	
	return res;
	
}

genv::grinput& genv::grinput::wait_event(event& ev)
{
    static event nullev = { 0, 0, 0, 0, 0 };
    static int lastx, lasty;
    ev = nullev;
    if (quit)
        return *this;

    bool got = false;
    do
    {
        SDL_Event se;
        if (!SDL_WaitEvent(&se))
        {
            quit = true;
            return *this;
        }

        switch (se.type)
        {
            case SDL_QUIT:
                quit = true;
                got = true;
                break;
            case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                {
                    std::string c = se.text.text;
					//std::cout << c << std::endl;
                    ev.keycode = c[0];
					ev.keyutf8 = c;
                }
                got = true;
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                ev.type = ev_key;
				ev.keyutf8="";
                ev.keycode = mkkeycode(se.key.keysym.sym, se.key.keysym.sym);
				ev.keyname = SDL_GetKeyName(SDL_GetKeyFromScancode(se.key.keysym.scancode));
				if (keycodes.find(ev.keyname)!=keycodes.end()) {
                    ev.keycode = keycodes.at(ev.keyname);
                    //std::cout <<"* " ;
                  got=true;
  				} else {
				    ev.keycode = mkkeycode(se.key.keysym.sym, se.key.keysym.sym);
				}
                ev.keycode *= (se.type == SDL_KEYUP ? -1 : 1);
                    if (ev.keycode < 0) got = true; // hack: positive values are catched as textinput
                //std::cout << ev.keycode << std::endl;
                //std::cout << ev.keyname.length() << " " << charcount(ev.keyname) << std::endl;
                if (!got)got = utf8charcount(ev.keyname)>1; // HACK: single character named keys should be textinput, but maybe not always..
                if (ev.keycode == key_space) ev.keyutf8=" "; //convenience function. Enter and Tab does not render well with current setup, so only space is supported here.
				break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                ev.type = ev_mouse;
                ev.button = se.button.button;
                ev.button *= (se.button.state == SDL_RELEASED ? -1 : 1);
                ev.pos_x = se.button.x;
                ev.pos_y = se.button.y;
                lastx = se.button.x;
                lasty = se.button.y;
                got = true;
                break;
            case SDL_MOUSEMOTION:
                ev.type = ev_mouse;
                ev.pos_x = se.motion.x;
                ev.pos_y = se.motion.y;
                lastx=se.motion.x;
                lasty=se.motion.y;
                got = true;
                break;
            case SDL_MOUSEWHEEL:
                ev.type = ev_mouse;
                ev.button = se.wheel.y>0?btn_wheelup:btn_wheeldown;
                ev.pos_x = lastx;
                ev.pos_y = lasty;
                got = true;
                break;
            case SDL_USEREVENT:
                ev.type = ev_timer;
                ev.time = se.user.code;
                got = true;
                break;
            default: ;
        }
    } while (!got);

    return *this;
}

genv::grinput& genv::grinput::instance()
{
    static grinput single_inst;
    return single_inst;
}

void genv::grinput::textmode(bool on) {
    if (on) SDL_StartTextInput();
    else SDL_StopTextInput();
}


int genv::canvas::cascent() const
{
    if (font == 0)
        return charheight - chardescent;
    // SDL_ttf ascent
    return TTF_FontAscent(font);
}

int genv::canvas::cdescent() const
{
    if (font == 0)
        return chardescent;
    // SDL_ttf descent
    return -TTF_FontDescent(font);
}

int genv::canvas::twidth(const std::string& s) const
{
    if (font == 0) {
        std::string::const_iterator prev = s.begin(), next;
        next = std::find(prev, s.end(), '\n');
        int max = next - prev;

        while (next != s.end())
        {
            prev = next+1;
        next = std::find(prev, s.end(), '\n');
            max = std::max(max, static_cast<int>(next-prev));
        }

        return max * charwidth;
    }
    // SDL_ttf width:
    int w,h;
    TTF_SizeUTF8(font, s.c_str(), &w, &h);
    return w;

}

