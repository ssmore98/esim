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

#endif // METRICS_CPP
