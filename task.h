#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <deque>
#include <iostream>

#define MAX_TASKQ 1024

class Server;
class Generator;

class Task {
	public:
		const uint64_t t;
		const size_t size;
		Server * const server;
		Generator * const generator;
		Task(const uint64_t & p_t, const size_t & p_size, Server * const p_server, Generator * const p_generator);
		virtual std::string Name() const = 0;
		virtual ~Task() {
		}
};

std::ostream & operator<<(std::ostream & o, Task * const io);

typedef std::deque<Task *> TaskQ;

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq);

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

#endif // TASK_H
