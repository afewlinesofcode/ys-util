/*!
 * \file
 * \author Stanislav Yaranov <stanislav.yaranov@gmail.com>
 * \date   2015-11-03
 * \brief  Config reader header file.
 */

#ifndef YS_CONFIG_H
#define YS_CONFIG_H

#include <string>
#include <type_traits>
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
     * Create a command-line option.
     * \return
     */
    template<typename T>
    boost::program_options::typed_value<T>*
    option()
    {
        return boost::program_options::value<T>();
    }

    /*!
     * Read command-line options configured before.
     */
    void
    read_cmd_options();

    /*!
     * Read a value of a command-line option into a variable.
     * \param name Option name.
     * \param v Target variable.
     * \return Value.
     */
    template<typename T>
    const T&
    read_cmd_option(const std::string& name, T* v)
    {
        if (cmd_options_.count(name))
            *v = cmd_options_[name].as<T>();
        return *v;
    }

    /*!
     * Get a map of command-line options values.
     * \return
     */
    const cmd_options_type&
    cmd_options() const;

    /*!
     * Test if command-line option exists.
     * \param name Option name.
     * \return
     */
    bool
    has_cmd_option(const std::string& name) const
    {
        return cmd_options_.count(name) > 0;
    }

    /*!
     * Read options from a config-file.
     * \param path Config-file path.
     */
    template<format F>
    void
    read_cfg_options(const std::string& path)
    {
        cfg_options_reader reader;
        reader.read<F>(path, cfg_options_);
    }

    /*!
     * Read a value of a config parameter into a variable.
     * \param path Path of the option in the configuration.
     * \param v Target variable.
     */
    template<typename T>
    void
    read_cfg_option(const std::string& path, T* v)
    {
        *v = cfg_options().get<T>(path);
    }

    /*!
     * Get a map of config-file options values.
     * \return
     */
    const cfg_options_type&
    cfg_options() const
    {
        return cfg_options_;
    }

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
     * Config-file options reader class.
     */
    class cfg_options_reader
    {
    public:
        /*!
         * Read options from a JSON config file.
         * \param path File path.
         * \param tree Target options container.
         * \return
         */
        template<format F>
        typename std::enable_if<F == format::json>::type
        read(const std::string& path, boost::property_tree::ptree& tree)
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
        template<format F>
        typename std::enable_if<F == format::xml>::type
        read(const std::string& path, boost::property_tree::ptree& tree)
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
    };
};

} // namespace ys

#endif // YS_CONFIG_H

