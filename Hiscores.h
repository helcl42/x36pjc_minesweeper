#ifndef HISCORES_H_
#define HISCORES_H_

#include "Bombs.h"
class Bombs;

class Hiscores
{
	public:	
		void show_hiscores(Bombs *b);
		void check_high_scores(Bombs *b, int t, int c);

	protected:		
		static Glib::ustring get_name(Bombs *b);
		static Glib::ustring get_skill_string(Bombs * b, int c);		
		bool compare_score(int a, int b);
		int getintval(std::string g);
};

#endif /*HISCORES_H_*/
