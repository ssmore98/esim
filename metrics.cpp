#ifndef METRICS_CPP
#define METRICS_CPP

#include <iomanip>
#include <iostream>
#include <cassert>

#include "metrics.h"

Metrics::Metrics(): n_tasks(0), task_time(), qd_sum(0), svc_sum(), sz_sum(0) {
}

const Time & Metrics::TASK_TIME() const { return task_time; }
const Time & Metrics::SVC_SUM() const { return svc_sum; }
const QueueDepth & Metrics::QD_SUM() const { return qd_sum; }
const IOS & Metrics::N_TASKS() const { return n_tasks; }
const Bytes & Metrics::SZ_SUM() const { return sz_sum; }

void Metrics::QueueTask(const QueueDepth & qd, const Bytes & iosize) {
	n_tasks++;
	qd_sum += qd;
       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << svc_sum << std::endl;
	// svc_sum += svc;
       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << svc_sum << std::endl;
	sz_sum += iosize;
}

void Metrics::StartTask(const Time & qwt, const Time & svc) {
	// n_tasks++;
	// qd_sum += qd;
       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << svc_sum << std::endl;
	qwt_sum += qwt;
	svc_sum += svc;
       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << svc_sum << std::endl;
	// sz_sum += iosize;
}


void Metrics::EndTask(const Time & response_time) {
	task_time += response_time;
}

std::ostream & operator<<(std::ostream & o, const Metrics & metrics) {
	o << "\tTotal I/Os "  << metrics.n_tasks << std::endl;
	o << "\tTotal Bytes " << metrics.sz_sum  << std::endl;
	if (metrics.n_tasks) {
	       	o << "\tavLatency "     << metrics.task_time / metrics.n_tasks << std::endl;
	       	o << "\tavServiceTime " << metrics.svc_sum   / metrics.n_tasks << std::endl;
	       	o << "\tavQueueDepth "  << metrics.qd_sum    / metrics.n_tasks << std::endl;
	}
	return o;
}

void Metrics::print(std::ostream & o) const {
	o << std::setw(8) << std::left << n_tasks << '\t' << std::left << sz_sum;
	o << '\t' << std::setiosflags(std::ios::fixed) << std::setprecision(6) <<
	       	(n_tasks ? task_time / n_tasks: Time());
	o << '\t' << std::setiosflags(std::ios::fixed) << std::setprecision(6) <<
		(n_tasks ? svc_sum / n_tasks: Time());
	o << '\t' << std::setiosflags(std::ios::fixed) << std::setprecision(2) <<
		(n_tasks ? qd_sum  / n_tasks : QueueDepth());
	o << '\t' << std::setiosflags(std::ios::fixed) << std::setprecision(2) <<
		(n_tasks ? qwt_sum / n_tasks : Time());
	// o << '\t' << svc_sum << '\t' << n_tasks;
}

void Metrics::print(std::ostream & o, const Time & current_time) const {
	o << std::left << (current_time ? n_tasks / current_time : IOPS(0));
	o << '\t' << std::left << (current_time ? sz_sum / current_time : DataRate(0));
	o << "\t";
	print(o);
}

DataRate::DataRate(const uint64_t & n): rate(n) {
}

std::ostream & operator<<(std::ostream & o, const DataRate & d) {
	if (d.rate < 1024) {
	       	o << d.rate << " B/s";
	} else if (d.rate < 1024 * 1024) {
	       	o << (d.rate / 1024) << " KB/s";
	} else if (d.rate < 1024 * 1024 * 1024) {
	       	o << (d.rate / (1024 * 1024)) << " MB/s";
	} else {
	       	o << (d.rate / (1024 * 1024 * 1024)) << " GB/s";
	}
	return o;
}

DataRate Bytes::operator/(const Time & t) const {
	DataRate retval((bytes * 1000 * 1000) / uint64_t(t));
	return retval;
}

Time::operator bool() const {
	return (t != 0);
}

std::ostream & operator<<(std::ostream & o, const Bytes & b) {
	if (b.bytes < 1024) {
	       	o << b.bytes << "B";
	} else if (b.bytes < 1024 * 1024) {
	       	o << (b.bytes / 1024) << "KB";
	} else if (b.bytes < 1024 * 1024 * 1024) {
	       	o << (b.bytes / (1024 * 1024)) << "MB";
	} else {
	       	o << (b.bytes / (1024 * 1024 * 1024)) << "GB";
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, const Time & b) {
	o << b.t << "us";
	return o;
}

Time & Time::operator+=(const Time & n) {
	t += n.t;
	return *this;
}

Time::Time(): t(0) {
       	// std::cout << __FILE__ << ':' << __LINE__ << " Zero Constructor" << std::endl;
}

Time::Time(const uint64_t & n): t(n) { // n in microseconds
       	// std::cout << __FILE__ << ':' << __LINE__ << " uint64 Constructor " << t << std::endl;
       	// std::cout << __FILE__ << ':' << __LINE__ << " uint64 Constructor " << n << std::endl;
}

Time::Time(const Time & n): t(n.t) { // n in microseconds
       	// std::cout << __FILE__ << ':' << __LINE__ << " Time Constructor" << std::endl;
}

Time & Time::operator=(const Time & n) {
	t = n.t;
	return *this;
}

Bytes & Bytes::operator+=(const Bytes & n) {
	bytes += n.bytes;
	return *this;
}

Bytes::Bytes(const uint64_t & n): bytes(n) {
}

bool Bytes::operator<=(const Bytes & t) const {
	return bytes <= t.bytes;
}

Bytes Bytes::operator*(const size_t & n) const {
	return Bytes(this->bytes * n);
}

Time Time::operator+(Time const & tm) const {
	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << this->t << '+' << tm.t << '=' << this->t + tm.t << std::endl;
	return Time(t + tm.t);
}

Time Bytes::operator/(const DataRate & t) const {
	return Time(bytes / uint64_t(t));
}

Bytes Bytes::operator+(const Bytes & n) const {
	return (bytes + n.bytes);
}

QueueDepth::QueueDepth(const size_t & n): count(n) {
}

IOS::operator bool() const {
	return (0 < count);
}

std::ostream & operator<<(std::ostream & o, const IOPS & b) {
	o << b.count;
	return o;
}

IOPS::IOPS(const uint64_t & n): count(n) {
}

IOPS IOS::operator/(const Time & t) const {
	return IOPS((count * 1000 * 1000) / uint64_t(t));
}

Time Time::operator-(const Time & n) const {
	assert(t >= n.t);
	return Time(t - n.t);
}

Time::operator uint64_t() const {
	return t;
}

std::ostream & operator<<(std::ostream & o, const IOS & b) {
	o << b.count;
	return o;
}

std::ostream & operator<<(std::ostream & o, const QueueDepth & b) {
	o << b.count;
	return o;
}

QueueDepth QueueDepth::operator/(const IOS & t) const {
	return QueueDepth(count / uint64_t(t));
}

IOS::operator uint64_t() const {
	return count;
}

QueueDepth::QueueDepth(): count(0) {
}

Time Time::operator/(const IOS & n) const {
	return Time(t / uint64_t(n));
}

QueueDepth & QueueDepth::operator+=(const QueueDepth & n) {
	count += n.count;
	return *this;
}

IOS IOS::operator++(int dummy) {
	const uint64_t retval(count);
	count++;
	return IOS(retval);
}

IOS::IOS(const uint64_t & n) : count(n) {
}

QueueDepth::QueueDepth(const QueueDepth & n): count(n.count) {
}

IOS::IOS(const IOS & n): count(n.count) {
}

bool Time::operator>(const Time & n) const {
	return t > n.t;
}

bool Time::operator<(const Time & n) const {
	return t < n.t;
}

QueueDepth QueueDepth::operator+(const QueueDepth & n) const {
	return QueueDepth(count + n.count);
}

IOS IOS::operator+(const IOS & n) const {
	return IOS(count + n.count);
}

QueueDepth QueueDepth::operator++(int dummy) {
	const uint64_t retval(count);
	count++;
	return QueueDepth(retval);
}

bool QueueDepth::operator<(const QueueDepth & n) const {
	return (count < n.count);
}

Time::operator double() const {
	return double(t);
}

#endif // METRICS_CPP
