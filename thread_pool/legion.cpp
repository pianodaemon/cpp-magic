#include "legion.h"
 
#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <map>
#include <string>
 
#define __MAX_THREADS  64
#define __MAX_QUEUE    65536
 
#define __EXECUTE_TASK(K) K.first(K.second)
 
enum pool_errors {
    THREAD_POOL_SUCCEED = 0,
    THREAD_POOL_SYSTEM_ERROR = -1,
    THREAD_POOL_NULL_PTR = -1000,
    THREAD_POOL_DELEGATE_NOT_FOUND = -1001,
    THREAD_POOL_QUEUE_FULL = -1002,
    THREAD_POOL_IS_SHUTTING_DOWN = -1003,
    THREAD_POOL_CAN_NOT_USE_SUCH_PARAMS = -1004
};
 
using task_config = std::pair<task_delegate, void*>;
 
struct thread_pool
{
    std::vector< std::thread > workers;
    std::queue< task_config > tasks_queue;
    size_t queue_limit;
    std::mutex cmutex;
    std::condition_variable ccv;
    bool stop;
    unsigned short started;
};
 
static std::map<enum pool_errors, std::string> error_msgs = {
    { THREAD_POOL_SYSTEM_ERROR, "A fatal error related with system limits has happened" },
    { THREAD_POOL_NULL_PTR, "The pointer that references the pool has been detected null" },
    { THREAD_POOL_DELEGATE_NOT_FOUND, "The pool's client side did not supply a task delegate" },
    { THREAD_POOL_QUEUE_FULL, "The task queue has reached its limit" },
    { THREAD_POOL_IS_SHUTTING_DOWN, "A shutdown action is not allowed again as long as thread pool is shuting down"},
    { THREAD_POOL_CAN_NOT_USE_SUCH_PARAMS , "Invalid params has been sumitted during thread pool creation" },
};
 
 
static std::string 
explain_err( int err_code )
{
    bool found = false;
    std::map<enum pool_errors, std::string > ::const_iterator it = error_msgs.find((enum pool_errors)err_code);
 
    if (it != error_msgs.end())
    {
        /* element was found !*/
        found = true;
    }
 
    return (found) ? error_msgs[(enum pool_errors)err_code] : "An unknow error has been detected";
}
 
static void
launch_workers( struct thread_pool* const pool_ptr , const size_t& workers_number )
{
    // Start worker threads
    for (unsigned int i = 0; i < workers_number; i++)
    {
        pool_ptr->workers.emplace_back([pool_ptr]()->void {
 
            task_config t;          
 
            while (true)
            {
                {
                    std::unique_lock<std::mutex> l(pool_ptr->cmutex);
 
                    // Wait on condition variable, check for spurious wakeups.
                    while ( pool_ptr->tasks_queue.empty() && ( !pool_ptr->stop ) ) pool_ptr->ccv.wait(l);
 
                    if ( pool_ptr->stop && pool_ptr->tasks_queue.empty() ) break;
 
                    t = std::move( pool_ptr->tasks_queue.front() );
                    pool_ptr->tasks_queue.pop();
                }
 
                __EXECUTE_TASK(t);
            }
 
            pool_ptr->started--;
        });
 
        pool_ptr->started++;
    }
}
 
extern const unsigned int
tp_get_pending_tasks( struct thread_pool* const pool_ptr )
{
    return (pool_ptr) ? pool_ptr->tasks_queue.size() : 0;
}
 
extern const unsigned int
tp_get_thread_count( struct thread_pool* const pool_ptr )
{
    return (pool_ptr) ? pool_ptr->workers.size() : 0;
}
 
extern const unsigned int
tp_get_started_threads( struct thread_pool* const pool_ptr )
{
    return (pool_ptr) ? pool_ptr->started : 0;
}
 
extern const unsigned int
tp_get_queue_size( struct thread_pool* const pool_ptr )
{
    return (pool_ptr) ? pool_ptr->queue_limit : 0;
}
 
extern void
tp_destroy( struct thread_pool* pool_ptr )
{
    int erc = (pool_ptr == NULL) ? THREAD_POOL_NULL_PTR : THREAD_POOL_SUCCEED;
 
    {
        std::unique_lock<std::mutex> lk( pool_ptr->cmutex );
         
        if ( !pool_ptr->stop ) pool_ptr->stop = true;
        else erc = THREAD_POOL_IS_SHUTTING_DOWN;
    }
 
    if ( erc < THREAD_POOL_SUCCEED ) throw thread_pool_exception( explain_err( erc ) );
 
    pool_ptr->ccv.notify_all();
 
    std::for_each( pool_ptr->workers.begin() , pool_ptr->workers.end() , std::mem_fn( &std::thread::join ) );
}
 
extern void
tp_create( struct thread_pool** new_pool_ptr, const size_t& workers_number, const size_t& queue_size )
{
    int erc = (new_pool_ptr == NULL) ? THREAD_POOL_NULL_PTR : THREAD_POOL_SUCCEED;
 
    do
    {
        if (erc < THREAD_POOL_SUCCEED) break;
 
        erc = ( workers_number <= 0 || workers_number > __MAX_THREADS || queue_size <= 0 || queue_size > __MAX_QUEUE ) ? THREAD_POOL_CAN_NOT_USE_SUCH_PARAMS : THREAD_POOL_SUCCEED;
 
        if (erc < THREAD_POOL_SUCCEED) break;
 
        if ((*new_pool_ptr = new thread_pool) == NULL) erc = THREAD_POOL_SYSTEM_ERROR;
 
        if (erc < THREAD_POOL_SUCCEED) break;
 
        (*new_pool_ptr)->queue_limit = queue_size;
        (*new_pool_ptr)->started = 0;
        (*new_pool_ptr)->stop = false;
 
    } while (false);
 
    if ( erc < THREAD_POOL_SUCCEED ) throw thread_pool_exception( explain_err( erc ) );
    else launch_workers(*new_pool_ptr, workers_number);
}
 
extern void
tp_add_task( struct thread_pool* const pool_ptr, task_delegate td, void* argument )
{
    int erc = ( pool_ptr == NULL ) ? THREAD_POOL_NULL_PTR : THREAD_POOL_SUCCEED;
 
    if ( erc == THREAD_POOL_SUCCEED )
    {
        if ( !td )
        {
            erc = THREAD_POOL_DELEGATE_NOT_FOUND;
        }
        else
        {
            {
                std::unique_lock<std::mutex> lk( pool_ptr->cmutex );
 
                if ( pool_ptr->tasks_queue.size() == pool_ptr->queue_limit ) erc = THREAD_POOL_QUEUE_FULL;
                else if ( pool_ptr->stop ) erc = THREAD_POOL_IS_SHUTTING_DOWN;
                else pool_ptr->tasks_queue.push( { td, argument } );
            }
 
            pool_ptr->ccv.notify_one();
        }
    }
 
    if ( erc < THREAD_POOL_SUCCEED ) throw thread_pool_exception( explain_err( erc ) );
}
