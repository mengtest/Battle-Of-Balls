#ifndef _Spore_H_
#define _Spore_H_

#include "Entity.h"

class Spore : public Entity {
public:
	Spore();
	~Spore();

	static Spore * create(const std::string& filename);
	bool init(const std::string& filename);

	static Spore * create(int id);
	bool init(int id);

	int getID();
private:
	int _skinID;
};

#endif
