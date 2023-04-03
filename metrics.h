#ifndef METRICS_H
#define METRICS_H

#include <cinttypes>

class IOPS;
class Time;

class IOS {
	protected:
		uint64_t count;
	public:
		IOS();
	       	IOS(const uint64_t & n);
	       	IOS(const IOS & n);
		IOS operator+(const IOS & n) const;
		// IOS & operator+=(const IOS & n);
		IOS operator++(int dummy);
		// IOS & operator=(const IOS & n);
		IOPS operator/(const Time & t) const;
		operator bool() const;
		operator uint64_t() const;
	       	friend std::ostream & operator<<(std::ostream & o, const IOS & b);
};

class QueueDepth {
	protected:
		uint64_t count;
	public:
		QueueDepth();
	       	QueueDepth(const size_t & n);
	       	QueueDepth(const QueueDepth & n);
		QueueDepth operator+(const QueueDepth & n) const;
		QueueDepth & operator+=(const QueueDepth & n);
		// QueueDepth & operator=(const QueueDepth & n);
		QueueDepth operator/(const IOS & t) const;
	       	friend std::ostream & operator<<(std::ostream & o, const QueueDepth & b);
		bool operator<(const QueueDepth & n) const;
		QueueDepth operator++(int dummy);
};


class Time {
	protected:
		uint64_t t;
	public:
		Time();
	       	Time(const uint64_t & n); // n in microseconds
	       	Time(const Time & n);
	       	// Time(const double & n); // n in seconds
		Time operator+(const Time & n) const;
		Time operator-(const Time & n) const;
		Time operator/(const IOS & n) const;
		Time & operator+=(const Time & n);
		Time & operator=(const Time & n);
		bool operator<(const Time & n) const;
		bool operator<=(const Time & n) const;
		bool operator>(const Time & n) const;
		operator bool() const;
		operator uint64_t() const;
		operator double() const;
	       	friend std::ostream & operator<<(std::ostream & o, const Time & b);
};

class IOPS {
	protected:
		uint64_t count;
	public:
		IOPS();
	       	IOPS(const uint64_t & n);
	       	IOPS(const IOPS & n);
		// IOPS operator+(const IOPS & n) const;
		// IOPS & operator+=(const IOPS & n);
		// IOPS & operator=(const IOPS & n);
	       	friend std::ostream & operator<<(std::ostream & o, const IOPS & b);
};

class DataRate;

class Bytes {
	protected:
		uint64_t bytes;
	public:
		Bytes();
		Bytes(const uint64_t & n);
		Bytes operator*(const size_t & n) const;
		Bytes operator+(const Bytes & n) const;
		Bytes & operator+=(const Bytes & n);
		DataRate operator/(const Time & t) const;
		Time operator/(const DataRate & t) const;
		QueueDepth operator/(const IOS & ios) const;
		bool operator<=(const Bytes & t) const;
	       	friend std::ostream & operator<<(std::ostream & o, const Bytes & b);
};

class DataRate { // in bytes per second
	protected:
		uint64_t rate;
	public:
		DataRate();
		DataRate(const uint64_t & n);
	       	friend std::ostream & operator<<(std::ostream & o, const DataRate & d);
		operator uint64_t() const { return rate; }
};

class Metrics {
	protected:
	       	IOS        n_tasks;
		Time       task_time;
		QueueDepth qd_sum;
		Time       qwt_sum;
		Time       svc_sum;
		Bytes      sz_sum;
	public:
		Metrics();
		const IOS & N_TASKS() const;
		const Bytes & SZ_SUM() const;
		const Time & TASK_TIME() const;
		const Time & SVC_SUM() const;
		const Time & QWT_SUM() const;
		const QueueDepth & QD_SUM() const;
	       	void QueueTask(const QueueDepth & qd, const Bytes & iosize);
	       	void StartTask(const Time & qwt, const Time & svc);
		void EndTask(const Time & response_time);
		friend std::ostream & operator<<(std::ostream & o, const Metrics & m);
	       	void print(std::ostream & o) const;
	       	void print(std::ostream & o, const Time & current_time) const;
};

#endif // METRICS_H
