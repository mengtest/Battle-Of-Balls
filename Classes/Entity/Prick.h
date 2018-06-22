#ifndef _Prick_H_
#define _Prick_H_

#include "Entity.h"

class Prick : public Entity {
public:
	Prick();
	~Prick();

	static Prick * create(const std::string& filename);
	bool init(const std::string& filename);
};

#endif
