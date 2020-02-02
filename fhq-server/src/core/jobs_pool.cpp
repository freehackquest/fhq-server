#include "jobs_pool.h"
#include <core/fallen.h>

// ---------------------------------------------------------------------

JobAsync::JobAsync(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &JobAsync::name() {
    return m_sName;
}

// ---------------------------------------------------------------------

JobAsync *JobAsyncDeque::pop() {
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

void JobAsyncDeque::push(JobAsync *pJobAsync) {
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    if (m_dequeJobsAsync.size() > 20) {
        WSJCppLog::warn(TAG, " deque more than " + std::to_string(m_dequeJobsAsync.size()));
    }
    m_dequeJobsAsync.push_front(pJobAsync);
}

// ----------------------------------------------------------------------

void JobAsyncDeque::cleanup() {
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    while (m_dequeJobsAsync.size() > 0) {
        delete m_dequeJobsAsync.back();
        m_dequeJobsAsync.pop_back();
    }
}

// ----------------------------------------------------------------------

bool JobAsyncDeque::isEmpty() {
    std::lock_guard<std::mutex> guard(this->m_mtxJobsAsyncDeque);
    bool bRet = m_dequeJobsAsync.size() == 0;
    return bRet;
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
    m_bBuzy = false;
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

bool JobsThreadWorker::isBuzy() {
    return m_bBuzy;
}

// ----------------------------------------------------------------------

void JobsThreadWorker::run() {
    while (1) {
        if (m_bStop) {
            m_bBuzy = false;
            return;
        }
        JobAsync *pJobAsync = m_pDeque->pop();
        while (pJobAsync != nullptr) {
            m_bBuzy = true;
            pJobAsync->run(TAG);
            delete pJobAsync;
            pJobAsync = m_pDeque->pop();
            if (m_bStop) {
                m_bBuzy = false;
                return;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (m_bStop) {
            m_bBuzy = false;
            return;
        }
        m_bBuzy = false;
    }
}

// ----------------------------------------------------------------------

JobAsyncDeque *g_pJobsFastPool = nullptr;
std::vector<JobsThreadWorker *> *g_vJobsFastWorkers = nullptr;
int g_nMaxJobFastWorker = 2;

void JobsPool::initGlobalVariables() {
    if (g_pJobsFastPool == nullptr) {
        g_pJobsFastPool = new JobAsyncDeque();
    }
    if (g_vJobsFastWorkers == nullptr) {
        g_vJobsFastWorkers = new std::vector<JobsThreadWorker *>();
        // 2 threads default
        for (int i = 0; i < g_nMaxJobFastWorker; i++) {
            g_vJobsFastWorkers->push_back(new JobsThreadWorker("fast-worker" + std::to_string(i), g_pJobsFastPool));
        }
    }
}

// ----------------------------------------------------------------------

 // Two queue for fast jobs 
void JobsPool::addJobFast(JobAsync *pJobAsync) {
    JobsPool::initGlobalVariables();
    g_pJobsFastPool->push(pJobAsync);
}

// ----------------------------------------------------------------------

// Long time jobs like a send a lot of emails and etc...
// better keep this list of jobs to storage
void JobsPool::addJobSlow(JobAsync *pJobAsync) {
    JobsPool::initGlobalVariables();
    WSJCppLog::warn("JobsPool", "addJobSlow not implemented yet");
    // g_pJobsLong->push(pJobAsync);
}

// ----------------------------------------------------------------------

// jobs with delay
void JobsPool::addJobDelay(int nMilliseconds, JobAsync *pJobAsync) {
    JobsPool::initGlobalVariables();
    WSJCppLog::warn("JobsPool", "addJobDelay not implemented yet");
    // g_pJobsShort->push(pJobAsync);
}

// ----------------------------------------------------------------------

// jobs by cron, for example every 5 minutes execute some job
void JobsPool::addJobCron(JobSchedule *pJobSchedule, JobAsync *pJobAsync) {
    JobsPool::initGlobalVariables();
    WSJCppLog::warn("JobsPool", "addJobCron not implemented yet");
    // g_pJobsShort->push(pJobAsync);
}

// ----------------------------------------------------------------------

void JobsPool::stop() {
    JobsPool::initGlobalVariables();
    for (int i = 0; i < g_vJobsFastWorkers->size(); i++) {
        g_vJobsFastWorkers->at(i)->stop();
    }
}

// ----------------------------------------------------------------------

void JobsPool::start() {
    JobsPool::initGlobalVariables();
    for (int i = 0; i < g_vJobsFastWorkers->size(); i++) {
        g_vJobsFastWorkers->at(i)->start();
    }
    // TODO slow
    // TODO thread for cron and delay
}

// ----------------------------------------------------------------------

void JobsPool::waitForDone() {
    // TODO when job in progress need wait deque for progress jobs ?
    JobsPool::initGlobalVariables();
    bool bBuzy = true;
    while (bBuzy) {
        bBuzy = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!g_pJobsFastPool->isEmpty()) {
            bBuzy = true;
        }
        for (int i = 0; i < g_vJobsFastWorkers->size(); i++) {
            if (g_vJobsFastWorkers->at(i)->isBuzy()) {
                bBuzy = true;
            }
        }
    }
}

// ----------------------------------------------------------------------

void JobsPool::cleanup() {
    g_pJobsFastPool->cleanup();
    // TODO slow
    // TODO thread for cron and delay
}

// ----------------------------------------------------------------------
