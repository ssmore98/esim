#ifndef SERVER_H
#define SERVER_H

// https://confluence.ngage.netapp.com/display/PALM/Discrete+Event+Simulator#DiscreteEventSimulator-Server

#include <cstdint>
#include <random>
#include <map>
#include <set>

#include "task.h"
#include "event.h"
#include "metrics.h"

class Server {
	private:
		static uint16_t index;
	protected:
	       	static std::default_random_engine generator;
		TaskQ taskq;
		Metrics metrics;
	public:
		const uint16_t my_index;
		const std::string name;
		Server(const std::string & p_name);
	       	virtual ~Server() = 0;
		virtual void print(std::ostream & o, const uint64_t & current_time);
		const Metrics & METRICS() const { return metrics; }
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t) = 0;
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task) = 0;
	       	virtual ServerEvents Start(const uint64_t & t) = 0;
};

class Servers: public std::set<Server *> {
	public:
	       	void print(std::ostream & o, const uint64_t & current_time) const;
};

class Shelf;

class Drive: public Server {
	protected:
		Shelf *shelf;
	public:
		Drive(const std::string & name);
		Drive & operator=(Shelf * const p_self);
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t) = 0;
		Shelf * const SHELF() const;
	       	virtual ServerEvents Start(const uint64_t & t) = 0;
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

class Drives: public std::set<Drive *> {
	public:
	       	void print(std::ostream & o, const uint64_t & current_time) const;
};

class IOModule: public Server {
	protected:
		Shelf *shelf;
		Tasks pending_tasks;
	       	uint64_t GetServiceTime(Task * const task);
	public:
		const uint64_t service_time;
		const double mbps;
		IOModule(const std::string & name, const uint64_t & p_service_time, const double & p_mbps);
	       	virtual ~IOModule();
	       	virtual void print(std::ostream & o, const uint64_t & current_time);
		IOModule & operator=(Shelf * const p_self);
		Shelf * const SHELF() const { return shelf; }
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
	       	virtual ServerEvents Start(const uint64_t & t);
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

class IOModules: public std::set<IOModule *> {
	public:
	       	void cumulative_print(std::ostream & o, const uint64_t & current_time) const;
	       	void print(std::ostream & o, const uint64_t & current_time) const;
};

class Shelf {
	protected:
		Drives drives;
		IOModules ioms;
	public:
		const std::string name;
		const uint16_t slots;
		Shelf(const std::string & name, const uint16_t & p_slots);
	       	virtual ~Shelf();
		Shelf & operator=(Drive * const drive);
		Shelf & operator=(IOModule * const iom);
	       	void print(std::ostream & o, const uint64_t & current_time);
		const Drives & DRIVES() const { return drives; }
	       	void PrintConfig(std::ostream & o, const std::string & prefix) const;
};

typedef std::set<Shelf *> Shelves;

class SSD_PM1733a: public Drive {
	protected:
	       	std::exponential_distribution<double> read_service_time_distr;
	       	std::exponential_distribution<double> write_service_time_distr;
		virtual uint64_t GetServiceTime(Task * const task);
	public:
	       	SSD_PM1733a(const std::string & name);
	       	virtual ~SSD_PM1733a();
		virtual size_t StripeSize() const;
		virtual void print(std::ostream & o, const uint64_t & current_time);
	       	virtual ServerEvents Submit(Task * const task, const uint64_t & t);
	       	virtual std::pair<Task *, Event *> Finish(const uint64_t & t, Task * const task);
	       	virtual ServerEvents Start(const uint64_t & t);
};

#endif // SERVER_H
