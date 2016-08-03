/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2016-07-29
 * \brief  ys error class header file.
 */

#ifndef YS_ERROR_H
#define YS_ERROR_H

#include <stdexcept>

namespace ys
{

/*!
 * ys error class.
 */
class error: public std::runtime_error
{
public:
    /*!
     * Constructor.
     * \param msg Error message.
     */
    error(std::string const& msg)
        : runtime_error(msg)
    {
    }
};

} // namespace ys

#endif // YS_ERROR_H

