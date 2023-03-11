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
	return std::pair<Task *, Event *>(task, NULL);
}

#endif // HBA_CPP
