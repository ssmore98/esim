#ifndef HBA_CPP
#define HBA_CPP

#include "hba.h"

HBA::HBA(const std::string & name, const uint64_t & p_service_time): Server(name), service_time(p_service_time) {
}

HBA & HBA::operator=(IOModule * const iom) {
	ioms.insert(iom);
	return *this;
}

ServerEvents HBA::Submit(Task * const task, const uint64_t & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	// std::cout << "IN " << task << std::endl;
	taskq.push_back(task);
       	metrics.StartTask(taskq.size(), service_time, task->size);
	if (1 == taskq.size()) {
		ServerEvents retval;
	       	retval.insert(new ServerEvent(t + service_time, EvTyHBAFinProc, this));
		return retval;
	}
	return ServerEvents();
}

ServerEvents HBA::Start(const uint64_t & t) {
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	metrics.EndTask(t - finished_task->t);
       	ServerEvents retval;
	if (0 < taskq.size()) {
	       	retval.insert(new ServerEvent(t + service_time, EvTyHBAFinProc, this));
	}
	for (IOModules::iterator iom = ioms.begin(); iom != ioms.end(); iom++) {
		if (finished_task->SERVERS().end() != finished_task->SERVERS().find(*iom)) {
		       	pending_tasks.insert(finished_task);
			ServerEvents sretval = (*iom)->Submit(finished_task, t);
			retval.insert(sretval.begin(), sretval.end());
			return retval;
		}
	}
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> HBA::Finish(const uint64_t & t, Task * const task) {
	assert(task);
	// std::cout << "OUT " << task << std::endl;
	Tasks::iterator itask = pending_tasks.find(task);
	assert(pending_tasks.end() != itask);
       	pending_tasks.erase(itask);
       	return std::pair<Task *, Event *>(task, NULL);
}

void HBA::print(std::ostream & o, const uint64_t & current_time) {
	o << "HBA " << name << std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	Server::print(o, current_time);
}
#endif // HBA_CPP
