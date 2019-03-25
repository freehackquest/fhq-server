#ifndef JOBS_POOL_H
#define JOBS_POOL_H

#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include <vector>

class JobAsync {
    public:
        JobAsync(const std::string &sName);
        const std::string &name();
        virtual bool run(const std::string &sWorkerId) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class JobSchedule {
    public:
        // TODO
};

// ---------------------------------------------------------------------

class JobAsyncDeque {
    public:
        JobAsync *pop();
        void push(JobAsync *pJobAsync);
        void cleanup();
        bool isEmpty();

    private:
        std::string TAG;

        std::mutex m_mtxJobsAsyncDeque;
        std::deque<JobAsync *> m_dequeJobsAsync;
};

// ---------------------------------------------------------------------

class JobsThreadWorker {
    public:

        JobsThreadWorker(const std::string &sName, JobAsyncDeque *pDeque);

        void start();
        void stop();
        bool isBuzy();
        void run();    
    private:
        std::string TAG;
        std::string m_sName;
        JobAsyncDeque *m_pDeque;
        bool m_bBuzy;
        bool m_bStop;
        pthread_t m_threadWorker;
};

// ---------------------------------------------------------------------

extern JobAsyncDeque *g_pJobsFastPool;
extern std::vector<JobsThreadWorker *> *g_vJobsFastWorkers;
extern int g_nMaxJobFastWorker;

extern JobAsyncDeque *g_pJobsLongPool; // TODO
extern JobAsyncDeque *g_pJobsDelay; // TODO
extern JobAsyncDeque *g_pJobsCron; // TODO
// TODO control thread will be add delay and cron jobs to long

// TODO statistics
// TODO max thread different workers

class JobsPool {
    public:
        static void initGlobalVariables();

        static void addJobSlow(JobAsync *pJobAsync);
        static void addJobFast(JobAsync *pJobAsync);
        static void addJobDelay(int nMilliseconds, JobAsync *pJobAsync);
        static void addJobCron(JobSchedule *pJobSchedule,JobAsync *pJobAsync);

        static void stop();
        static void start();
        static void waitForDone();
        static void cleanup();
};

// ---------------------------------------------------------------------

#endif // JOBS_POOL_H
