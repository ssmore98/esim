#ifndef PORT_H
#define PORT_H

#include <set>

#include "server.h"

class Port: public Server {
	protected:
		Tasks pending_tasks;
		IOModule * iom;
	       	Time GetServiceTime(Task * const task);
	public:
		const Time service_time;
		const DataRate mbps;
		Port(const std::string & name, const Time & p_service_time, const double & p_mbps);
		Port & operator=(IOModule * const p_iom);
	       	virtual ServerEvents Submit(Task * const task, const Time & t);
	       	virtual std::pair<Task *, Event *> Finish(const Time & t, Task * const task);
		virtual void print(std::ostream & o, const Time & current_time);
	       	virtual ServerEvents Start(const Time & t);
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
		IOModule * const IOM() const { return iom; }
};

class Ports: public std::set<Port *> {
	public:
	       	void print(std::ostream & o, const Time & current_time);
};

class HBA: public Ports {
	public:
		const std::string name;
		HBA(const std::string & p_name);
		HBA & operator=(Port * const port);
		Ports & PORTS() { return *this; }
		void print(std::ostream & o, const Time & current_time);
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

typedef std::set<HBA *> HBAs;

#endif // PORT_H
