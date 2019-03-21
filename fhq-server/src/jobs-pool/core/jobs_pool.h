#ifndef JOBS_POOL_H
#define JOBS_POOL_H

#include <string>
#include <mutex>
#include <deque>
#include <thread>

class JobAsync {
    public:
        JobAsync(const std::string &sName);
        const std::string &name();
        virtual bool run(const std::string &sWorkerId) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class JobPromise {
    public:
        void resolve();
        void reject(const std::string &sError);
    protected:
        virtual void onDone() = 0;
        virtual void onFail(const std::string &sError) = 0;
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
        void run();    
    private:
        std::string TAG;
        std::string m_sName;
        JobAsyncDeque *m_pDeque;
        bool m_bStop;
        pthread_t m_threadWorker;
};

// ---------------------------------------------------------------------

extern JobAsyncDeque *g_pJobsShort;
extern JobAsyncDeque *g_pJobsLong;
extern JobAsyncDeque *g_pJobsDelay;
extern JobAsyncDeque *g_pJobsCron;
// TODO statistics
// TODO max thread different workers

/*
long task - длинный по времени работы таск (отдельные треды или даже один отдельный тред)
short task - в обычном режиме короткие по времени обработки таски без лишних ожиданий.
delayed task - отложенный таск 
cron task - запускается с определнной переодичностью, по расписанию по сути.
*/

class JobsPool {
    public:
        static void initGlobalVariables();

        static void addJobLong( // доллгие работы если их нет то выполняем короткие
            JobAsync *pJobAsync,
            JobPromise *pJobPromise = nullptr
        );
        static void addJobShort( // две очереди для быстрых работ (сразу собирать статистику)
            JobAsync *pJobAsync,
            JobPromise *pJobPromise = nullptr
        );
        static void addJobDelay( // здесь создается отдельная long job
            int nMilliseconds,
            JobAsync *pJobAsync,
            JobPromise *pJobPromise = nullptr
        );
        static void addJobCron( // отдельный тред для cron
            JobSchedule *pJobSchedule,
            JobAsync *pJobAsync,
            JobPromise *pJobPromise = nullptr
        );

        static void stopAll();
};

// ---------------------------------------------------------------------

#endif // JOBS_POOL_H