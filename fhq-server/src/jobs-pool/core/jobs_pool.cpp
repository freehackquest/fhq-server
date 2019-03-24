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

void JobsPool::addJobFast( // две очереди для быстрых работ (сразу собирать статистику)
    JobAsync *pJobAsync,
    JobPromise *pJobPromise
) {
    JobsPool::initGlobalVariables();
    g_pJobsFastPool->push(pJobAsync);
}

// ----------------------------------------------------------------------

void JobsPool::addJobSlow( // долгие работы если их нет то выполняем короткие
    JobAsync *pJobAsync,
    JobPromise *pJobPromise
) {
    JobsPool::initGlobalVariables();
    Log::warn("JobsPool", "addJobSlow not implemented yet");
    // g_pJobsLong->push(pJobAsync);
}

// ----------------------------------------------------------------------

void JobsPool::addJobDelay( // здесь создается отдельная long job
    int nMilliseconds,
    JobAsync *pJobAsync,
    JobPromise *pJobPromise
) {
    JobsPool::initGlobalVariables();
    Log::warn("JobsPool", "addJobDelay not implemented yet");
    // g_pJobsShort->push(pJobAsync);
}

// ----------------------------------------------------------------------

void JobsPool::addJobCron( // отдельный тред для cron
    JobSchedule *pJobSchedule,
    JobAsync *pJobAsync,
    JobPromise *pJobPromise
) {
    JobsPool::initGlobalVariables();
    Log::warn("JobsPool", "addJobCron not implemented yet");
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
}

// ----------------------------------------------------------------------

void JobsPool::cleanup() {
    g_pJobsFastPool->cleanup();
}

// ----------------------------------------------------------------------
