#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

#include <string>
#include <vector>

struct SDL_Surface;
struct SDL_Window;
struct _TTF_Font;
struct SDL_Renderer;

namespace genv
{

/*********** Graphical output device definition ***********/

class canvas {
public:
    canvas();
    virtual ~canvas();
    canvas(int w, int h);
    canvas(const canvas & c);
	genv::canvas& operator=(const genv::canvas &c);
    bool open(unsigned width, unsigned height);
    bool save(const std::string& file) const;
    void transparent(bool t) {transp=t;}
    void set_color(int r, int g, int b);
    bool move_point(int x, int y);
    void draw_dot();
    void draw_line(int x, int y);
    void draw_box(int x, int y);
    void draw_text(const std::string& str);
    void blitfrom(const canvas &c, short x1, short y1, unsigned short x2, unsigned short y2, short x3, short y3);

    bool load_font(const std::string& fname, int fontsize = 16, bool antialias=true);
    void set_antialias(bool antialias) {antialiastext=antialias;}

    int x() const { return pt_x; }
    int y() const { return pt_y; }

    int cascent() const;
    int cdescent() const;

    int twidth(const std::string& s) const;

    virtual void refresh() {}

    template <typename T>
    inline void call_with_rel(T meth, int vec_x, int vec_y) {
	if (vec_x || vec_y) {
	    int dx=vec_x-sgn(vec_x);
	    int dy=vec_y-sgn(vec_y);
	    (this->*meth)(dx, dy);
	}
    }

protected:

    template <typename T>
    inline int sgn(const T& a) {
		if (a<0) return -1;
		if (a>0) return 1;
		return 0;
    }

    short pt_x;
    short pt_y;
    SDL_Surface* buf;
    int draw_clr;
    bool transp;
    _TTF_Font* font;
    bool antialiastext;
    std::string loaded_font_file_name;
    int font_size;

};




// Class of output device (singleton)
class groutput : public canvas
{
public:
    static groutput& instance();
    virtual ~groutput();

    void showmouse(bool toggle);
    void movemouse(int x, int y);
    bool open(unsigned width, unsigned height, bool fullscreen=false);
    virtual void refresh();
	void set_title(const std::string& title);
	void message(std::string errortext);

private:
	SDL_Window * window;
	SDL_Renderer* renderer;
    groutput();
};

// Global accessor for the output device instance
extern groutput& gout;

// Generic operator for applying global manipulators
template <typename Op>
inline canvas& operator << (canvas& out, Op oper)
{ oper(out); return out; }


// Global manipulators for frequently used operations
inline void refresh(canvas& out) { out.refresh(); }
inline void dot(canvas& out) { out.draw_dot(); }

struct stamp
{
    canvas &c;
    int x1,y1,x2,y2,x3,y3;
    stamp(canvas&cc, int sx1, int sy1, int xsize, int ysize, int tx1, int ty1) :
        c(cc), x1(sx1), y1(sy1),x2(xsize), y2(ysize), x3(tx1), y3(ty1) {}
    stamp(canvas&cc, int tx1, int ty1) :
        c(cc), x1(-1), y1(-1),x2(-1), y2(-1), x3(tx1), y3(ty1) {}
    void operator () (canvas& out)
    { out.blitfrom(c,x1,y1,x2,y2,x3,y3); }
};

struct color
{
    int red, green, blue;
    color(int r, int g, int b) : red(r), green(g), blue(b) {}
    void operator () (canvas& out)
    { out.set_color(red, green, blue); }
};



struct move
{
    int vec_x, vec_y;
    move(int x, int y) : vec_x(x), vec_y(y) {}
    void operator () (canvas& out)
    { out.move_point(vec_x, vec_y); }
};

struct move_to
{
    int pos_x, pos_y;
    move_to(unsigned x, unsigned y) : pos_x(x), pos_y(y) {}
    void operator () (canvas& out)
    { out.move_point(pos_x - out.x(), pos_y - out.y()); }
};

struct line
{
    int vec_x, vec_y;
    line(int x, int y) : vec_x(x), vec_y(y) {}
    void operator () (canvas& out)
    { out.call_with_rel(&canvas::draw_line,vec_x, vec_y); }
};

struct line_to
{
    int pos_x, pos_y;
    line_to(unsigned x, unsigned y) : pos_x(x), pos_y(y) {}
    void operator () (canvas& out)
    { out.draw_line(pos_x - out.x(), pos_y - out.y()); }
};

struct box
{
    int vec_x, vec_y;
    box(int x, int y) : vec_x(x), vec_y(y) {}
    void operator () (canvas& out)
    { out.call_with_rel(&canvas::draw_box, vec_x, vec_y); }
};

struct box_to
{
    int pos_x, pos_y;
    box_to(unsigned x, unsigned y) : pos_x(x), pos_y(y) {}
    void operator () (canvas& out)
    { out.draw_box(pos_x - out.x(), pos_y - out.y()); }
};

struct text
{
    std::string str;
    text(const std::string& s) : str(s) {}
    text(char c) : str(1, c) {}
    void operator () (canvas& out)
    { out.draw_text(str); }
};
/*
struct title
{
    std::string str;
    title(const std::string& s) : str(s) {}
    void operator () (canvas& out)
    { out.set_title(str); }
};
*/

struct font
{
    std::string font_name;
    int font_size;
    bool antialias;
    font(const std::string& s, int fs, bool a=true) : font_name(s), font_size(fs), antialias(a) {}
    void operator () (canvas& out)
    { out.load_font(font_name, font_size, antialias); }
};


/*********** Input device definition **********/

// Keycode constants
enum keycode_t {
    key_tab = '\t', key_backspace = '\b', key_enter = '\r',
    key_escape = '\033', key_space = ' ',
    key_up = 0x10000, key_down, key_right, key_left,
    key_insert, key_delete, key_home, key_end, key_pgup, key_pgdn,
    key_lshift, key_rshift, key_lctrl, key_rctrl, key_lalt, key_ralt,
    key_lwin, key_rwin, key_menu, key_numl, key_capsl, key_scrl,
    key_f0 = 0x20000, key_f1, key_f2, key_f3, key_f4, key_f5, key_f6, key_f7,
    key_f8, key_f9, key_f10, key_f11, key_f12, key_f13, key_f14, key_f15
};

enum button_t {
    btn_left = 1, btn_middle, btn_right, btn_wheelup, btn_wheeldown
};

enum event_type {
    ev_key = 1, ev_mouse, ev_timer
};

// Event descriptor
struct event
{
    int keycode;
    int pos_x, pos_y;
    int button;
    int time;
    int type;
	std::string keyname;
	std::string keyutf8;

};

// Class of input device (singleton)
class grinput
{
public:
    static grinput& instance();

    grinput& wait_event(event&);
    void timer(int wait);

    operator const void*() const
    { if (quit) return 0; else return this; }
    void textmode(bool on);
private:
    grinput() : quit(false) {textmode(true);}
    bool quit;
};

// Global accessor for the input device instance
extern grinput& gin;

// Event reader operator
inline grinput& operator >> (grinput& inp, event& ev)
{ return inp.wait_event(ev); }

std::vector<int> utf8_character_index(std::string str);
std::vector<std::string> utf8_character_split(std::string str) ;

}

#endif // GRAPHICS_HPP_INCLUDED
