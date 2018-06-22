#ifndef _Bean_H_
#define _Bean_H_

#include "Entity.h"

class Bean : public Entity {
public:
	Bean();
	~Bean();

	static Bean * create(const std::string& filename);
	bool init(const std::string& filename);
};

#endif
