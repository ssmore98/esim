#ifndef PORT_CPP
#define PORT_CPP

#include <iomanip>

#include "port.h"

Time Port::GetServiceTime(Task * const task) {
	Time xtime = task->size / mbps;
	// std::cout << (xtime > service_time ? xtime : service_time) << std::endl;
	return (xtime > service_time ? xtime : service_time);
}

Port::Port(const std::string & name, const Time & p_service_time, const double & p_mbps): Server(name),
       	iom(NULL), service_time(p_service_time), mbps(p_mbps) {
}

Port & Port::operator=(IOModule * const p_iom) {
	assert(!iom);
	iom = p_iom;
	return *this;
}

ServerEvents Port::Submit(Task * const task, const Time & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	// std::cout << "IN " << task << std::endl;
	taskq.push_back(task, t);
       	metrics.QueueTask(taskq.size(), task->size);
	if (1 == taskq.size()) {
		ServerEvents retval;
	       	const std::pair<Task *, Time> started_task = taskq.start_task(t);
		metrics.StartTask(started_task.second, GetServiceTime(started_task.first));
	       	retval.insert(new ServerEvent(t + GetServiceTime(started_task.first), EvTyPortFinProc, this, started_task.first));
		return retval;
	}
	return ServerEvents();
}

ServerEvents Port::Start(const Time & t) {
	assert(0 < taskq.size());
	Task * const finished_task = taskq.pop_front();
       	ServerEvents retval;
	if (0 < taskq.size()) {
	       	const std::pair<Task *, Time> started_task = taskq.start_task(t);
		metrics.StartTask(started_task.second, GetServiceTime(started_task.first));
	       	retval.insert(new ServerEvent(t + GetServiceTime(started_task.first), EvTyPortFinProc, this, started_task.first));
	}
	if (finished_task->SERVERS().end() != finished_task->SERVERS().find(iom)) {
	       	pending_tasks.insert(finished_task);
		ServerEvents sretval = iom->Submit(finished_task, t);
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << sretval << std::endl;
		retval.insert(sretval.begin(), sretval.end());
		return retval;
	}
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> Port::Finish(const Time & t, Task * const task) {
	assert(task);
	// std::cout << "OUT " << task << std::endl;
	Tasks::iterator itask = pending_tasks.find(task);
	assert(pending_tasks.end() != itask);
       	pending_tasks.erase(itask);
       	metrics.EndTask(t - task->t);
       	return std::pair<Task *, Event *>(task, NULL);
}

void Port::print(std::ostream & o, const Time & current_time) {
	o << "Port " << name << std::endl;
	if (current_time) o << "\tIOPS " << metrics.N_TASKS() / current_time << std::endl;
	if (current_time) o << "\tMBPS " << metrics.SZ_SUM() / current_time << std::endl;
	Server::print(o, current_time);
}

void Ports::print(std::ostream & o, const Time & current_time) {
	o << "Port\t\tIOPS\tTPUT\t\tIOS\t\tBYTES\tRT\tST\tQLEN\tQTIME" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (Ports::const_iterator i = begin(); i != end(); i++) {
	       	o << std::setw(10) << std::left << (*i)->name;
		o << '\t';
		(*i)->METRICS().print(o, current_time);
		o << std::endl;
	}
	o << std::string(80, '-') << std::endl;
}

void Port::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "Port " << name << " -> IO Module " << iom->name << std::endl;
}
HBA::HBA(const std::string & p_name): name(p_name) {
}

HBA & HBA::operator=(Port * const port) {
	Ports::insert(port);
	return *this;
}

void HBA::print(std::ostream & o, const Time & current_time) {
	o << "HBA " << name << std::endl;
}

void HBA::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "HBA " << name << std::endl;
	for (Ports::const_iterator i = Ports::begin(); i != Ports::end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t");
	}
}
#endif // PORT_CPP
