#ifndef WSJCPP_EMPLOY_ASYNC_JOBS_H
#define WSJCPP_EMPLOY_ASYNC_JOBS_H

#include <wsjcpp_employees.h>
#include <wsjcpp_core.h>
#include <model_leak.h>

// ---------------------------------------------------------------------

class WSJCppAsyncJob {
    public:
        WSJCppAsyncJob(const std::string &sName);
        const std::string &name();
        virtual bool run(const std::string &sWorkerId) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class WSJCppJobAsyncDeque {
    public:
        WSJCppAsyncJob *pop();
        void push(WSJCppAsyncJob *pJobAsync);
        void cleanup();
        bool isEmpty();

    private:
        std::string TAG;

        std::mutex m_mtxJobsAsyncDeque;
        std::deque<WSJCppAsyncJob *> m_dequeJobsAsync;
};

// ---------------------------------------------------------------------

class WSJCppAsyncJobsThreadWorker {
    public:

        WSJCppAsyncJobsThreadWorker(const std::string &sName, WSJCppJobAsyncDeque *pDeque);

        void start();
        void stop();
        bool isBuzy();
        void run();    
    private:
        std::string TAG;
        std::string m_sName;
        WSJCppJobAsyncDeque *m_pDeque;
        bool m_bBuzy;
        bool m_bStop;
        pthread_t m_threadWorker;
};

// ---------------------------------------------------------------------

class WSJCppEmployAsyncJobs : public WsjcppEmployBase {
    public:
        WSJCppEmployAsyncJobs();
        static std::string name() { return "WSJCppEmployAsyncJobs"; }
        virtual bool init();
        virtual bool deinit();
        void runOnceAsyncJob(WSJCppAsyncJob *pAsyncJob);
        void runOnceAsyncJobAfter(WSJCppAsyncJob *pAsyncJob, int nTime);
        void addToScheduleAsyncJobIfNotExist(WSJCppAsyncJob *pAsyncJob); // WSJCppAsyncSchedule
        void waitForDone();
    private:
        std::string TAG;

        WSJCppJobAsyncDeque *m_pJobsFastPool;
        WSJCppJobAsyncDeque *m_pJobsLongPool; // TODO
        WSJCppJobAsyncDeque *m_pJobsDelay; // TODO
        WSJCppJobAsyncDeque *m_pJobsCron; // TODO

        std::vector<WSJCppAsyncJobsThreadWorker *> *g_vJobsFastWorkers;
        int m_nMaxJobFastWorker;
};

#endif // WSJCPP_EMPLOY_ASYNC_JOBS_H
