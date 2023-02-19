#ifndef TASK_CPP
#define TASK_CPP

#include "task.h"
#include "server.h"
#include "generator.h"

Task::Task(const uint64_t & p_t, const size_t & p_size, Server * const p_server, Generator * const p_generator):
       	t(p_t), size(p_size), server(p_server), generator(p_generator) {
}

std::ostream & operator<<(std::ostream & o, Task * const io) {
	o << io->Name() << " t=" << io->t << " size=" << io->size << " server=" << io->server->name  << " generator=" << io->generator->name << std::endl;
	return o;
}

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq) {
	for (TaskQ::const_iterator i = taskq.begin(); i != taskq.end(); i++) {
		o << (*i)->t << " ";
	}
	o << std::endl;
	return o;
}

ReadTask::ReadTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator):
       	Task(t, size, server, generator) {
}

WriteTask::WriteTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator):
       	Task(t, size, server, generator) {
}

std::string ReadTask::Name()  const { return std::string("READ");  }
std::string WriteTask::Name() const { return std::string("WRITE"); }

#endif // TASK_CPP
