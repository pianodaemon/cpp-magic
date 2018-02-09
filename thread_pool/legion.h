#ifndef __LEGION_THREAD_POOL_H__
#define __LEGION_THREAD_POOL_H__
 
#include <functional>
#include <stdexcept>
 
using task_delegate = std::function<void(void*)>;
 
class thread_pool_exception : public std::runtime_error
{
    public:
        thread_pool_exception::thread_pool_exception( std::string explanation ) : runtime_error( explanation ) {}
};
     
extern void
tp_create( struct thread_pool** pool_ptr , const size_t& workers_number , const size_t& queue_size );
 
extern void
tp_destroy( struct thread_pool* pool_ptr );
 
extern void
tp_add_task( struct thread_pool* const pool_ptr, task_delegate td, void* argument );
 
extern const unsigned int
tp_get_queue_size( struct thread_pool* const pool_ptr );
 
extern const unsigned int
tp_get_pending_tasks( struct thread_pool* const pool_ptr );
 
extern const unsigned int
tp_get_started_threads( struct thread_pool* const pool_ptr );
 
extern const unsigned int
tp_get_thread_count( struct thread_pool* const pool_ptr );
 
#endif /* __LEGION_THREAD_POOL_H__ */
