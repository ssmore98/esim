#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include "controller.h"

Controller::Controller(const std::string & p_name): name(p_name) {
}

Controller & Controller::operator=(Generator * const generator) {
	generators.insert(generator);
	generator->operator=(this);
	return *this;
}

Controller & Controller::operator=(HBA * const hba) {
	hbas.insert(hba);
	return *this;
}

void Controller::ScheduleTask(RAID * const raid, Task * const task, Events & events, const uint64_t & current_time) {
	std::cout << task << std::endl;
       	ControllerTaskList::iterator ictl = ctl.insert(ctl.end(), ControllerTaskList::value_type(task, Tasks()));
       	metrics.StartTask(ctl.size(), 0, task->size);
       	TaskList tlist = raid->Execute(task);
	if (0 == tlist.size()) {
	       	ctl.erase(ictl);
	       	metrics.EndTask(0);
		Task * const next_task = task->generator->EndTask(task, current_time);
		if (next_task) ScheduleTask(raid, next_task, events, current_time);
		return;
	}
       	for (TaskList::iterator k = tlist.begin(); k != tlist.end(); k++) {
	       	Drive * const drive = k->first;
	       	bool done = false;
	       	for (HBAs::iterator hba = hbas.begin(); hba != hbas.end(); hba++) {
		       	for (IOModules::const_iterator iom = (*hba)->IOMS().begin(); iom != (*hba)->IOMS().end(); iom++) {
			       	Shelf * const shelf = (*iom)->SHELF();
			       	for (Drives::const_iterator d = shelf->DRIVES().begin(); d != shelf->DRIVES().end(); d++) {
				       	if (drive == *d) {
					       	// std::cout << drive << std::endl;
					       	for (Tasks::iterator t = k->second.begin(); t != k->second.end(); t++) {
						       	**t = drive;
						       	**t = *iom;
						       	**t = *hba;
						       	// std::cout << '\t' << *t << std::endl;
							ictl->second.insert(*t);
						       	ServerEvents sevents = (*hba)->Submit(*t, current_time);
							for (ServerEvents::iterator sevent = sevents.begin(); sevent != sevents.end();
								       sevent++) {
							       	events.push_back(*sevent);
							       	std::push_heap(events.begin(), events.end(), cmp);
							}
					       	}
					       	done = true;
				       	}
				       	if (done) break;
			       	}
			       	if (done) break;
		       	}
		       	if (done) break;
	       	}
       	}
}

void Controller::Begin(Events & events, const uint64_t & t) {
	for (Generators::iterator i = generators.begin(); i != generators.end(); i++) {
		Tasks tasks = (*i)->Begin(events, t);
		for (Tasks::iterator j = tasks.begin(); j != tasks.end(); j++) {
		       	ScheduleTask((*i)->raid, *j, events, t);
		}
	}
}

Task * const Controller::EndTask(const uint64_t & t, Task * const task) {
	// std::cout << task << std::endl;
	for (ControllerTaskList::iterator i = ctl.begin(); i != ctl.end(); i++) {
		if (i->second.end() != i->second.find(task)) {
			// std::cout << "found!\n";
			i->second.erase(i->second.find(task));
			if (0 == i->second.size()) {
			       	// std::cout << "done!\n";
				Task * const retval = i->first;
				ctl.erase(i);
			       	metrics.EndTask(0);
				return retval;
			}
			return NULL;
		}
	}
	assert(0);
}

static std::string concath(std::string & a, HBA * b) { return b->name + " " + a; }
static std::string concatg(std::string & a, Generator * b) { return b->name + " " + a; }

void Controller::print(std::ostream & o, const uint64_t & current_time) const {
	o << "Controller " << name <<
	       	": HBAs(" << std::accumulate(hbas.begin(), hbas.end(), std::string(""), concath) << ")" <<
	       	" Generators(" << std::accumulate(generators.begin(), generators.end(), std::string(""), concatg) << ")" <<
	       	std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	o << metrics;
}

#endif // CONTROLLER_CPP
