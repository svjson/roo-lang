#ifndef ROO__RUNTIME__WORKER_H
#define ROO__RUNTIME__WORKER_H

#include <condition_variable>
#include <exception>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Roo
{
  class Worker
  {
   private:
    std::mutex mutex;
    std::condition_variable state_changed;
    std::thread execution_thread;
    std::exception_ptr startup_failure;
    bool ready = false;
    bool stopping = false;

    void run();

   public:
    Worker();
    ~Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
  };

  class WorkerRegistry
  {
   private:
    std::map<std::string, std::unique_ptr<Worker>> workers;

   public:
    void create(const std::string& identity);
  };
} // namespace Roo

#endif
