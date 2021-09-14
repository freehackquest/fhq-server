#include <wsjcpp_unit_tests.h>
#include <vector>
#include <core/jobs_pool.h>

class UnitTestJobsPool : public WsjcppUnitTestBase {
    public:
        UnitTestJobsPool();
        virtual bool doBeforeTest();
        virtual void executeTest();
        virtual bool doAfterTest();
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestJobsPool)

UnitTestJobsPool::UnitTestJobsPool()
    : WsjcppUnitTestBase("UnitTestJobsPool") {
    //
}

// ----------------------------------------------------------------------

bool UnitTestJobsPool::doBeforeTest() {
    // nothing
    return true;
}

// ----------------------------------------------------------------------

class JobWaiterResult {
    public:
        JobWaiterResult() {
            m_nFinished = 0;
        }
        
        void onDone() {
            m_nFinished++;
        };

        void onFail(const std::string &sError) {
            WsjcppLog::err("JobWaiterResult", "Failed job");
        };

        int finishedJobs() {
            return m_nFinished;
        };
    private:
        int m_nFinished;
};

// ----------------------------------------------------------------------

class JobAsyncWaiter : public JobAsync {
    public:
        JobAsyncWaiter(int n, int ms, JobWaiterResult* pJobWaiterResult) : JobAsync("job-example") { 
            m_nNumber = n;
            m_nMilliseconds = ms;
            m_pJobWaiterResult = pJobWaiterResult;
        };
        virtual bool run(const std::string &sWorkerId) {
            WsjcppLog::info(sWorkerId, "begin job " + std::to_string(m_nNumber));
            std::this_thread::sleep_for(std::chrono::milliseconds(m_nMilliseconds));
            WsjcppLog::info(sWorkerId, "end job  " + std::to_string(m_nNumber));
            m_pJobWaiterResult->onDone();
            return true;
        }

    private:
        int m_nMilliseconds;
        JobWaiterResult* m_pJobWaiterResult;
        int m_nNumber;
};

// ----------------------------------------------------------------------

void UnitTestJobsPool::executeTest() {
    JobsPool::start();

    // TEST waitForDone
    int nCountJobs = 5;
    WsjcppLog::info(TAG, "Check waitForDone...");
    JobWaiterResult *pJobWaiterResult = new JobWaiterResult();
    for (int i = 0; i < nCountJobs; i++) {
        JobsPool::addJobFast(new JobAsyncWaiter(i, 500, pJobWaiterResult));
    }

    JobsPool::waitForDone();
    compare("Test waitForDone", pJobWaiterResult->finishedJobs(), nCountJobs);
}

// ----------------------------------------------------------------------

bool UnitTestJobsPool::doAfterTest() {
    // nothing
    return true;
}