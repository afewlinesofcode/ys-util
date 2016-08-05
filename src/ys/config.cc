/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-11-03
 * \brief  Config reader source file.
 */

#include <ys/config.h>
#include <iostream>

namespace ys
{

namespace po = boost::program_options;

/*!
 * Constructor.
 * \param argc Count of Application parameters.
 * \param argv Application parameters values.
 */
config::config(int argc, char** argv) :
    argc_ { argc },
    argv_ { argv }
{
}

/*!
 * Print out help message.
 */
void
config::print_help() const
{
    std::cout << options_ << std::endl;
}

/*!
 * Initialize a list of available command-line options.
 * \return
 */
boost::program_options::options_description_easy_init
config::init_cmd_options()
{
    return options_.add_options();
}

/*!
 * Read command-line options configured before.
 */
void
config::read_cmd_options()
{
    try
    {
        po::store(
            po::parse_command_line(argc_, argv_, options_),
            cmd_options_
        );

        po::notify(cmd_options_);
    }
    catch (boost::program_options::error& e)
    {
        throw error(e.what());
    }
}

/*!
 * Test if command-line option exists.
 * \param name Option name.
 * \return
 */
bool
config::has_cmd_option(std::string const& name) const
{
    return cmd_options_.count(name) > 0;
}

/*!
 * Get a map of command-line options values.
 * \return
 */
const config::cmd_options_type&
config::cmd_options() const
{
    return cmd_options_;
}

/*!
 * Get a map of config-file options values.
 * \return
 */
const config::cfg_options_type&
config::cfg_options() const
{
    return cfg_options_;
}

/*!
 * Read a vector of string options from a config into a container.
 */
void
config::load_cfg_string_list(std::string path, std::vector<std::string>* v) const
{
    for (auto& node : cfg_options().get_child(path)) {
        v->push_back(node.second.data());
    }
}

} // namespace ys

