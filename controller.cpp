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

void Controller::ScheduleTask(RAID * const raid, Task * const task, Events & events) {
       	TaskList tlist = raid->Execute(task);
       	ControllerTaskList::iterator ictl = ctl.insert(ctl.end(), ControllerTaskList::value_type(task, Tasks()));
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
						       	ServerEvent * const event = (*hba)->Submit(*t);
							if (event) {
							       	events.push_back(event);
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
		       	ScheduleTask((*i)->raid, *j, events);
		}
	}
}

Task * const Controller::EndTask(const uint64_t & t, Task * const task) {
	for (ControllerTaskList::iterator i = ctl.begin(); i != ctl.end(); i++) {
		if (i->second.end() != i->second.find(task)) {
			// std::cout << "found!\n";
			i->second.erase(i->second.find(task));
			if (0 == i->second.size()) {
			       	// std::cout << "done!\n";
				Task * const retval = i->first;
				ctl.erase(i);
				return retval;
			}
			return NULL;
		}
	}
	assert(0);
}

#endif // CONTROLLER_CPP
