/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2016-08-03
 * \brief  Example of using ys::config.
 */

#include <ys/config.h>

#include <iostream>
#include <string>

#define CONFIG_PATH "config.json"

class config: public ys::config
{
public:
    /*!
     * Config data structure.
     */
    struct
    {
        int opt1;

        struct
        {
            std::string opt1_1;
            std::string opt1_2;
        } nested;
    } data;

    /*!
     * Whether the application need to proceed.
     */
    bool skip { false };

    /*!
     * Constructor.
     * \param argc
     * \param argv
     */
    config(int argc, char** argv) :
        ys::config(argc, argv)
    {
        /*
         * Initialize and read command-line options.
         */
        init_cmd_options();
        read_cmd_options();

        /*
         * Read config-file options.
         */
        read_cfg_options();

        std::cout << "val:" << cmd_option<int>("opt1") << std::endl;

        /*
         * First load options from config-file.
         */
        load_cfg_options();

        /*
         * Then load options came from command-line as we're able to
         * override some.
         */
        load_cmd_options();

        /*
         * If help requested then the application should not proceed.
         */
        if (has_cmd_option("help"))
        {
            skip = true;
        }
    }

    /*!
     * Initialize options configurable from console.
     */
    void
    init_cmd_options()
    {
        ys::config::init_cmd_options()
        ( "help", "Show help" )
        ( "opt1", option<int>(), "opt1 value" );
    }

    /*!
     * Load command-line options into variables.
     */
    void
    load_cmd_options()
    {
        read_cmd_option("opt1", &data.opt1);
    }

    /*!
     * Read config-file options.
     */
    void
    read_cfg_options()
    {
        ys::config::read_cfg_options<json>(CONFIG_PATH);
    }

    /*!
     * Load config-file options into variables.
     */
    void
    load_cfg_options()
    {
        read_cfg_option("opt1", &data.opt1);
        read_cfg_option("nested.opt1_1", &data.nested.opt1_1);
        read_cfg_option("nested.opt1_2", &data.nested.opt1_2);
    }

    /*!
     * Print out config data.
     */
    friend
    std::ostream&
    operator<<(std::ostream& os, config const& c)
    {
        os <<
           "opt1 = " << c.data.opt1 << std::endl <<
           "nested.opt1_1 = " << c.data.nested.opt1_1 << std::endl <<
           "nested.opt1_2 = " << c.data.nested.opt1_2 <<
           std::endl;

        return os;
    }
};

/*!
 * Main function.
 * \param argc
 * \param argv
 */
int
main(int argc, char* argv[])
{
    config c { argc, argv };

    if (c.skip)
    {
        c.print_help();
    }
    else
    {
        std::cout << c;
    }
}

