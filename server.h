#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <random>

#include "task.h"
#include "event.h"

class Server {
	private:
		static uint16_t index;
	protected:
	       	static std::default_random_engine generator;
		TaskQ taskq;
		uint64_t n_tasks;
		uint64_t task_time;
		uint64_t qd_sum;
		uint64_t svc_sum;
		uint64_t sz_sum;
		virtual uint64_t GetServiceTime(Task * const task) = 0;
	public:
		const size_t rate;
		const uint16_t my_index;
		const std::string name;
		Server(const std::string & p_name, const unsigned int & service_time, const size_t & p_rate);
	       	virtual void Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
	       	virtual ~Server();
};

typedef std::vector<Server *> Servers;

class SSD_PM1733a: public Server {
	protected:
	       	std::exponential_distribution<double> read_service_time_distr;
	       	std::exponential_distribution<double> write_service_time_distr;
		virtual uint64_t GetServiceTime(Task * const task);
		const uint64_t & current_time;
	public:
	       	SSD_PM1733a(const std::string & name, const uint64_t & p_t);
	       	virtual ~SSD_PM1733a();
};

#endif // SERVER_H
