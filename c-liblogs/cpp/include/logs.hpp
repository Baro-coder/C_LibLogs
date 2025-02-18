namespace logs 
{
    enum class Level
    {
        Notice = 0,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    void log(Level level, const char* message);
    void logNotice(const char* message);
    void logInfo(const char* message);
    void logDebug(const char* message);
    void logWarning(const char* message);
    void logError(const char* message);
    void logFatal(const char* message);

    void setLevel(Level level);
    Level getLevel();
} // namespace logs