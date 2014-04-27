#ifndef LOGGING_HPP_K8VSYA6N
#define LOGGING_HPP_K8VSYA6N

#include <ctime>
#include <sstream>

#include "util/util.hpp"
#include "level.hpp"
#include "output.hpp"

namespace logging {

#ifdef DISABLE_LOGGING
#define LOG //
#define LOG_CHECK //
#else
#define LOG(lvl) if ((logging::lvl) <= logging::getLevel()) logging::Log(logging::lvl, __FILENAME__, __LINE__, __func__).get()
#define LOG_CHECK(lvl, check) if (logging::lvl <= logging::getLevel() && (check)) logging::Log(logging::lvl, __FILENAME__, __LINE__, __func__).get()
#endif

/**
 * Sets the global logging level.
 * \param level Logging level.
 */
void setLevel(LogLevel level);

/**
 * Returns the global logging level.
 * \return Global logging level.  */
LogLevel getLevel();

/**
 * Adds an output configuration.
 * \param output Output configuration.
 * \return Output id.
 */
unsigned addOutput(Output *output);

/**
 * Removes output with a given id.
 * \param id Output id.
 */
void removeOutput(unsigned outputId);

/**
 * Log class.
 */
class Log {
private:
    std::ostringstream message;

    LogLevel level;
    std::time_t timestamp;
    std::string file;
    int line;
    std::string function;

public:
    Log(LogLevel level, const std::string &file, int line, const std::string &function);
    ~Log();

    std::ostringstream &get();
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Log::Log(LogLevel lvl, const std::string &f, int l, const std::string &func) :
    level(lvl), file(f), line(l), function(func)
{
    timestamp = std::time(nullptr);
}

inline std::ostringstream &Log::get() {
    return message;
}

}

#endif /* end of include guard: LOGGING_HPP_K8VSYA6N */
