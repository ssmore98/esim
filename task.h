#ifndef TASK_H
#define TASK_H

#include <cstdint>
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
std::ostream & operator<<(std::ostream & o, const Tasks & tasks);

class SubTask: public Task {
	protected:
		MasterTask * mtask;
	public:
		SubTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator);
		virtual MasterTask * & MTASK();
};

class MasterTask: public Task {
	protected:
		Tasks tasks;
	public:
	       	MasterTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator, Tasks p_tasks);
		virtual MasterTask * & MTASK();
	       	size_t EndTask(Task * const task, const uint64_t & t);
};

#endif // TASK_H
