#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <set>

#include "generator.h"
#include "port.h"

typedef std::map<Task *, Tasks> ControllerTaskList;
std::ostream & operator<<(std::ostream & o, const ControllerTaskList & ctl);

class Controller {
	protected:
		Generators generators;
		HBAs hbas;
		ControllerTaskList ctl;
		Metrics metrics;
	public:
		const std::string name;
		Controller(const std::string & p_name);
		Controller & operator=(Generator * const generator);
		Controller & operator=(HBA * const hba);
		void Begin(Events & events, const Time & t);
	       	void ScheduleTask(RAID * const raid, Task * const task, Events & events, const Time & current_time);
	       	Task * const EndTask(const Time & t, Task * const task);
		void print(std::ostream & o, const Time & current_time) const;
		const HBAs & HBAS() const { return hbas; }
		const Generators & GENERATORS() const { return generators; }
		const Metrics & METRICS() const { return metrics; }
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

class Controllers: public std::set<Controller *> {
	public:
	       	void print(std::ostream & o, const Time & current_time) const;
};

class Filer: public Controllers {
	public:
		const std::string name;
		Filer(const std::string & p_name);
		Filer & operator=(Controller * const controller);
		void print(std::ostream & o, const Time & current_time) const;
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

class Filers: public std::set<Filer *> {
	public:
	       	void print(std::ostream & o, const Time & current_time) const;
};

#endif // CONTROLLER_H
