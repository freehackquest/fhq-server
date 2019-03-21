#include "jobs_pool.h"
#include <logger.h>

// ---------------------------------------------------------------------

JobAsync::JobAsync(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &JobAsync::name() {
    return m_sName;
}

// ---------------------------------------------------------------------

void JobPromise::reject(const std::string &sError) {
    // TODO logging
    onFail(sError);
}

// ---------------------------------------------------------------------

void JobPromise::resolve() {
    onDone();
}

// ---------------------------------------------------------------------

JobAsync *JobAsyncDeque::pop(){
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    JobAsync *pJobAsync = nullptr;
    int nSize = m_dequeJobsAsync.size();
    if (nSize > 0) {
        pJobAsync = m_dequeJobsAsync.back();
        m_dequeJobsAsync.pop_back();
    }
    return pJobAsync;
}

// ----------------------------------------------------------------------

void JobAsyncDeque::push(JobAsync *pJobAsync){
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    if (m_dequeJobsAsync.size() > 20) {
        Log::warn(TAG, " deque more than " + std::to_string(m_dequeJobsAsync.size()));
    }
    m_dequeJobsAsync.push_front(pJobAsync);
}

// ----------------------------------------------------------------------

void JobAsyncDeque::cleanup(){
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    while (m_dequeJobsAsync.size() > 0) {
        delete m_dequeJobsAsync.back();
        m_dequeJobsAsync.pop_back();
    }
}

// ----------------------------------------------------------------------

void* processJobsThreadWorker(void *arg) {
    JobsThreadWorker *pWorker = (JobsThreadWorker *)arg;
    pthread_detach(pthread_self());
    pWorker->run();
    return 0;
}

// ----------------------------------------------------------------------

JobsThreadWorker::JobsThreadWorker(const std::string &sName, JobAsyncDeque *pDeque) {
    TAG = "JobsThreadWorker-" + sName;
    m_pDeque = pDeque;
    m_bStop = false;
    m_sName = sName;
}

// ----------------------------------------------------------------------

void JobsThreadWorker::start() {
    m_bStop = false;
    pthread_create(&m_threadWorker, NULL, &processJobsThreadWorker, (void *)this);
}

// ----------------------------------------------------------------------

void JobsThreadWorker::stop() {
    m_bStop = true;
}

// ----------------------------------------------------------------------

void JobsThreadWorker::run() {
    const int nMaxPackageSize = 4096;
    while(1) {
        if (m_bStop) return;
        JobAsync *pRunnable = m_pDeque->pop();
        bool bExists = pRunnable != nullptr;
        // TODO refactor
        if (bExists) {
            pRunnable->run(TAG);
            delete pRunnable;
        }

        if (!bExists) {
            if (m_bStop) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (m_bStop) return;
        }
    }
}