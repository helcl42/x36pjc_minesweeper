#ifndef ONEBUTTON_H_
#define ONEBUTTON_H_
#include "Game.h"
#include <gtkmm.h>

class Game;

class OneButton : public Gtk::ToggleButton
{

public:	
	OneButton();
	virtual ~OneButton();
	
	void init(int r, int n, Game *ref);
	bool get_flagged();
	void set_flagged(bool b);
	bool get_selected();
	bool get_bomb();
	void set_bomb(bool b);
	void set_bombs(int n);
	int get_bombs();
	
protected:
	
	virtual void on_button_press(GdkEventButton* event);
	virtual void on_button_pressed();
	virtual void on_button_released();
	virtual void on_toggled();

	bool flagged;
	bool selected;
	bool was_bomb;
	bool bomb;
	bool bombset;	
	int bombs;
	Game *engref;
	int num;
};

#endif /*ONEBUTTON_H_*/
