/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2016-08-03
 * \brief  Resources set source file.
 */

#ifndef YS_RSRC_SET_TCC
#define YS_RSRC_SET_TCC

#include "rsrc_set_mt.h"

namespace ys
{

/*!
 * Constructor.
 */
template<class T>
rsrc_set_mt<T>::rsrc_set_mt()
{
    initializer_ = []()
    {
        return std::make_shared<T>();
    };
}

/*!
 * Constructor.
 * \param init Initializer for new resources.
 */
template<class t>
rsrc_set_mt<t>::rsrc_set_mt(initializer_type const& init) :
    initializer_ { init }
{
}

/*!
 * Take a resource pointer from the set or
 * generate a resource if the set is empty.
 */
template<class T>
typename rsrc_set_mt<T>::resource_ptr
rsrc_set_mt<T>::take()
{
    std::lock_guard<std::mutex> lock { m_ };

    if (resources_.empty())
    {
        return this->initializer_();
    }
    else
    {
        resource_ptr res { resources_.back() };

        resources_.pop_back();

        return res;
    }
}

/*!
 * Put a resource back to the set.
 * \param r The resource.
 */
template<class T>
void
rsrc_set_mt<T>::put(resource_ptr r)
{
    std::lock_guard<std::mutex> lock { m_ };

    resources_.push_back(r);
}

} // namespace ys

#endif // YS_RSRC_SET_TCC

