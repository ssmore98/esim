#ifndef METRICS_CPP
#define METRICS_CPP

#include "metrics.h"

Metrics::Metrics(): n_tasks(0), task_time(0), qd_sum(0), svc_sum(0), sz_sum(0) {
}

const uint64_t & Metrics::TASK_TIME() const { return task_time; }
const uint64_t & Metrics::SVC_SUM() const { return svc_sum; }
const uint64_t & Metrics::QD_SUM() const { return qd_sum; }
const uint64_t & Metrics::N_TASKS() const { return n_tasks; }
const uint64_t & Metrics::SZ_SUM() const { return sz_sum; }

void Metrics::StartTask(const uint16_t & qd, const uint64_t & svc, const size_t & iosize) {
	n_tasks++;
	qd_sum += qd;
	svc_sum += svc;
	sz_sum += iosize;
}

void Metrics::EndTask(const uint64_t & xt) {
	task_time += xt;
}

std::ostream & operator<<(std::ostream & o, const Metrics & metrics) {
	o << "\tTotal I/Os "  << metrics.n_tasks << std::endl;
	o << "\tTotal Bytes " << metrics.sz_sum  << std::endl;
	if (metrics.n_tasks) {
	       	o << "\tavLatency "     << double(metrics.task_time) / double(metrics.n_tasks * 1000 * 1000) << std::endl;
	       	o << "\tavServiceTime " << double(metrics.svc_sum)   / double(metrics.n_tasks * 1000 * 1000) << std::endl;
	       	o << "\tavQueueDepth "  << double(metrics.qd_sum)    / double(metrics.n_tasks)               << std::endl;
	}
	return o;
}

#endif // METRICS_CPP
