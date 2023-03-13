#ifndef HBA_CPP
#define HBA_CPP

#include "hba.h"

HBA::HBA(const std::string & name): Server(name) {
}

HBA & HBA::operator=(IOModule * const iom) {
	ioms.insert(iom);
	return *this;
}

ServerEvent *HBA::Submit(Task * const task) {
	taskq.push_back(task);
       	metrics.StartTask(taskq.size(), 0, task->size);
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	for (IOModules::iterator iom = ioms.begin(); iom != ioms.end(); iom++) {
		if (task->SERVERS().end() != task->SERVERS().find(*iom)) {
			return (*iom)->Submit(task);
		}
	}
	assert(0);
	return NULL;
}

std::pair<Task *, Event *> HBA::Finish(const uint64_t & t, Task * const task) {
	assert(task);
	for (TaskQ::iterator itask = taskq.begin(); itask != taskq.end(); itask++) {
		if (task == *itask) {
			taskq.erase(itask);
		       	metrics.EndTask(0);
		       	return std::pair<Task *, Event *>(task, NULL);
		}
	}
	assert(0);
	return std::pair<Task *, Event *>(NULL, NULL);
}

void HBA::print(std::ostream & o, const uint64_t & current_time) {
	o << "HBA " << name << std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	Server::print(o, current_time);
}
#endif // HBA_CPP
