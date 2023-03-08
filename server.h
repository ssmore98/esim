#ifndef SERVER_H
#define SERVER_H

// https://confluence.ngage.netapp.com/display/PALM/Discrete+Event+Simulator#DiscreteEventSimulator-Server

#include <cstdint>
#include <random>
#include <map>
#include <set>

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
		Server(const std::string & p_name);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t) = 0;
		virtual size_t StripeSize() const = 0;
	       	virtual ~Server() = 0;
		virtual void print(std::ostream & o, const uint64_t & current_time);
		const uint64_t & N_TASKS() const;
		const uint64_t & SZ_SUM() const;
		const uint64_t & TASK_TIME() const;
		const uint64_t & SVC_SUM() const;
		const uint64_t & QD_SUM() const;
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
		Shelf * const SHELF() const;
};

typedef std::set<Drive *> Drives;

class IOModule: public Server {
	protected:
		virtual uint64_t GetServiceTime(Task * const task);
		Shelf *shelf;
	public:
		IOModule(const std::string & name);
	       	virtual ~IOModule();
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual void print(std::ostream & o, const uint64_t & current_time);
		IOModule & operator=(Shelf * const p_self);
	       	Task * const Queue(Events & events, const uint64_t & t, Task * const task, Drive * const drive);
};

class IOModules: public std::set<IOModule *> {
	public:
	       	void print(std::ostream & o, const uint64_t & current_time) const;
};

class Shelf {
	protected:
		Drives drives;
		IOModules ioms;
	public:
		const std::string name;
		Shelf(const std::string & name);
	       	virtual ~Shelf();
		Shelf & operator=(Drive * const drive);
		Shelf & operator=(IOModule * const iom);
	       	void print(std::ostream & o, const uint64_t & current_time);
	       	Task * const Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom,
				Drive * const drive);
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
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

class RAID: public Server {
	public:
		RAID(const std::string & name);
	       	Task * const Queue(Events & events, const uint64_t & t, Task * const task,
			       	IOModule * const iom, Drive * const drive) const;
};

typedef std::set<RAID *> RAIDs;

class RAID_0: public RAID {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
		virtual uint64_t GetServiceTime(Task * const task);
		Drives drives;
	public:
		const size_t stripe_width;
		const std::string raid_level;
		RAID_0(const std::string & name, Drives & p_drives, const size_t & p_stripe_width, const std::string & p_raid_level="0");
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_0();
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

class RAID_1: public RAID_0 {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
		virtual uint64_t GetServiceTime(Task * const task);
	public:
		RAID_1(const std::string & name, Drives & p_drives);
	       	virtual Task * const  Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_1();
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

class RAID_5: public RAID_0 {
	protected:
		virtual uint64_t GetServiceTime(Task * const task);
	public:
		RAID_5(const std::string & name, Drives & p_drives, const size_t & p_stripe_width);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
		virtual size_t StripeSize() const;
	       	virtual ~RAID_5();
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

class RAID_4: public RAID_0 {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_parity_distr;
		virtual uint64_t GetServiceTime(Task * const task);
	public:
		Drives parity_drives;
		const size_t stripe_width;
		RAID_4(const std::string & name, Drives & data_drives, Drives & p_parity_drives, const size_t & p_stripe_width,
			       	const std::string & p_raid_level="4");
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom);
	       	virtual void UnQueue(Events & events, const uint64_t & t);
		virtual ServerEvent * const ScheduleTaskEnd(Task * const task, const uint64_t & t);
	       	size_t StripeSize() const;
	       	virtual ~RAID_4();
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

typedef std::map<MasterTask * const, Tasks> ConsistencyPoints;
typedef ConsistencyPoints::value_type ConsistencyPoint;

std::ostream & operator<<(std::ostream & o, const ConsistencyPoint & cp);
std::ostream & operator<<(std::ostream & o, const ConsistencyPoints & cps);

class RAID_DP: public RAID_4 {
	private:
		Tasks write_tasks;
		ConsistencyPoints cps;
		size_t GetAccWriteSize() const;
	public:
		RAID_DP(const std::string & name, Drives & data_drives, Drives & parity_drives,
			       	const size_t & p_stripe_width);
	       	virtual Task * const Queue(Events & events, const uint64_t & t, Task * const task, IOModule * const iom);
	       	virtual void EndTask(Task * const task, const uint64_t & t);
	       	virtual ~RAID_DP();
		virtual void print(std::ostream & o, const uint64_t & current_time);
};

#endif // SERVER_H
