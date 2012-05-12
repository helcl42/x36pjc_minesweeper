#include "Hiscores.h"
#include "OneButton.h"
#include <iostream>
#include <string>
#include <sstream>

#define RATIO 7

// konstruktor
Bombs::Bombs(int width, int height):align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 0.0, 0.0)			
{
	// zde jsou nastaveny cesty k obrazkum obliceje
	toolface[0].set("./data/normal.png");
	toolface[1].set("./data/win.png");
	toolface[2].set("./data/dead.png");
	toolface[3].set("./data/clicking.png");
	firstload = true;
	skill = 0;

	set_title("Minesweeper");
	set_border_width(0);
	set_resizable(false);	
	vb1.set_border_width(10);
	eng = new Game();				// Game se nastavi na default
	eng->set_bombref(this);				// Nastavi referenci na Game
	
	gridratio = RATIO;				// tato hodnota je dana definem urcuje "pravdepodobnost"
	gridwidth = width;
	gridheight = height;
	
	add(vb);						// prida hlavni Vertical-Box
	build_main_menu();
	align.add(vb1);	
	vb.pack_end(align);
	build_toolbar();
	draw_grid(gridwidth, gridheight);
	
	firstload = false;
}

// destruktor
Bombs::~Bombs()
{
	delete[] hb;
	delete[] squares;
}

// inicializuje mrizku a vytvori pole objektu OneButton
void Bombs::draw_grid(int width, int height)
{	
	// nastaveni obliceje
	set_face(0);
	
	// pokud opakujeme hru
	if (!firstload && (gridwidth != width || gridheight != height))
	{
		delete[] hb;
		delete[] squares;
	}
	
	//pokud jede hra poprve od spusteni, nebo pokud se zmenila velikost rozmeru
	if (firstload || (gridwidth != width || gridheight != height))
	{
		squares = new OneButton[width * height]; //vytvori pole objektu tridy OneButton
		hb = new Gtk::HBox[height]; ///zde vytvori pole horozontalnich boxu
	}
	
	eng->set_size(width, height); //nastavi promenne pro vysku a sirku ve tride Game
	eng->new_game();//nastavi promenne na default

	eng->set_bombs();

	if (firstload || (gridwidth != width || gridheight != height))
	{
		for (int h = 0; h < height; h++)///bezi podle vysky sloupce
		{
			for (int i = width * h; i < width * h + width; i++)
				hb[h].pack_start(squares[i], Gtk::PACK_SHRINK);///naplni se vzdy jeden Hbox(tvori radku)

			vb1.pack_start(hb[h], Gtk::PACK_SHRINK);///vlozi se do hlavniho Vboxu
		}
	}
	
	gridwidth = width;		// nastavi aktualni rozmery
	gridheight = height;
		
	show_all_children();
		
	update_toolbar();//updatuje se pocet min a cas se nastavi na nulu, pak se ceka na prvni klik na tlacitko
}

//vrati pointer na objekt OneButton
OneButton* Bombs::get_square(int i)
{
	return &squares[i];
}

// vrati "cislo pravdepodobnosti"
int Bombs::get_ratio()
{
	return gridratio;
}

// Vrati level obtiznosti
int Bombs::get_skill()
{
	return skill;
}

// Menu v horni casto okna
void Bombs::build_main_menu()
{
	// vytvori polozky celeho menu a jednotlivym polozkam priradi metodu, ktera se bude volat po jejim stisku
	m_refActionGroup = Gtk::ActionGroup::create();
	
	m_refActionGroup->add(	Gtk::Action::create("FileNewEasy",
							Gtk::Stock::YES, "Easy", "easy game."),
							sigc::mem_fun(*this, &Bombs::on_menu_file_new_easy));

	m_refActionGroup->add(	Gtk::Action::create("FileNewMedium",
							Gtk::Stock::YES, "Medium", "medium game."),
							sigc::mem_fun(*this, &Bombs::on_menu_file_new_medium));

	m_refActionGroup->add(	Gtk::Action::create("FileNewHard",
							Gtk::Stock::NO, "Hard", "hard game."),
							sigc::mem_fun(*this, &Bombs::on_menu_file_new_hard));
	
	m_refActionGroup->add(	Gtk::Action::create("FileScoreboard",
							Gtk::Stock::PASTE, "Hiscores", "View hiscores."),
							sigc::mem_fun(*this, &Bombs::on_menu_file_hiscores));
		
	m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
	m_refActionGroup->add(	Gtk::Action::create("HelpAbout",
							Gtk::Stock::ABOUT, "About", "About."),
							sigc::mem_fun(*this, &Bombs::on_menu_help_about));


	m_refActionGroup->add(Gtk::Action::create("FileMenu", "Game"));

	m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::EXECUTE, "New", "New game."));

	m_refActionGroup->add(	Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &Bombs::on_menu_file_quit));
		
	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);
	
	// klavesove zkratky
	add_accel_group(m_refUIManager->get_accel_group());
	
	// tento uistring rika jak bude menu clenene
	Glib::ustring ui_info = 
		"<ui>"
			"<menubar name='MenuBar'>"
					"<menu action='FileMenu'>"
						"<menu action='FileNew'>"
						"<menuitem action='FileNewEasy'/>"
						"<menuitem action='FileNewMedium'/>"
						"<menuitem action='FileNewHard'/>"
					"<separator/>"		
					"</menu>"
					"<separator/>"
					"<menuitem action='FileScoreboard'/>"
					"<separator/>"
					"<menuitem action='FileQuit'/>"
				"</menu>"
				"<menu action='HelpMenu'>"
        			"<menuitem action='HelpAbout'/>"
        			"</menu>"
			"</menubar>"
        "</ui>";
	
	#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{
		m_refUIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
	}
	#else
	std::auto_ptr<Glib::Error> ex;
	m_refUIManager->add_ui_from_string(ui_info, ex);
	if(ex.get())
	{
		std::cerr << "building menus failed: " <<  ex->what();
	}
	#endif //GLIBMM_EXCEPTIONS_ENABLED

	Gtk::Widget *menu_widget = m_refUIManager->get_widget("/MenuBar");
	if (menu_widget)
		vb.pack_start(*menu_widget, Gtk::PACK_SHRINK);
}

// vytvori toolbar s tlacitkem a dvema labely(pocet bomb a cas)
void Bombs::build_toolbar()
{
	hb1.set_border_width(2);
	hb1.set_homogeneous(true);
	
	newbut.set_size_request(43, 43);
	//priradi signal, po kliknuti vola on_button_clicked
	newbut.signal_clicked().connect(sigc::mem_fun(*this, &Bombs::on_button_clicked));

	update_toolbar(); //nastavi se na aktualni hodnoty
	//vklada odleva do Horizontal-Boxu
	hb1.pack_start(bombs_label);
	hb1.pack_start(newbut);
	hb1.pack_start(time_label);
	
	//vlozi novy HBox do ndrazeneho Vboxu
	vb.pack_start(hb1);
}

// nastavu hodnoty na labelech, uplynuly cas od zacatku hry, celkovy pocet min a pocet zbyvajicich flagu
void Bombs::update_toolbar()
{
	Glib::ustring bombstr;
	int bcount = eng->get_bombs(); //zjisti kolik je jeste neoznacenych bomb(nepouzitych flagu)
	 
	// toto je pro nastaveni formatu cisel
	if (bcount < 10)
	 	bombstr = Glib::ustring::compose("00%1", bcount);
	else if (bcount < 100)
		bombstr = Glib::ustring::compose("0%1", bcount);
	else
		bombstr = Glib::ustring::compose("%1", bcount);

	int time = eng->get_time();
	int min = int(time / 60);
	int sec = time - min * 60;
	Glib::ustring minstr, secstr, timestr;
	
	// minuty
	if (min < 10)
		minstr = Glib::ustring::compose("0%1", min);
	else
		minstr = Glib::ustring::compose("%1", min);
	
	// sekundy
	if (sec < 10)
		secstr = Glib::ustring::compose("0%1", sec);
	else
		secstr = Glib::ustring::compose("%1", sec);

	std::stringstream ss;
        ss << eng->get_count_bombs();	

	timestr = minstr + ":" + secstr;
	bombstr = bombstr + "/" + ss.str();
	
//std::cout << "bomb " << bombstr << " time " << timestr << std::endl;

	bombs_label.set_text(bombstr);
	time_label.set_text(timestr);
}

// klik na polozku z rozeviraci menu zavola jednu z metod
void Bombs::on_menu_file_new_easy()
{
	skill = 0;
	gridratio = 7;
	draw_grid(10, 10);
}

void Bombs::on_menu_file_new_medium()
{
	skill = 1;
	gridratio = 6;
	draw_grid(20, 16);
}

void Bombs::on_menu_file_new_hard()
{
	skill = 2;
	gridratio = 5;
	draw_grid(30, 16);
}
//ukonci hlavni okno
void Bombs::on_menu_file_quit()
{
	hide();
}

// zobrazi hiscores
void Bombs::on_menu_file_hiscores()
{
	hi->show_hiscores(this);
}

void Bombs::on_menu_help_about()
{
	std::cout << "About?...to to je jedina akce po kliku na About" << std::endl;
}

// klik na velke tlacitko s oblicejem, zacne nova hra s predchozimi rozmery
void Bombs::on_button_clicked()
{
	draw_grid(gridwidth, gridheight);
}

// nastavuje obrazek obliceje
void Bombs::set_face(int num)
{
	newbut.remove();	// maze oblicej
	newbut.set_image(toolface[num]); //nastavuje novy
}
