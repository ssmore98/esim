#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <set>

#include "generator.h"
#include "hba.h"

typedef std::map<Task *, Tasks> ControllerTaskList;

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
		void Begin(Events & events, const uint64_t & t);
	       	void ScheduleTask(RAID * const raid, Task * const task, Events & events);
	       	Task * const EndTask(const uint64_t & t, Task * const task);
		void print(std::ostream & o, const uint64_t & current_time) const;
};

typedef std::set<Controller *> Controllers;

#endif // CONTROLLER_H
