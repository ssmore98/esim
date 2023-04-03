#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <iomanip>

#include "controller.h"

Controller::Controller(const std::string & p_name): name(p_name) {
}

Filer::Filer(const std::string & p_name): name(p_name) {
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

Filer & Filer::operator=(Controller * const controller) {
	Controllers::insert(controller);
	return *this;
}

void Controller::ScheduleTask(RAID * const raid, Task * const task, Events & events, const Time & current_time) {
	// std::cout << __FILE__ << ':' << __LINE__ << ' '  << task << " " << events << std::endl;
       	ControllerTaskList::iterator ictl = ctl.insert(ctl.end(), ControllerTaskList::value_type(task, Tasks()));
       	metrics.QueueTask(ctl.size(), task->size);
       	metrics.StartTask(Time(), Time());
       	TaskList tlist = raid->Execute(task);
	assert(tlist.size());
       	for (TaskList::iterator k = tlist.begin(); k != tlist.end(); k++) {
	       	Drive * const drive = k->first;
		// std::cout << __FILE__ << ':' << __LINE__ << ' '  << drive->name << " " << k->second;
	       	bool done = false;
	       	for (HBAs::iterator hba = hbas.begin(); hba != hbas.end(); hba++) {
	       	       	for (Ports::iterator port = (*hba)->PORTS().begin(); port != (*hba)->PORTS().end(); port++) {
				Shelf * const shelf = (*port)->IOM()->SHELF();
				for (Drives::const_iterator d = shelf->DRIVES().begin(); d != shelf->DRIVES().end(); d++) {
					// std::cout << __FILE__ << ':' << __LINE__ << ' '  << '\t' << (*d)->name << std::endl;
				       	if (drive == *d) {
					       	// std::cout << drive->name << std::endl;
					       	for (Tasks::iterator t = k->second.begin(); t != k->second.end(); t++) {
						       	**t = drive;
						       	**t = (*port)->IOM();
						       	**t = *port;
						       	// std::cout << '\t' << *t << std::endl;
							ictl->second.insert(*t);
						       	ServerEvents sevents = (*port)->Submit(*t, current_time);
						       	// std::cout << __FILE__ << ':' << __LINE__ << " " << sevents << std::endl;
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
		}
		assert(done);
       	}
	// std::cout << __FILE__ << ':' << __LINE__ << ' '  << task << " " << events << std::endl;
}

void Controller::Begin(Events & events, const Time & t) {
	for (Generators::iterator i = generators.begin(); i != generators.end(); i++) {
		Tasks tasks = (*i)->Begin(events, t);
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' '  << tasks.size() << std::endl;
		for (Tasks::iterator j = tasks.begin(); j != tasks.end(); j++) {
		       	ScheduleTask((*i)->raid, *j, events, t);
		}
	}
}

Task * const Controller::EndTask(const Time & t, Task * const task) {
	// std::cout << task << ' ' << ctl << std::endl;
	for (ControllerTaskList::iterator i = ctl.begin(); i != ctl.end(); i++) {
		if (i->second.end() != i->second.find(task)) {
			// std::cout << "found!\n";
			i->second.erase(i->second.find(task));
			if (0 == i->second.size()) {
			       	// std::cout << "done!\n";
				Task * const retval = i->first;
				ctl.erase(i);
			       	metrics.EndTask(t - task->t);
				return retval;
			}
			return NULL;
		}
	}
	assert(0);
}

static std::string concath(std::string & a, HBA * b) { return b->name + "," + a; }
static std::string concatg(std::string & a, Generator * b) { return b->name + "," + a; }

void Controller::print(std::ostream & o, const Time & current_time) const {
	o << "Controller " << name <<
	       	": HBAs (" << std::accumulate(hbas.begin(), hbas.end(), std::string(""), concath) << ")" <<
	       	" Generators (" << std::accumulate(generators.begin(), generators.end(), std::string(""), concatg) << ")" <<
	       	std::endl;
	if (current_time) o << "\tIOPS " << metrics.N_TASKS() / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM()) / (current_time) << std::endl;
	o << metrics;
}

void Controllers::print(std::ostream & o, const Time & current_time) const {
	o << "Controller\tIOPS\tTPUT\t\tIOS\t\tBYTES\tRT\tST\tQLEN\tQTIME" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (Controllers::const_iterator i = begin(); i != end(); i++) {
	       	o << (*i)->name;
		o << '\t';
	       	(*i)->METRICS().print(o, current_time);
		o << std::endl;
	}
	o << std::string(80, '-') << std::endl;
}

void Controller::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "Controller " << name << std::endl;
	for (HBAs::const_iterator i = hbas.begin(); i != hbas.end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t");
	}
}

std::ostream & operator<<(std::ostream & o, const ControllerTaskList & ctl) {
	for (ControllerTaskList::const_iterator i = ctl.begin(); i != ctl.end(); i++) {
		o << "Master(" << i->first << ")";
	}
	return o;
}

// static std::string concatc(std::string & a, Controller * b) { return b->name + " " + a; }
static IOS sumtasks(IOS acc, Controller * const x) { return acc + x->METRICS().N_TASKS(); }
static Bytes sumszs(Bytes acc, Controller * const x) { return acc + x->METRICS().SZ_SUM(); }
void Filer::print(std::ostream & o, const Time & current_time) const {
	o << std::setw(10) << std::left  << name;
	o << "\t" << (current_time ? std::accumulate<Controllers::iterator, IOS>(Controllers::begin(), Controllers::end(), 0, sumtasks)
		       	/ current_time : 0);
	o << "\t" << (current_time ? std::accumulate<Controllers::iterator, Bytes>(Controllers::begin(), Controllers::end(), 0, sumszs)
			       	 / current_time : DataRate(0));
	o << "\t" << std::setw(8) << std::left <<
	       	std::accumulate<Controllers::iterator, IOS>(Controllers::begin(), Controllers::end(), 0, sumtasks);
	o << "\t" << std::left <<
	       	std::accumulate<Controllers::iterator, Bytes>(Controllers::begin(), Controllers::end(), 0, sumszs);
}

void Filer::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "Filer " << name << std::endl;
	for (Controllers::const_iterator i = Controllers::begin(); i != Controllers::end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t");
	}
}

void Filers::print(std::ostream & o, const Time & current_time) const {
	o << "Filer\t\tIOPS\tTPUT\t\tIOS\t\tBYTES" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (Filers::const_iterator i = begin(); i != end(); i++) {
	       	(*i)->print(o, current_time);
		o << std::endl;
	}
	o << std::string(80, '-') << std::endl;
}


#endif // CONTROLLER_CPP
