#ifndef PORT_CPP
#define PORT_CPP

#include "port.h"

uint64_t Port::GetServiceTime(Task * const task) {
	const uint64_t xtime = double(task->size * 1000 * 1000) / (1024 * 1024 * mbps);
	return (xtime > service_time ? xtime : service_time);
}

Port::Port(const std::string & name, const uint64_t & p_service_time, const double & p_mbps): Server(name),
       	iom(NULL), service_time(p_service_time), mbps(p_mbps) {
}

Port & Port::operator=(IOModule * const p_iom) {
	assert(!iom);
	iom = p_iom;
	return *this;
}

ServerEvents Port::Submit(Task * const task, const uint64_t & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	// std::cout << "IN " << task << std::endl;
	taskq.push_back(task);
       	metrics.StartTask(taskq.size(), GetServiceTime(task), task->size);
	if (1 == taskq.size()) {
		ServerEvents retval;
	       	retval.insert(new ServerEvent(t + GetServiceTime(task), EvTyPortFinProc, this, task));
		return retval;
	}
	return ServerEvents();
}

ServerEvents Port::Start(const uint64_t & t) {
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	ServerEvents retval;
	if (0 < taskq.size()) {
	       	retval.insert(new ServerEvent(t + GetServiceTime(taskq.front()), EvTyPortFinProc, this, taskq.front()));
	}
	if (finished_task->SERVERS().end() != finished_task->SERVERS().find(iom)) {
	       	pending_tasks.insert(finished_task);
		ServerEvents sretval = iom->Submit(finished_task, t);
		retval.insert(sretval.begin(), sretval.end());
		return retval;
	}
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> Port::Finish(const uint64_t & t, Task * const task) {
	assert(task);
	// std::cout << "OUT " << task << std::endl;
	Tasks::iterator itask = pending_tasks.find(task);
	assert(pending_tasks.end() != itask);
       	pending_tasks.erase(itask);
       	metrics.EndTask(t - task->t);
       	return std::pair<Task *, Event *>(task, NULL);
}

void Port::print(std::ostream & o, const uint64_t & current_time) {
	o << "Port " << name << std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	Server::print(o, current_time);
}

void Ports::print(std::ostream & o, const uint64_t & current_time) {
	o << "Port\t\tIOPS\tMBPS\tIOS\tBYTES\tRT\tST\tQLEN" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (Ports::const_iterator i = begin(); i != end(); i++) {
	       	o << (*i)->name;
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

void HBA::print(std::ostream & o, const uint64_t & current_time) {
	o << "HBA " << name << std::endl;
}

void HBA::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "HBA " << name << std::endl;
	for (Ports::const_iterator i = Ports::begin(); i != Ports::end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t");
	}
}
#endif // PORT_CPP
