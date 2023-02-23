#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>

#include "event.h"

class Generator {
	private:
		static uint16_t index;
	protected:
		Events & events;
	       	static std::default_random_engine generator;
	       	std::exponential_distribution<double> rw_type_distr;
	       	std::exponential_distribution<double> loc_distr;
	       	uint64_t ia_time_sum;
	       	uint64_t ia_time_count;
	       	uint64_t last_task_time;
	public:
		Server * const server;
		const uint16_t my_index;
		const std::string name;
		const uint16_t percent_read;
		const uint16_t percent_random;
		const size_t size;
		Generator(const std::string & p_name, const size_t & p_size, const uint16_t & p_percent_read, const uint16_t & p_percent_random,
			       	Events & p_events, Server * const p_server);
	       	void IssueTask(const uint64_t & t);
	       	virtual void StartTask(const uint64_t & t) = 0;
	       	virtual void EndTask(const uint64_t & t) = 0;
		virtual ~Generator();
};

typedef std::vector<Generator *> Generators;

class RateGenerator : public Generator {
	protected:
	       	std::exponential_distribution<double> ia_time_distr;
	public:
		RateGenerator(const std::string & name, const size_t & p_size, const uint16_t & percent_read, const uint16_t & percent_random,
			       	Events & events, const unsigned int & ia_time, Server * const server);
	       	void StartTask(const uint64_t & t);
	       	void EndTask(const uint64_t & t);
};

class QueueGenerator : public Generator {
	public:
	       	const unsigned int qdepth;
		QueueGenerator(const std::string & name, const size_t & p_size, const uint16_t & percent_read, const uint16_t & percent_random,
			       	Events & events, const unsigned int & p_qdepth, Server * const server);
	       	void StartTask(const uint64_t & t);
	       	void EndTask(const uint64_t & t);
};


#endif // GENERATOR_H
