/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-11-03
 * \brief  Config reader header file.
 */

#ifndef YS_CONFIG_H
#define YS_CONFIG_H

#include <string>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ys/error.h>

namespace ys
{

/*!
 * \brief Config reader class.
 * \details Is used as a base config reader class.
 */
class config
{
public:
    /*!
     * Enum of known config formats.
     */
    enum format
    {
        json,
        xml
    };

    /*!
     * Constructor.
     * \param argc Count of Application parameters.
     * \param argv Application parameters values.
     */
    config(int argc, char** argv);

    /*!
     * Print out help message.
     */
    void
    print_help() const;

protected:
    /*!
     * Command-line options typedef.
     */
    using cmd_options_type = boost::program_options::variables_map;

    /*!
     * Config-file options typedef.
     */
    using cfg_options_type = boost::property_tree::ptree;

    /*!
     * Initialize a list of available command-line options.
     * \return
     */
    boost::program_options::options_description_easy_init
    init_cmd_options();

    /*!
     * Create a command-line option for initialization.
     * \return
     */
    template<typename T>
    boost::program_options::typed_value<T>*
    option();

    /*!
     * Read command-line options configured before into internal structures.
     */
    void
    read_cmd_options();

    /*!
     * Test if command-line option exists.
     * \param name Option name.
     * \return
     */
    bool
    has_cmd_option(std::string const& name) const;

    /*!
     * Get command-line option value.
     */
    template<typename T>
    T
    cmd_option(std::string const& name, bool* error = nullptr) const;

    /*!
     * Load value from command-line option into variable.
     * \param name Option name.
     * \param v Target variable.
     * \return Value.
     */
    template<typename T>
    const T&
    load_cmd_option(std::string const& name, T* v) const;

    /*!
     * Get a map of command-line options.
     * \return
     */
    const cmd_options_type&
    cmd_options() const;

    /*!
     * Read options from a config-file.
     * \param path Config-file path.
     */
    template<format F>
    void
    read_cfg_options(std::string const& path);

    /*!
     * Read a value of a config parameter into a variable.
     * \param path Path of the option in the configuration.
     * \param v Target variable.
     */
    template<typename T>
    void
    load_cfg_option(std::string const& path, T* v) const;

    /*!
     * Read a vector of string options from a config into a container.
     */
    void
    load_cfg_string_list(std::string path, std::vector<std::string>* v) const;

    /*!
     * Get a map of config-file options values.
     * \return
     */
    const cfg_options_type&
    cfg_options() const;

private:
    /*!
     * A count of parameters passed to the application.
     */
    int argc_;

    /*!
     * Values of parameters passed to the application.
     */
    char** argv_;

    /*!
     * Options summary combined from command-line options
     * and config-file options.
     */
    boost::program_options::options_description options_;

    /*!
     * Command-line options container.
     */
    cmd_options_type cmd_options_;

    /*!
     * Config-file options container.
     */
    cfg_options_type cfg_options_;

    /*!
     * Read options from a JSON config file.
     * \param path File path.
     * \param tree Target options container.
     * \return
     */
    template<format F>
    typename std::enable_if<F == format::json>::type
    read_config_file(std::string const& path,
            boost::property_tree::ptree& tree);

    /*!
     * Read options from an XML config file.
     * \param path File path.
     * \param tree Target options container.
     * \return
     */
    template<format F>
    typename std::enable_if<F == format::xml>::type
    read_config_file(std::string const& path,
            boost::property_tree::ptree& tree);
};

} // namespace ys

#include "config.tcc"

#endif // YS_CONFIG_H

