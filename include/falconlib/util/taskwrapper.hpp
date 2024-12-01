#pragma once
#include "pros/rtos.hpp"

namespace falconlib {

    /*
     * The TaskWrapper class abstracts away the complicated syntax for declaring and
     * managing tasks, simplifying the implementation of asynchronous functions. 
     * Other classes need only inherit from TaskWrapper and immediately have a
     * fully-functional task manager that can be overwritten as necessary using
     * virtual functions
     */
    class TaskWrapper {
    protected:

        TaskWrapper() = default;

        TaskWrapper(const TaskWrapper& iTask) = delete;

        TaskWrapper(TaskWrapper&& iTask) = default;

        // Destructor. Ensures task is removed upon deallocation
        virtual ~TaskWrapper();

        virtual void loop() = 0;

    public:

        // Calls function trampoline() and allocates task
        virtual void startTask(const char* iname = "task");

        // Resumes suspended task
        virtual void resumeTask();

        // Suspends running task
        virtual void pauseTask();

        // Deallocates task
        virtual void stopTask();

        //Trivial getter of task's name
        virtual const char* getName();

    private:

        // Calls virtual function loop()
        static void trampoline(void* iparam);

        std::unique_ptr<pros::Task> task {nullptr};
    };
}