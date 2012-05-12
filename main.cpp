#include <gtkmm/main.h>
#include "Bombs.h"

int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);
	
	Bombs bombs(10, 10); //hlavni okno
	Gtk::Main::run(bombs);	// run hlavni okno
	
	return 0;
}
