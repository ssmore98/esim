#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>

#include "event.h"
#include "task.h"
#include "raid.h"

class Controller;

class Generator {
	private:
		static uint16_t index;
	protected:
		// Events & events;
	       	static std::default_random_engine generator;
	       	std::exponential_distribution<double> rw_type_distr;
	       	std::exponential_distribution<double> loc_distr;
	       	uint64_t ia_time_sum;
	       	uint64_t ia_time_count;
	       	uint64_t last_task_time;
		Tasks    pending;
		Controller *controller;
	public:
		// Server * const server;
		// Server * const iom;
		const uint16_t my_index;
		const std::string name;
		const uint16_t percent_read;
		const uint16_t percent_random;
		const size_t size;
		RAID * const raid;
		Generator(const std::string & p_name, const size_t & p_size, const uint16_t & p_percent_read,
			       	const uint16_t & p_percent_random, RAID * const p_raid);
		virtual Tasks Begin(Events & events, const uint64_t & t) = 0;
	       	virtual Task * const NextTask(Events & events, const uint64_t & t) = 0;
	       	virtual Task * const EndTask(Task * const task, const uint64_t & t) = 0;
		virtual ~Generator();
		Generator & operator=(Controller * const c);
		Controller * const GetController() const { return controller; }
};

typedef std::set<Generator *> Generators;

class RateGenerator : public Generator {
	protected:
	       	std::exponential_distribution<double> ia_time_distr;
	public:
		RateGenerator(const std::string & name, const size_t & p_size, const uint16_t & percent_read,
			       	const uint16_t & percent_random, const unsigned int & ia_time, RAID * const raid);
		virtual Tasks Begin(Events & events, const uint64_t & t);
	       	virtual Task * const NextTask(Events & events, const uint64_t & t);
	       	virtual Task * const EndTask(Task * const task, const uint64_t & t);
};

class QueueGenerator : public Generator {
	private:
		Task * const CreateTask(const uint64_t & t);
	public:
	       	const unsigned int qdepth;
		QueueGenerator(const std::string & name, const size_t & p_size, const uint16_t & percent_read,
			       	const uint16_t & percent_random, const unsigned int & p_qdepth, RAID * const raid);
		virtual Tasks Begin(Events & events, const uint64_t & t);
	       	virtual Task * const EndTask(Task * const task, const uint64_t & t);
	       	virtual Task * const NextTask(Events & events, const uint64_t & t);
};

#endif // GENERATOR_H
