#ifndef GAME_H_
#define GAME_H_

#include <gtkmm.h>

class OneButton;
class Bombs;
class Hiscores;

class Game
{
public:

	Game();
	virtual ~Game();
	
	void new_game();
	void set_bombs();
	void set_size(int w, int h);
	void toggle_neighbors(int n);
	void toggle_neighbors_hard(int n);
	int get_blank_neighbors(int n);
	int get_flagged_neighbors(int n);
	void game_lost();
	void set_bombref(Bombs *ref);
	bool get_lost();
	bool get_winner();
	void dec_tiles(int n);
	int get_bombs();
	int get_count_bombs();
	void inc_bombs(int n);
	void dec_bombs(int n);	
	int get_time();
	bool get_firstclick();
	void set_firstclick();
	void set_face(int num);
	int calc_bombs(int n);

	Glib::RefPtr<Gdk::Pixbuf> bomb_pixbuf;
	Glib::RefPtr<Gdk::Pixbuf> num_pixbuf[8];

protected:
	
	void start_timer();
	void stop_timer();
	bool on_timeout(int n);

	int width, height;
	bool lost, winner;
	int bombs, actualbombs, tiles;
	int bombcount;
	int gtime;
	double acc_time;
	struct timeval start_time, end_time;
	bool firstclick;
	sigc::connection conn;		
	Bombs *bombref;
	Hiscores *hi;
};

#endif /*GAME_H_*/
