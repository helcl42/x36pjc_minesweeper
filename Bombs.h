#ifndef BOMBS_H_
#define BOMBS_H_

#include <gtkmm.h>

class Game;
class OneButton;
class Hiscores;


class Bombs : public Gtk::Window
{
public:	
	Bombs(int width, int height);
	virtual ~Bombs();

	void draw_grid(int width, int height);
	void update_toolbar();
	void set_face(int num);
	OneButton* get_square(int i);
	int get_ratio();
	int get_skill();

protected:

	Gtk::VBox vb, vb1;
	Gtk::HBox hb1;		
	Gtk::Button newbut;	//velke tlacitko
	Gtk::Alignment align;
		
	void build_main_menu();
	void build_toolbar();

	OneButton *squares;			// pole OneButtonu
	Gtk::HBox *hb;				// pole Hboxu
	bool firstload;				// bool signalizujici, zda jed o prvni hru
	int gridwidth, gridheight;		// sirka, vyska
	int gridratio;				// pro random	
	Game *eng;				// Game object	
	Hiscores *hi;				// Hiscores object
	int skill;				
	Gtk::Image toolface[4];			//pole 4 obrazku...podle cesty
  	Gtk::Label bombs_label, time_label;	//labely pro pocte bomb a uplynuly cas

	

	Glib::RefPtr<Gtk::UIManager> m_refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	
	//handlery signalu
	virtual void on_menu_file_new_easy();
	virtual void on_menu_file_new_medium();
	virtual void on_menu_file_new_hard();	
	virtual void on_menu_file_hiscores();
	virtual void on_menu_file_quit();
	virtual void on_menu_help_about();
	virtual void on_button_clicked();	
};

#endif /*BOMBZ_H_*/
