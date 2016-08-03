/*!
 * \file   
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2016-08-03
 * \brief  ys::rsrc_set_mt class test file.
 *
 * TODO: Convert to test and move to test dir.
 */

#include <iostream>
#include <ys/rsrc_set_mt.h>

/*!
 * Dummy resource class.
 */
class resource
{
public:
    /*!
     * Forbid use of default constructor.
     */
    resource() = delete;

    /*!
     * \brief Constructor.
     * \details Construct with a value of a resource.
     * \param v
     */
    resource(int v) :
        val_ { v }
    {
    }

    /*!
     * Get a value of a resource.
     * \return
     */
    int
    get() const
    {
        return val_;
    }

    /*!
     * Set a value of a resource.
     * \param v
     */
    void
    set(int v)
    {
        val_ = v;
    }

    /*!
     * Print out a resource value.
     * \param r The resource.
     */
    static
    void
    dump(resource const& r)
    {
        std::cout << "resource value: " << r.get() << std::endl;
    }

private:
    /*!
     * A resource value.
     */
    int val_;
};

/*!
 * Main function.
 * \param argc
 * \param argc
 */
int
main(int argc, char* argv[])
{
    /*!
     * Comfort typedef.
     */
    using rsrc_type = ys::rsrc_set_mt<resource>;

    /*!
     * Create a resource set with a custom resource initializer
     * because we're unable to use default constructor.
     */
    rsrc_type st
    {
        []()
        {
            return std::make_shared<resource>(2);
        }
    };

    /*
     * Make some checks.
     */

    rsrc_type::resource_ptr p1 = st.take();
    rsrc_type::resource_ptr p2 = st.take();
    rsrc_type::resource_ptr p3;

    resource::dump(*p1);
    resource::dump(*p2);

    p1->set(14);
    st.put(p1);

    p3 = st.take();

    resource::dump(*p3);

    return 0;
}

