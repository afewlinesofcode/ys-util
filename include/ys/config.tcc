/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-11-03
 * \brief  Config reader source file.
 */

#ifndef YS_CONFIG_TCC
#define YS_CONFIG_TCC

#include "config.h"

namespace ys
{

/*!
 * Create a command-line option for initialization.
 * \return
 */
template<typename T>
boost::program_options::typed_value<T>*
config::option()
{
    return boost::program_options::value<T>();
}

/*!
 * Get command-line option value.
 */
template<typename T>
T
config::cmd_option(std::string const& name, bool* error) const
{
    T v {};

    if (cmd_options_.count(name))
    {
        v = cmd_options_[name].as<T>();

        if (error)
            *error = false;
    }
    else
    {
        if (error)
            *error = true;
    }

    return v;
}

/*!
 * Read a value of a command-line option into a variable.
 * \param name Option name.
 * \param v Target variable.
 * \return Value.
 */
template<typename T>
const T&
config::load_cmd_option(std::string const& name, T* v) const
{
    bool error {};

    T val = cmd_option<T>(name, &error);

    if (!error)
    {
        *v = val;
    }

    return *v;
}

/*!
 * Read options from a config-file.
 * \param path Config-file path.
 */
template<config::format F>
void
config::read_cfg_options(std::string const& path)
{
    read_config_file<F>(path, cfg_options_);
}

/*!
 * Read a value of a config parameter into a variable.
 * \param path Path of the option in the configuration.
 * \param v Target variable.
 */
template<typename T>
void
config::load_cfg_option(std::string const& path, T* v) const
{
    *v = cfg_options().get<T>(path);
}

/*!
 * Read options from a JSON config file.
 * \param path File path.
 * \param tree Target options container.
 * \return
 */
template<config::format F>
typename std::enable_if<F == config::format::json>::type
config::read_config_file(std::string const& path,
        boost::property_tree::ptree& tree)
{
    try
    {
        boost::property_tree::read_json(path, tree);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
        throw error(e.what());
    }
}

/*!
 * Read options from an XML config file.
 * \param path File path.
 * \param tree Target options container.
 * \return
 */
template<config::format F>
typename std::enable_if<F == config::format::xml>::type
config::read_config_file(std::string const& path,
        boost::property_tree::ptree& tree)
{
    try
    {
        boost::property_tree::read_xml(path, tree);
    }
    catch (boost::property_tree::xml_parser_error& e)
    {
        throw error(e.what());
    }
}

} // namespace ys

#endif // YS_CONFIG_TCC

