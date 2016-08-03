#include <iostream>
#include <ys/rsrc_set_mt.h>

class resource
{
public:
    resource() = delete;
    resource(int v) :
        val_ { v }
    {
    }

    int
    get() const
    {
        return val_;
    }

    void
    set(int v)
    {
        val_ = v;
    }

    static
    void
    dump(resource const& r)
    {
        std::cout << "resource value: " << r.get() << std::endl;
    }

private:
    int val_;
};

int
main(int argc, char* argv[])
{
    using rsrc_type = ys::rsrc_set_mt<resource>;

    rsrc_type st
    {
        []()
        {
            return std::make_shared<resource>(2);
        }
    };

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

