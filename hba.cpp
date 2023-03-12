#ifndef HBA_CPP
#define HBA_CPP

#include "hba.h"

HBA::HBA(const std::string & name): Server(name), current_qdepth(0) {
}

HBA & HBA::operator=(IOModule * const iom) {
	ioms.insert(iom);
	return *this;
}

ServerEvent *HBA::Submit(Task * const task) {
	current_qdepth += 1;
       	metrics.StartTask(task, current_qdepth, 0);
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
	assert(current_qdepth);
	current_qdepth -= 1;
       	metrics.EndTask(task, 0);
	return std::pair<Task *, Event *>(task, NULL);
}

void HBA::print(std::ostream & o, const uint64_t & current_time) {
	o << "HBA " << name << std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	Server::print(o, current_time);
}
#endif // HBA_CPP
