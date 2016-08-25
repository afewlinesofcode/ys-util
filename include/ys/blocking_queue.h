/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-06-29
 * \brief
 */

#ifndef YS_BLOCKING_QUEUE_H
#define YS_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace ys
{

/*!
 * Blocking queue class.
 */
template<class T>
class blocking_queue
{
public:
    /*!
     * Constructor.
     */
    blocking_queue();

    /*!
     * Destructor.
     * Interrupt queue if it is in waiting state.
     */
    ~blocking_queue();

    /*!
     * Add element to the end of the queue.
     * \param item Element to add.
     */
    void
    push(T const& item);

    /*!
     * Pop (in the case the queue is empty then wait and then pop) front
     * element from the queue.
     * \param item The result item.
     * \return true if item was fetched, false if fetch failed (interrupted, ...)
     */
    bool
    pop(T* item);

    /*!
     * Check whether the queue is empty.
     * \return
     */
    bool
    empty() const;

    /*!
     * Get the size of the queue.
     * \return
     */
    size_t
    size() const;

    /*!
     * Interrupt waiting and set interrupted flag.
     */
    void
    interrupt();

private:
    /*!
     * The queue.
     */
    std::queue<T> queue_;

    /*!
     * Mutex for thread-safety.
     */
    mutable std::mutex mutex_;

    /*!
     * The waiter.
     */
    std::condition_variable cv_;

    /*!
     * Set when no more waiting is required.
     */
    std::atomic_bool interrupted_;
};

} // namespace ys

#include "blocking_queue.tcc"

#endif // YS_BLOCKING_QUEUE_H

