#ifndef HBA_H
#define HBA_H

#include <set>

#include "server.h"

class HBA: public Server {
	protected:
		IOModules ioms;
	public:
		HBA(const std::string & name);
		HBA & operator=(IOModule * const iom);
		const IOModules & IOMS() const { return ioms; }
	       	virtual ServerEvent *Submit(Task * const task);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
};

typedef std::set<HBA *> HBAs;

#endif // HBA_H
