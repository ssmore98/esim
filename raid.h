#ifndef RAID_H
#define RAID_H

#include <string>
#include <iostream>
#include <map>

#include "task.h"
#include "event.h"
#include "event.h"
#include "server.h"
#include "metrics.h"

typedef std::map<Drive *, Tasks> TaskList;

class RAID {
	protected:
	       	static std::default_random_engine generator;
		Tasks tasks;
		Metrics metrics;
	public:
		const std::string name;
		RAID(const std::string & name);
		virtual ~RAID();
	       	virtual TaskList Execute(Task * const task) = 0;
	       	virtual Tasks Finish(const Time & current_time, Task * const task) = 0;
		virtual Bytes StripeSize() const = 0;
		virtual void print(std::ostream & o, const Time & current_time) const = 0;
};

typedef std::set<RAID *> RAIDs;

class RAID_0: public RAID {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
		Drives drives;
	public:
		const Bytes stripe_width;
		const std::string raid_level;
		RAID_0(const std::string & name, Drives & p_drives, const Bytes & p_stripe_width, const std::string & p_raid_level="0");
	       	virtual ~RAID_0();
	       	virtual TaskList Execute(Task * const task);
	       	virtual Tasks Finish(const Time & current_time, Task * const task);
		virtual Bytes StripeSize() const;
		virtual void print(std::ostream & o, const Time & current_time) const;
};

class RAID_1: public RAID_0 {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_server_distr;
	public:
		RAID_1(const std::string & name, Drives & p_drives);
		virtual Bytes StripeSize() const;
	       	virtual ~RAID_1();
		virtual void print(std::ostream & o, const Time & current_time) const;
	       	virtual TaskList Execute(Task * const task);
	       	virtual Tasks Finish(const Time & current_time, Task * const task);
};

class RAID_5: public RAID_0 {
	public:
		RAID_5(const std::string & name, Drives & p_drives, const Bytes & p_stripe_width);
		virtual Bytes StripeSize() const;
	       	virtual ~RAID_5();
		virtual void print(std::ostream & o, const Time & current_time) const;
	       	virtual TaskList Execute(Task * const task);
	       	virtual Tasks Finish(const Time & current_time, Task * const task);
};

class RAID_4: public RAID_0 {
	protected:
	       	std::uniform_int_distribution<uint16_t> select_parity_distr;
	public:
		Drives parity_drives;
		RAID_4(const std::string & name, Drives & data_drives, Drives & p_parity_drives, const Bytes & stripe_width,
			       	const std::string & p_raid_level="4");
	       	Bytes StripeSize() const;
	       	virtual ~RAID_4();
		virtual void print(std::ostream & o, const Time & current_time);
	       	virtual TaskList Execute(Task * const task);
	       	virtual Tasks Finish(const Time & current_time, Task * const task);
};

typedef std::map<Task * const, Tasks> ConsistencyPoints;
typedef ConsistencyPoints::value_type ConsistencyPoint;

std::ostream & operator<<(std::ostream & o, const ConsistencyPoint & cp);
std::ostream & operator<<(std::ostream & o, const ConsistencyPoints & cps);

class RAID_DP: public RAID_4 {
	private:
		Tasks write_tasks;
		ConsistencyPoints cps;
		Bytes GetAccWriteSize() const;
	public:
		RAID_DP(const std::string & name, Drives & data_drives, Drives & parity_drives,
			       	const Bytes & p_stripe_width);
	       	virtual ~RAID_DP();
		virtual void print(std::ostream & o, const Time & current_time);
	       	virtual TaskList Execute(Task * const task);
	       	virtual Tasks Finish(const Time & current_time, Task * const task);
};

#endif // RAID_H
