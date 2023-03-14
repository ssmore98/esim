#ifndef HBA_H
#define HBA_H

#include <set>

#include "server.h"

class HBA: public Server {
	protected:
		Tasks pending_tasks;
		IOModules ioms;
	public:
		const uint64_t service_time;
		HBA(const std::string & name, const uint64_t & p_service_time);
		HBA & operator=(IOModule * const iom);
		const IOModules & IOMS() const { return ioms; }
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
		virtual void print(std::ostream & o, const uint64_t & current_time);
	       	virtual ServerEvents Start(const uint64_t & t);
};

typedef std::set<HBA *> HBAs;

#endif // HBA_H
