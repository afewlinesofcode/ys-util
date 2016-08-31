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
class blocking_queue: public std::queue<T>
{
public:
    /*!
     * Base queue typedef.
     */
    using queue_type = std::queue<T>;

    /*
     * Import typedefs from base queue type.
     */
    using container_type = typename queue_type::container_type;
    using value_type = typename queue_type::value_type;
    using size_type = typename queue_type::size_type;
    using reference = typename queue_type::reference;
    using const_reference = typename queue_type::const_reference;

    /*!
     * Typedef for a lock used when modifying queue.
     */
    using lock_type = std::lock_guard<std::mutex>;

    /*!
     * Construct the queue.
     */
    blocking_queue();

    /*!
     * Destruct the queue.
     * \details Interrupt queue if it is in waiting state.
     */
    ~blocking_queue();

    /*!
     * Check whether the queue is empty.
     * \return
     */
    bool
    empty() const;

    /*!
     * Return the number of elements.
     * \return
     */
    size_t
    size() const;

    /*!
     * Insert element at the end.
     * \param item Element to insert.
     */
    void
    push(T const& value);

    /*!
     * Insert element at the end.
     * \param item Element to insert.
     */
    void
    push(T&& value);

    /*!
     * Construct element in-place at the end.
     */
    template<class ...Args>
    void
    emplace(Args&&... args);

    /*!
     * Remove the first element.
     */
    void
    pop();

    /*!
     * Pop an element.
     * Remove an element from the front of the queue into the passed parameter.
     * If the queue is empty then first wait for an element.
     * \param item The removed element.
     * \return Returns false if the operation was interrupted.
     */
    bool
    pop(T* item);

    /*!
     * Swap the contents.
     */
    void
    swap(blocking_queue& q);

    /*!
     * Interrupt waiting and set interrupted flag.
     */
    void
    interrupt();

    /*!
     * Specializes the std::swap algorithm.
     * \param l Left queue.
     * \param r Right queue.
     */
    friend
    void
    swap(blocking_queue<T>& l, blocking_queue<T>& r)
    {
        l.swap(r);
    }

private:
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

