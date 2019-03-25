#include "unit_test_jobs_pool.h"
#include <vector>
#include <md5.h>
#include <str.h>
#include <jobs_pool.h>


REGISTRY_UNIT_TEST(UnitTestJobsPool)

UnitTestJobsPool::UnitTestJobsPool()
    : UnitTestBase("UnitTestJobsPool") {
    //
}

void UnitTestJobsPool::init() {
    // nothing
}

class JobAsyncWaiter : public JobAsync {
    public:
        JobAsyncWaiter(int ms) : JobAsync("job-example") { 
            m_nMilliseconds = ms;
            m_bFinished = false;
        };
        virtual bool run(const std::string &sWorkerId) {
            Log::info(sWorkerId, "begin");
            std::this_thread::sleep_for(std::chrono::milliseconds(m_nMilliseconds));
            Log::info(sWorkerId, "end");
            m_bFinished = true;
        }

        bool isFinished() {
            return m_bFinished;
        }

    private:
        int m_nMilliseconds;
        bool m_bFinished;
};

bool UnitTestJobsPool::run() {
    JobsPool::start();

    // TEST waitForDone
    Log::info(TAG, "Check waitForDone...");
    JobAsyncWaiter *pJobAsyncWaiter_1000 = new JobAsyncWaiter(1000);
    JobsPool::addJobFast(pJobAsyncWaiter_1000);
    JobsPool::waitForDone();
    if (!pJobAsyncWaiter_1000->isFinished()) {
        Log::err(TAG, "Test waitForDone FAILED");
        return false;
    }

    return true;
}

