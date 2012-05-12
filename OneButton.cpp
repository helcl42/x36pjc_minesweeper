#include "OneButton.h"
#include <iostream>

#define SIZE 30

//konstruktor
OneButton::OneButton() : flagged(false), selected(false), bomb(false), bombset(false), bombs(0)
{
	// Connect signal_clicked to on_button_clicked
	signal_button_press_event().connect_notify( sigc::mem_fun(*this, &OneButton::on_button_press) );
	signal_pressed().connect( sigc::mem_fun(*this, &OneButton::on_button_pressed) );
	signal_released().connect( sigc::mem_fun(*this, &OneButton::on_button_released) );
	
}

// destructor
OneButton::~OneButton()
{
}


void OneButton::init(int r, int n, Game *ref)
{	
	engref = ref;			// reference na Game
	set_size_request(SIZE, SIZE);	// velikost pres define SIZE
	num = n;			// cislo jako nazev buttonu
	
	// cerveny podklad u vybuchle bomby zustaval, tak je potreba jej dat pryc
	if (was_bomb)
	{
		unset_bg(Gtk::STATE_NORMAL);
		unset_bg(Gtk::STATE_ACTIVE);
		unset_bg(Gtk::STATE_PRELIGHT);
		unset_bg(Gtk::STATE_SELECTED);
		unset_bg(Gtk::STATE_INSENSITIVE);
	}
	
	was_bomb = false;
	flagged = false;
	selected = false;
	bomb = false;
	bombset = false;
	bombs = 0;
	set_active(false);	
	set_sensitive(true);
	
	// smaze jakykoliv obrazek na tlacitku
	remove();	
	//kde r pirjde v parametru jako 1, tam bude mina	
	if (r == 1)
		bomb = true;
}

//pro zmenu obliceje
void OneButton::on_button_pressed()
{
	// kontrola na konec hry
	if (engref->get_lost() || engref->get_winner())
		return;
		
	engref->set_face(3);
}

// zmeni se oblicej po pusteni tlacitka
void OneButton::on_button_released()
{
	// kontrola jestli nahodou nejni konec hry
	if (engref->get_lost() || engref->get_winner())
		return;
		
	engref->set_face(0);
}

// metoda provadena pri kliku na tlacitko
void OneButton::on_button_press(GdkEventButton* event)
{	
	if (!bomb && !bombset)
	{
		set_bombs(engref->calc_bombs(num));
		bombset = true;
	}

	//po kliku na prave tlacitko, policko nebuze byt oflagovano, pokud jsme jej jiz odhalili
	if (event->button == 3 && !get_active() && !engref->get_winner())
	{
		// pokud na policku neni flag tak jej prida
		if (!flagged && engref->get_bombs() > 0)
		{
			add_pixlabel("./data/flag.xpm", "");	// prida obrazek vlajky na aktualne kliknute policko
			engref->dec_bombs(num);	// Decrease the bomb count
			flagged = true;	
		}
		// pokud je pole oznacene jako mina(flagem), tak jej odrastrani
		else if (flagged)
		{
			remove();		// smaze vlajku
			engref->inc_bombs(num);	// musi se zmenit pocet neoznacenych bomb pak i updatovat labely
			flagged = false;	
		}
	}
}

void OneButton::on_toggled()
{	
	if (!bomb && !bombset)
	{
		set_bombs(engref->calc_bombs(num));
		bombset = true;
	}

	if (!engref->get_firstclick())
	{
		engref->set_firstclick();//start citac casu				
	}
	
	if (!selected && !flagged && get_active())
	{
		if (bomb)
		{
			set_image(*manage(new Gtk::Image(engref->bomb_pixbuf)));

			if (!engref->get_lost())
			{
				was_bomb = true;
				
				//bomba na kterou hrac klikl bude na cervenem poli
				Gdk::Color c("red");
				modify_bg(get_state(), c);
				
				// Run the losing method
				engref->game_lost();
			}
		}
		else
		{
			// decrementujeme pocet neodkrytych policek
			if (!engref->get_lost())
				engref->dec_tiles(num);
						
			//obrazek s cislici poctu sousedicich bomb bude pridan jenom tem, ktere sousedi alepon s jednou
			if (bombs > 0) 
			{
				set_image(*manage(new Gtk::Image(engref->num_pixbuf[bombs - 1])));						
			}
			else
			{	
				///metoda na zamcknuti prazdnych tlacitek(ty se kterymi nesousedi zadna mina) se vola pro min jedno takove policko
				if (!engref->get_lost() && engref->get_blank_neighbors(num) > 0){			
					engref->toggle_neighbors(num);				
				}
			}
		}
		
		selected = true;
	}
	// pokud budeme klikat na odhalene tlacitko, bude nezmenitelne(aktivovane)
	else if (selected && !get_active())
	{
		set_active(true);
	}
	// pokud se budeme snazit odkryt pole, ktere jsme jiz oznacili flagem
	else if (flagged && get_active())
	{
		set_active(false);
	}
}

// zda je je oznacene flagem
bool OneButton::get_flagged()
{
	return flagged;
}

//bool v parametru nastavi, ci odnastavi flag a smaze vlajku
void OneButton::set_flagged(bool b)
{
	if (b == false)
		remove(); 
	
	flagged = b;
}

// rekne o policku, zda jiz bylo vybrano
bool OneButton::get_selected()
{
	return selected;
}

// odpovi zda je na tomto poli bomba, ci ne
bool OneButton::get_bomb()
{
	return bomb;
}

// nastavi status jestli je bombou, ci ne
void OneButton::set_bomb(bool b)
{
	bomb = b;
}

// nastavuje pocet okolnich bomb
void OneButton::set_bombs(int n)
{
	bombs = n;
}

// vrati pocet okolnich bomb
int OneButton::get_bombs()
{
	return bombs;
}
