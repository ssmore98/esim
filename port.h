#ifndef PORT_H
#define PORT_H

#include <set>

#include "server.h"

class Port: public Server {
	protected:
		Tasks pending_tasks;
		IOModules ioms;
	       	uint64_t GetServiceTime(Task * const task);
	public:
		const uint64_t service_time;
		const double mbps;
		Port(const std::string & name, const uint64_t & p_service_time, const double & p_mbps);
		Port & operator=(IOModule * const iom);
		const IOModules & IOMS() const { return ioms; }
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
		virtual void print(std::ostream & o, const uint64_t & current_time);
	       	virtual ServerEvents Start(const uint64_t & t);
};

typedef std::set<Port *> Ports;

class HBA: public Ports {
	public:
		const std::string name;
		HBA(const std::string & p_name);
		HBA & operator=(Port * const port);
		Ports & PORTS() { return *this; }
		void print(std::ostream & o, const uint64_t & current_time);
};

typedef std::set<HBA *> HBAs;

#endif // PORT_H
