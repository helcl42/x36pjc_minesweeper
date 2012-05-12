#include "Hiscores.h"
#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <sstream>
#include <fstream>

// porovnani casu
bool Hiscores::compare_score(int a, int b)
{
	if(a > b)return true;
	else return false;
}

//vrati in ze stringu
int Hiscores::getintval(std::string strConvert) 
{
int intReturn; 

	intReturn = atoi(strConvert.c_str());

return(intReturn);
}

// Dialog pro vlozeni jmena 
Glib::ustring Hiscores::get_name(Bombs *b)
{
	Gtk::Dialog hiscore("High Score", *b, true, true);
	Gtk::VBox *vbmain = hiscore.get_vbox();
	Gtk::HBox namebox;
	Gtk::Label lname("Enter name: ");
	Gtk::Entry ename;
	Glib::ustring level;
	
	// Set entry properties.
	ename.set_max_length(10);
	ename.set_width_chars(10);
	ename.set_activates_default(true);
	
	// Pack the shiz.
	namebox.set_border_width(6);
	namebox.pack_start(lname);
	namebox.pack_start(ename);
	vbmain->pack_start(namebox);
	
	// Set the dialog's padding
	hiscore.set_border_width(10);
	
	// Add buttons
	hiscore.add_button(Gtk::Stock::OK, 101);
	hiscore.set_default_response(101);
	
	// Show the dialog.
	hiscore.show_all_children();
	hiscore.run();
	
	// Check if nothing was entered for name.
	if (ename.get_text().compare("") == 0)
		ename.set_text("None");
	
	return Glib::ustring::compose("%1", ename.get_text());
}

Glib::ustring Hiscores::get_skill_string(Bombs *b, int c){

std::string level;

	switch(c){
	case 0: level = "easy";
		break;
	case 1: level = "medium";
		break;
	case 2: level = "hard";
		break;
	default: level = "unknown";
	}

	return Glib::ustring::compose("%1", level);
}

// kontrola po kazde vyhrane hre jestli nahodou nebylo nejvyssi score
void Hiscores::check_high_scores(Bombs *b, int time, int skill)
{
	std::vector<std::string> data;
	Glib::ustring timestr;	
	bool flag = false;
	char sstr[20];	
	char * tokenPtr;
	
	std::ifstream in("./hiscores");
	
	if (in.is_open())
	{
		while (!in.eof())
		{			
			in.getline(sstr, 20);

			tokenPtr = strtok(sstr, ":");

			while(tokenPtr != NULL){
				data.push_back(tokenPtr);			
			tokenPtr = strtok(NULL, ":");	
			}			
		}		
		in.close();

		switch(skill){

			case 0:
				//std::cout <<"skill je "<< skill <<std::endl;									
				flag = compare_score(getintval(data[2]), skill);
				//std::cout << "flag je " << flag << std::endl;		//pokud flag bude true tak bude zapisovat do souboru
				if(flag) {				
					timestr = Glib::ustring::compose("%1", time);
					data[0] = get_skill_string(b, skill);
					data[1] = get_name(b);				
					data[2] = timestr;				
				}
				break;
			case 1:	
				std::cout << "skill medium je " << skill << std::endl;						
				flag = compare_score(getintval(data[5]), skill);
				if(flag) {				
					timestr = Glib::ustring::compose("%1", time);
					data[3] = get_skill_string(b, skill);
					data[4] = get_name(b);				
					data[5] = timestr;				
				}
				break;
			case 2:	
					std::cout << "skill hard je " << skill << std::endl;					
				flag = compare_score(getintval(data[8]), skill);
				if(flag) {				
					timestr = Glib::ustring::compose("%1", time);
					data[6] = get_skill_string(b, skill);
					data[7] = get_name(b);				
					data[8] = timestr;				
				}
				break;
			default:	 
				std::cout << "tohle by nemelo nastat:]" << std::endl; 
		}	
	}
	else
	{
		std::cout << "...nejspis neexistuje file hiscores" << std::endl;
	}
	

	// Open file write stream.
	std::ofstream out("./hiscores");
	
	for (int i = 0; i < (int)data.size(); i++)
	{				
		out << data[i];
		if((i-2)%3 == 0) out << "\n";
		else out << ":";
	}
	
	out.close();	
	
	if (flag) show_hiscores(b);
}

void Hiscores::show_hiscores(Bombs *b){

	Gtk::Dialog hiscore("HS", *b, true, true);
	Gtk::VBox *vbmain = hiscore.get_vbox();
	Gtk::HBox hsbox;
	Gtk::Label lname;	
	std::string ii;	
	Glib::ustring all;
	
	all = Glib::ustring::compose("%1", "Hiscores\n");		
	
	std::ifstream in("./hiscores");
	
	if (in.is_open())
	{
		for(int i = 0; i < 3; i++)
		{			
			std::getline(in, ii);						
			all = Glib::ustring::compose("%1%2%3",all,"\n", ii);					
		}
	}		
	in.close();
	
	lname.set_text(all);

	hsbox.set_border_width(2);
	hsbox.pack_start(lname);
	vbmain->pack_start(hsbox);
	
	hiscore.set_border_width(5);
	
	hiscore.add_button(Gtk::Stock::OK, 101);
	hiscore.set_default_response(101);
	
	hiscore.show_all_children();
	hiscore.run();	
}

