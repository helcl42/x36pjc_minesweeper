#include "OneButton.h"
#include "Hiscores.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>

// konstruktor
Game::Game() : lost(false), winner(false), bombs(0), actualbombs(0), gtime(0), firstclick(false)
{
	//nastavi se cesty k obrazkum
	bomb_pixbuf = Gdk::Pixbuf::create_from_file("./data/bombface6.xpm");
	num_pixbuf[0] = Gdk::Pixbuf::create_from_file("./data/1.xpm");
	num_pixbuf[1] = Gdk::Pixbuf::create_from_file("./data/2.xpm");
	num_pixbuf[2] = Gdk::Pixbuf::create_from_file("./data/3.xpm");
	num_pixbuf[3] = Gdk::Pixbuf::create_from_file("./data/4.xpm");
	num_pixbuf[4] = Gdk::Pixbuf::create_from_file("./data/5.xpm");
	num_pixbuf[5] = Gdk::Pixbuf::create_from_file("./data/6.xpm");
	num_pixbuf[6] = Gdk::Pixbuf::create_from_file("./data/7.xpm");
	num_pixbuf[7] = Gdk::Pixbuf::create_from_file("./data/8.xpm");
}

// destruktor
Game::~Game()
{		
	delete bombref;
}

// New game = zakladni nastaveni 
void Game::new_game()
{
	srand(time(0));
	lost = false;		// hra neni prohrana
	winner = false;		// ani vyhrana
	firstclick = false;	// toto se nastavi az po prvnim eventu na tlacitku
	bombs = 0;		
	bombcount = 0;		
	actualbombs = 0;	
	gtime = 0;
	stop_timer();		// pokud by bezel, tak jej zastavi
}

// nastavi bomby do pole OneButtonu
void Game::set_bombs()
{
	// projede vsechny policka a inicializuje mista kde budou bomby, podle random%ratio
	for (int i = 0; i < width * height; i++)
	{
		bombref->get_square(i)->init((rand() % bombref->get_ratio()) + 1, i, this);
				
		if (bombref->get_square(i)->get_bomb())
		{
			bombs++;
			continue;
		}
	}
	
	actualbombs = bombs;				// vzdy aktualni pocet bomb, ktery se bude menit
	bombcount = actualbombs;			// kolik je bomb celkem
	tiles = width * height - actualbombs;		// zde vime kolik je Ne-Bomb
}

// spocita sousedni bomby
int Game::calc_bombs(int n)
{
	int count = 0;
	
	for (int r = n - width; r <= n + width; r += width)
	{	
		//
		if (r < 0 || r >= width * height)
			continue;
		
		for (int c = -1; c <= 1; c++)
		{
			int cur = r + c;	// aktualni pozice v poli
			
			
			//preskakujeme pole, pro nejz hledame bomby kolem...preskoci jej
			if (cur == n || cur < 0 || cur >= width * height || (cur / width != r / width))
				continue;
			
			// pokud je to bomba-zvedni pocet
			if (bombref->get_square(cur)->get_bomb())
				count++;
		}
	}
	
	return count;
}

// nastavi zde promenne velikosti
void Game::set_size(int w, int h)
{
	width = w;
	height = h;
}

// vsehcny odkryta pole zamacke...jsou "nevratne" zamacknuta
void Game::toggle_neighbors(int n)
{
	for (int r = n - width; r <= n + width; r += width)
	{		
		if (r < 0 || r >= width * height)
			continue;
		bombref->get_square(r)->set_sensitive(false);	
		for (int c = -1; c <= 1; c++)
		{
			int cur = r + c;			
			if (cur == n || cur < 0 || cur >= width * height || (cur / width != r / width))
				continue;
			bombref->get_square(cur)->set_sensitive(false);
			if (	!bombref->get_square(cur)->get_flagged() && 
				!bombref->get_square(cur)->get_bomb() &&
				!bombref->get_square(cur)->get_active()	)
			{
				bombref->get_square(cur)->set_active(true);				
			}	
		}
	}
}

// zmackne sousedy na okraji prazdneho pole
void Game::toggle_neighbors_hard(int n)
{
	for (int r = n - width; r <= n + width; r += width)
	{			
		if (r < 0 || r >= width * height)
			continue;
		bombref->get_square(r)->set_sensitive(false);	
		for (int c = -1; c <= 1; c++)
		{
			int cur = r + c;
			
			if (cur == n || cur < 0 || cur >= width * height || (cur / width != r / width))
				continue;
			bombref->get_square(cur)->set_sensitive(false);
			if (!bombref->get_square(cur)->get_flagged() && !bombref->get_square(cur)->get_active())
			{
				bombref->get_square(cur)->set_active(true);						
			}
				
		}
	}
}

// vrati pocet policek, ktere nesousedi s zadnymi bombami a sousedi s polickem n
int Game::get_blank_neighbors(int n)
{
	int count = 0;
	
	for (int r = n - width; r <= n + width; r += width)
	{	
		if (r < 0 || r >= width * height)
			continue;
		bombref->get_square(r)->set_sensitive(false);
		for (int c = -1; c <= 1; c++)
		{
			int cur = r + c;
			
			if (cur == n || cur < 0 || cur >= width * height || (cur / width != r / width))
				continue;
			bombref->get_square(cur)->set_sensitive(false);			
			if (!bombref->get_square(cur)->get_flagged() && !bombref->get_square(cur)->get_bomb())			
				count++;
		}
	}
	
	return count;	
}

//vrati pocet oflagovanyhc poli
int Game::get_flagged_neighbors(int n)
{
	int count = 0;
	
	for (int r = n - width; r <= n + width; r += width)
	{
		if (r < 0 || r >= width * height)
			continue;		
		for (int c = -1; c <= 1; c++)
		{
			int cur = r + c;
			
			if (cur == n || cur < 0 || cur >= width * height || (cur / width != r / width))
				continue;
						
			if (bombref->get_square(cur)->get_flagged())
				count++;
		}
	}
	
	return count;
}


// prohra
void Game::game_lost()
{
	lost = true;
	stop_timer();
	
	bombref->set_face(2);
	

	for (int i = 0; i < width * height; i++)
	{
		if (bombref->get_square(i)->get_flagged())
			bombref->get_square(i)->set_flagged(false);
				
		if (!bombref->get_square(i)->get_active())
			bombref->get_square(i)->set_active(true);
		
		// zsediveni tlacitek
		if (!bombref->get_square(i)->get_bomb())
			bombref->get_square(i)->set_sensitive(false);
	}

//hi->check_high_scores(bombref,get_time(), bombref->get_skill());
}

// nastavi pointer na Bombs
void Game::set_bombref(Bombs *ref)
{
	bombref = ref;
}

// zda hrac prohral?
bool Game::get_lost()
{
	return lost;
}

// vrati jestli hrac vyhral
bool Game::get_winner()
{
	return winner;
}

// zmensi pocet pouzitelnych policek v hracim poli
void Game::dec_tiles(int n)
{
	tiles--;
	
	if (tiles == 0 && actualbombs == 0)
	{
		gettimeofday(&end_time, NULL);
		long elapsed_seconds  = end_time.tv_sec  - start_time.tv_sec;
		long elapsed_useconds = end_time.tv_usec - start_time.tv_usec;
		int elapsed_mtime = (int)((elapsed_seconds) * 1000 + elapsed_useconds/1000.0) + 0.5;
		
		winner = true;
		stop_timer();
		
		bombref->set_face(1);
				
		bombref->get_square(n)->set_active(false);
		bombref->get_square(n)->set_active(true);
		
		hi->check_high_scores(bombref,elapsed_mtime, bombref->get_skill());
	}
}

// pocet flagu ktere zbyvaji
int Game::get_bombs()
{
	return bombs;
}

int Game::get_count_bombs()
{
	return bombcount;
}

// pokud odflagujeme pole, tak se musi zvetsit pocet neodhalenych bomb
void Game::inc_bombs(int n)
{
	bombs++;
	
	if (bombref->get_square(n)->get_bomb())
		actualbombs++;
	
	Glib::ustring str = Glib::ustring::compose("Bombs: %1", bombs);
	bombref->update_toolbar();
}

// pokud oflagujeme nejake pole, snizi se pocet "nenalezenych" mozna i vyhra,
// pokud oznacime vlakou vsechny hledane spravne
void Game::dec_bombs(int n)
{
	bombs--;
	
	if (bombref->get_square(n)->get_bomb())
		actualbombs--;
	
	bombref->update_toolbar();
	
	// koukne se jestli nahodou uz nejsou vsechny miny odhalene
	if (actualbombs == 0 && tiles == 0)
	{
		gettimeofday(&end_time, NULL);
		long elapsed_seconds  = end_time.tv_sec  - start_time.tv_sec;
		long elapsed_useconds = end_time.tv_usec - start_time.tv_usec;
		long elapsed_mtime = ((elapsed_seconds) * 1000 + elapsed_useconds/1000.0) + 0.5;
		
		winner = true;
		stop_timer();
		
		bombref->set_face(1);
		
		// kontrola jestli nahodou dosazeno nejkratsiho casu na urcite odtiznosti
		hi->check_high_scores(bombref, elapsed_mtime, bombcount);
	}
}

// vrati cas v sekundach
int Game::get_time()
{
	return gtime;
}

// zda je prvni ve hre
bool Game::get_firstclick()
{
	return firstclick;
}

// pokud klikneme poprve ve hre
void Game::set_firstclick()
{
	gettimeofday(&start_time, NULL);
	
	firstclick = true;
	start_timer();
}

// zacne pocitat cas
void Game::start_timer()
{
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this, &Game::on_timeout), 0);
	conn = Glib::signal_timeout().connect(my_slot, 1000);
}

void Game::stop_timer()
{
	conn.disconnect();
}

bool Game::on_timeout(int n)
{	
	gtime++;
	bombref->update_toolbar();
	
	return true;
}

void Game::set_face(int num)
{
	bombref->set_face(num);
}
