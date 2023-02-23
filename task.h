#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <deque>
#include <iostream>
#include <cassert>
#include <vector>

#define MAX_TASKQ (64 * 1024)

class Server;
class Generator;

class MasterTask;

class Task {
	public:
		const uint64_t t;
		const size_t size;
		Server * const server;
		Generator * const generator;
		const bool is_read;
		const bool is_random;
		Task(const uint64_t & p_t, const size_t & p_size, const bool & pis_read, const bool & pis_random,
			       	Server * const p_server, Generator * const p_generator);
		virtual MasterTask * & MTASK();
		virtual ~Task() {
		}
};

std::ostream & operator<<(std::ostream & o, Task * const io);

typedef std::deque<Task *> TaskQ;
typedef std::vector<Task *> Tasks;

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq);

#if 0
class ReadTask: public Task {
	public:
		ReadTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator);
	       	virtual std::string Name() const;
};

class WriteTask: public Task {
	public:
		WriteTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator);
	       	virtual std::string Name() const;
};
#endif

class SubTask: public Task {
	protected:
		MasterTask * mtask;
	public:
		SubTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator);
		virtual MasterTask * & MTASK();
};

typedef std::vector<SubTask *> SubTasks;

class MasterTask: public Task {
	protected:
		SubTasks tasks;
	public:
	       	MasterTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator, SubTasks p_tasks);
		virtual MasterTask * & MTASK();
	       	size_t EndTask(Task * const task, const uint64_t & t);
};

#endif // TASK_H
