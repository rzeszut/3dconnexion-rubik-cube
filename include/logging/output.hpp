#ifndef OUTPUT_HPP_WJFR9CBI
#define OUTPUT_HPP_WJFR9CBI

#include <ctime>
#include <mutex>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

#include "level.hpp"

namespace logging {

/**
 * Base class for logging outputs
 */
class Output {
public:
    virtual ~Output() {}

    /**
     * Writes a line to the log.
     * \param logLevel Logging severity level.
     * \param timestamp Timestamp
     * \param file File name
     * \param line Line number
     * \param message Message to log.
     */
    virtual void writeLine(LogLevel logLevel,
                           std::time_t timestamp,
                           const std::string &file,
                           int line,
                           const std::string &function,
                           const std::string &message) = 0;
};

/**
 * Simple base class for logging outputs.
 * Blocks while writing -- only one write per time allowed.
 */
class SimpleOutput : public Output {
private:
    LogLevel outputLevel;
    static std::mutex mtx;

protected:
    /**
     * Base constructor for extending classes.
     * \param outputLevel Logging level for this output.
     *  Ex. when global logging level is DEBUG, and this output's is INFO,
     *  then this outpu will not log DEBUG messages.
     */
    SimpleOutput(LogLevel outputLevel = LogLevel::DEBUG);

    /**
     * Returns output stream for a given log level.
     * \param logLevel Logging severity level.
     * \return Output stream.
     */
    virtual std::ostream &getOutput() = 0;

public:
    virtual ~SimpleOutput() {}

    void writeLine(LogLevel logLevel,
                   std::time_t timestamp,
                   const std::string &file,
                   int line,
                   const std::string &function,
                   const std::string &message);
};

/**
 * Simple console output -- logs messages to the stdout.
 */
class StdoutOutput : public SimpleOutput {
protected:
    std::ostream &getOutput();

public:
    StdoutOutput(LogLevel outputLevel = LogLevel::DEBUG);
};

/**
 * Simple console output -- logs messages to the stderr.
 */
class StderrOutput : public SimpleOutput {
protected:
    std::ostream &getOutput();

public:
    StderrOutput(LogLevel outputLevel = LogLevel::DEBUG);
};

/**
 * Simple file logger -- logs output to the file.
 */
class FileOutput : public SimpleOutput {
private:
    std::ofstream outputFile;

protected:
    std::ostream &getOutput();

public:
    explicit FileOutput(const char *filename, LogLevel level = LogLevel::DEBUG);
    ~FileOutput();
};

// ------------------ //
// inline definitions //
// ------------------ //
inline SimpleOutput::SimpleOutput(LogLevel level) :
    outputLevel(level)
{}

inline StdoutOutput::StdoutOutput(LogLevel outputLevel) :
    SimpleOutput(outputLevel)
{}

inline std::ostream &StdoutOutput::getOutput() {
    return std::cout;
}

inline StderrOutput::StderrOutput(LogLevel outputLevel) :
    SimpleOutput(outputLevel)
{}

inline std::ostream &StderrOutput::getOutput() {
    return std::cerr;
}

inline FileOutput::FileOutput(const char *filename, LogLevel level) :
    SimpleOutput(level),
    outputFile(filename)
{}

inline FileOutput::~FileOutput() {
    outputFile.close();
}

inline std::ostream &FileOutput::getOutput() {
    return outputFile;
}

}

#endif /* end of include guard: OUTPUT_HPP_WJFR9CBI */
