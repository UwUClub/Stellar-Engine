#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <fstream>
#include <iostream>
#include <string>

class Logger
{
    public:
        Logger() = delete;

        static void log(const std::string &aMessage)
        {
            if (Config::getEnable()) {
                std::string msg = "[LOG]: " + aMessage;

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void log(const std::string &aMessage, int aLine, const std::string &aFile)
        {
            if (Config::getEnable()) {
                std::string msg = "[LOG]: " + aMessage + " (line: " + std::to_string(aLine) + ", file: " + aFile + ")";

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void debug(const std::string &aMessage)
        {
            if (Config::getDebug()) {
                std::string msg = "[DEBUG]: " + aMessage;

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void debug(const std::string &aMessage, int aLine, const std::string &aFile)
        {
            if (Config::getDebug()) {
                std::string msg =
                    "[DEBUG]: " + aMessage + " (line: " + std::to_string(aLine) + ", file: " + aFile + ")";

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void info(const std::string &aMessage)
        {
            if (Config::getInfo()) {
                std::string msg = "[INFO]: " + aMessage;

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void info(const std::string &aMessage, int aLine, const std::string &aFile)
        {
            if (Config::getInfo()) {
                std::string msg = "[INFO]: " + aMessage + " (line: " + std::to_string(aLine) + ", file: " + aFile + ")";

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void warning(const std::string &aMessage)
        {
            if (Config::getWarning()) {
                std::string msg = "[WARNING]: " + aMessage;

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void warning(const std::string &aMessage, int aLine, const std::string &aFile)
        {
            if (Config::getWarning()) {
                std::string msg =
                    "[WARNING]: " + aMessage + " (line: " + std::to_string(aLine) + ", file: " + aFile + ")";

                saveToFile(msg);
                std::cout << msg << '\n';
            }
        }

        static void error(const std::string &aMessage)
        {
            if (Config::getError()) {
                std::string msg = "[ERROR]: " + aMessage;

                saveToFile(msg);
                std::cerr << msg << '\n';
            }
        }

        static void error(const std::string &aMessage, int aLine, const std::string &aFile)
        {
            if (Config::getError()) {
                std::string msg =
                    "[ERROR]: " + aMessage + " (line: " + std::to_string(aLine) + ", file: " + aFile + ")";

                saveToFile(msg);
                std::cerr << msg << '\n';
            }
        }

        static void saveToFile(const std::string &message)
        {
            if (!Config::getFilePath().empty()) {
                std::ofstream file = Config::getShouldEraseOldLogs()
                                         ? std::ofstream(Config::getFilePath(), std::ios::out | std::ios::trunc)
                                         : std::ofstream(Config::getFilePath(), std::ios::out | std::ios::app);

                if (file.is_open()) {
                    file << message << '\n';
                    if (file.fail()) {
                        std::cerr << "Failed to write to file: " << Config::getFilePath() << '\n';
                    }
                    if (Config::getShouldEraseOldLogs()) {
                        Config::setShouldEraseOldLogs(false);
                    }
                    file.close();
                }
            };
        }

        class Config
        {
            public:
                static Config &getInstance()
                {
                    static Config instance;

                    return instance;
                }

                static void setEnable(bool enable)
                {
                    Config::getInstance().enable = enable;
                }

                static void setDebug(bool debug)
                {
                    Config::getInstance().debug = debug;
                }

                static void setInfo(bool info)
                {
                    Config::getInstance().info = info;
                }

                static void setWarning(bool warning)
                {
                    Config::getInstance().warning = warning;
                }

                static void setError(bool error)
                {
                    Config::getInstance().error = error;
                }

                static void setFilePath(const std::string &filePath)
                {
                    Config::getInstance().filePath = filePath;
                }

                static void setShouldEraseOldLogs(bool shouldEraseOldLogs)
                {
                    Config::getInstance().shouldEraseOldLogs = shouldEraseOldLogs;
                }

                static bool getEnable()
                {
                    return Config::getInstance().enable;
                }

                static bool getDebug()
                {
                    return Config::getInstance().debug;
                }

                static bool getInfo()
                {
                    return Config::getInstance().info;
                }

                static bool getWarning()
                {
                    return Config::getInstance().warning;
                }

                static bool getError()
                {
                    return Config::getInstance().error;
                }

                static std::string &getFilePath()
                {
                    return Config::getInstance().filePath;
                }

                static bool getShouldEraseOldLogs()
                {
                    return Config::getInstance().shouldEraseOldLogs;
                }

                bool enable = true;
                bool debug = true;
                bool info = true;
                bool warning = true;
                bool error = true;
                bool shouldEraseOldLogs = true;
                std::string filePath = "log.txt";

            private:
                Config() = default;
        };
};

#endif /* !LOGGER_HPP_ */
