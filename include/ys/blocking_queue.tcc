/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-06-29
 * \brief
 */

#include "blocking_queue.h"

#include <utility>

namespace ys
{


/*!
 * Constructor.
 */
template<class T>
blocking_queue<T>::blocking_queue() :
    queue_type {}, interrupted_ { false }
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
 * Check whether the queue is empty.
 * \return
 */
template<class T>
bool
blocking_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    return queue_type::empty();
}

/*!
 * Return the number of elements.
 * \return
 */
template<class T>
size_t
blocking_queue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    return queue_type::size();
}

/*!
 * Insert element at the end.
 * \param item Element to insert.
 */
template<class T>
void
blocking_queue<T>::push(T const& item)
{
    interrupted_ = false;

    {
        lock_type lock { mutex_ };

        queue_type::push(item);
    }

    cv_.notify_one();
}

/*!
 * Insert element at the end.
 * \param item Element to insert.
 */
template<class T>
void
blocking_queue<T>::push(T&& item)
{
    interrupted_ = false;

    {
        lock_type lock { mutex_ };

        queue_type::push(item);
    }

    cv_.notify_one();
}

/*!
 * Construct element in-place at the end.
 */
template<class T>
template<class ...Args>
void
blocking_queue<T>::emplace(Args&&... args)
{
    queue_type::emplace(std::forward<Args>(args)...);
}

/*!
 * Remove the first element.
 */
template<class T>
void
blocking_queue<T>::pop()
{
    lock_type lock { mutex_ };

    queue_type::pop();
}

/*!
 * Pop an element.
 * Remove an element from the front of the queue into the passed parameter.
 * If the queue is empty then first wait for an element.
 * \param item The removed element.
 * \return Returns false if the operation was interrupted.
 */
template<class T>
bool
blocking_queue<T>::pop(T* item)
{
    do
    {
        std::unique_lock<std::mutex> lock { mutex_ };

        if (!queue_type::empty())
        {
            *item = std::move(queue_type::front());
            queue_type::pop();

            return true;
        }

        if (!(bool)interrupted_)
            cv_.wait(lock);
    }
    while (!(bool)interrupted_);

    return false;
}

/*!
 * Swap the contents.
 */
template<class T>
void
blocking_queue<T>::swap(blocking_queue& q)
{
    lock_type l1 { mutex_ };
    lock_type l2 { q.mutex_ };

    queue_type::swap(q);
    swap(interrupted_, q.interrupted_);
}

/*!
 * Interrupt waiting and set interrupted flag on.
 */
template<class T>
void
blocking_queue<T>::interrupt()
{
    interrupted_ = true;

    cv_.notify_all();
}

} // namespace ys

