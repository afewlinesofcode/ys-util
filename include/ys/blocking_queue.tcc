/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-06-29
 * \brief
 */

#include "blocking_queue.h"

namespace ys
{

/*!
 * Constructor.
 */
template<class T>
blocking_queue<T>::blocking_queue() :
    interrupted_ { false }
{
}

/*!
 * Destructor.
 * Interrupt queue if it is in waiting state.
 */
template<class T>
blocking_queue<T>::~blocking_queue()
{
    interrupt();
}

/*!
 * Add element to the end of the queue.
 * \param item Element to add.
 */
template<class T>
void
blocking_queue<T>::push(T const &item)
{
    interrupted_ = false;

    {
        std::lock_guard<std::mutex> lock { mutex_ };

        queue_.push(item);
    }

    cv_.notify_one();
}

/*!
 * Pop (in the case the queue is empty then wait and then pop) front
 * element from the queue.
 * \param item The result item.
 * \return true if item was fetched, false if fetch failed (interrupted, ...)
 */
template<class T>
bool
blocking_queue<T>::pop(T *item)
{
    do
    {
        std::unique_lock<std::mutex> lock { mutex_ };

        if (!queue_.empty())
        {
            *item = std::move(queue_.front());
            queue_.pop();

            return true;
        }

        if (!(bool)interrupted_)
            cv_.wait(lock);
    }
    while (!(bool)interrupted_);

    return false;
}

/*!
 * Check whether the queue is empty.
 * \return
 */
template<class T>
bool
blocking_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    return queue_.empty();
}

/*!
 * Get the size of the queue.
 * \return
 */
template<class T>
size_t
blocking_queue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    return queue_.size();
}

/*!
 * Interrupt waiting and set interrupted flag.
 */
template<class T>
void
blocking_queue<T>::interrupt()
{
    interrupted_ = true;

    cv_.notify_all();
}

} // namespace ys

