/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#pragma once

#include <deque>
#include <mutex>
#include <string>
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

class JobSchedule {
public:
  // TODO
};

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

extern JobAsyncDeque *g_pJobsFastPool;
extern std::vector<JobsThreadWorker *> *g_vJobsFastWorkers;
extern int g_nMaxJobFastWorker;

extern JobAsyncDeque *g_pJobsLongPool; // TODO
extern JobAsyncDeque *g_pJobsDelay;    // TODO
extern JobAsyncDeque *g_pJobsCron;     // TODO
// TODO control thread will be add delay and cron jobs to long

// TODO statistics
// TODO max thread different workers

class JobsPool {
public:
  static void initGlobalVariables();

  static void addJobSlow(JobAsync *pJobAsync);
  static void addJobFast(JobAsync *pJobAsync);
  static void addJobDelay(int nMilliseconds, JobAsync *pJobAsync);
  static void addJobCron(JobSchedule *pJobSchedule, JobAsync *pJobAsync);

  static void stop();
  static void start();
  static void waitForDone();
  static void cleanup();
};
