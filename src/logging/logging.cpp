#include <locale>
#include <list>
#include <memory>
#include <iomanip>

#include "logging/output.hpp"
#include "logging/logging.hpp"

using namespace std;

namespace logging {

mutex SimpleOutput::mtx;

// remove this function when GCC supports std::put_time
// as of version 4.8.1, std::put_time is not supported
static void printTm(ostream &stream, tm *time) {
    stream << setfill('0')
        << time->tm_year + 1900 << "-"
        << setw(2) << time->tm_mon << "-"
        << setw(2) << time->tm_mday << " "
        << setw(2) << time->tm_hour << ":"
        << setw(2) << time->tm_min << ":"
        << setw(2) << time->tm_sec;
}

void SimpleOutput::writeLine(LogLevel level,
                             time_t timestamp,
                             const string &file,
                             int line,
                             const string &function,
                             const string &message) {
    if (level <= outputLevel) {
        ostream &out = this->getOutput();

        // lock for simultaneous print
        mtx.lock();

        // log level
        switch (level) {
        case DEBUG: out << "DEBUG"; break;
        case INFO: out << "INFO"; break;
        case WARNING: out << "WARNING"; break;
        default: out << "ERROR";
        }

        // log timestamp
        auto time = localtime(&timestamp);
        //out << " - " << put_time(time, "%F %D") << " ";
        out << " - ";
        printTm(out, time);
        out << " ";

        // log file, line and function
        out << file << ":" << line << " :: " << function << "() -- ";

        // log message
        out << message << endl;

        mtx.unlock();
    }
}

static LogLevel loggingLevel = DEBUG;
static list<std::shared_ptr<Output>> outputs;

Log::~Log() {
    for (auto &out : outputs) {
        out->writeLine(level, timestamp, file, line, function, message.str());
    }
}

void setLevel(LogLevel l) {
    loggingLevel = l;
}

LogLevel getLevel() {
    return loggingLevel;
}

unsigned addOutput(Output *o) {
    auto id = outputs.size();
    outputs.push_back(std::shared_ptr<Output>(o));
    return id;
}

void removeOutput(unsigned outputId) {
    if (outputId >= outputs.size()) {
        return;
    }
    auto it = outputs.begin();
    advance(it, outputId);
    outputs.erase(it);
}

}

