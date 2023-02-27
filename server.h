#ifndef SERVER_H
#define SERVER_H

// https://confluence.ngage.netapp.com/display/PALM/Discrete+Event+Simulator#DiscreteEventSimulator-Server

#include <cstdint>
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
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t) = 0;
	public:
		const uint16_t my_index;
		const std::string name;
		Server(const std::string & p_name, const unsigned int & service_time);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t) = 0;
		virtual size_t StripeSize() const = 0;
	       	virtual ~Server() = 0;
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
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
};

class RAID_0: public Server {
	protected:
	       	// size_t alignment;
	       	// size_t space_left_in_stripe;
	       	// Servers::iterator next_server;
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
	       	// std::uniform_int_distribution<uint16_t> alignment_distr;
		virtual uint64_t GetServiceTime(Task * const task);
		const uint64_t & current_time;
		Servers servers;
	public:
		const size_t stripe_width;
		RAID_0(const std::string & name, Servers & p_servers, const size_t & p_stripe_width, const uint64_t & t);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_0();
};

class RAID_1: public Server {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
		virtual uint64_t GetServiceTime(Task * const task);
		const uint64_t & current_time;
	public:
		Servers servers;
		RAID_1(const std::string & name, Servers & p_servers, const uint64_t & t);
	       	virtual Task * const  Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_1();
};

class RAID_5: public RAID_0 {
	protected:
	       	// Servers::iterator parity;
		virtual uint64_t GetServiceTime(Task * const task);
	       	// void AdvanceParity();
	       	// bool AdvanceServer();
	public:
		RAID_5(const std::string & name, Servers & p_servers, const size_t & p_stripe_width, const uint64_t & t);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_5();
};

class RAID_4: public RAID_0 {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_parity_distr;
		virtual uint64_t GetServiceTime(Task * const task);
		const uint64_t & current_time;
	public:
		Servers parity_servers;
		const size_t stripe_width;
		RAID_4(const std::string & name, Servers & data_servers, Servers & p_parity_servers, const size_t & p_stripe_width, const uint64_t & t);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
	       	size_t StripeSize() const;
	       	virtual ~RAID_4();
};

class RAID_DP: public RAID_4 {
	public:
		RAID_DP(const std::string & name, Servers & data_servers, Servers & parity_servers, const size_t & p_stripe_width, const uint64_t & t);
	       	virtual ~RAID_DP();
};

#endif // SERVER_H
