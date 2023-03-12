#ifndef HBA_H
#define HBA_H

#include <set>

#include "server.h"

class HBA: public Server {
	private:
		uint16_t current_qdepth;
	protected:
		IOModules ioms;
	public:
		HBA(const std::string & name);
		HBA & operator=(IOModule * const iom);
		const IOModules & IOMS() const { return ioms; }
	       	virtual ServerEvent *Submit(Task * const task);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

typedef std::set<HBA *> HBAs;

#endif // HBA_H
