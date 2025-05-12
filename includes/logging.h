// Small logging lib I made for convenience :-)
#ifndef LOGGING_HGUARD
#define LOGGING_HGUARD

#define LOG_INFO(str) Serial.println("INFO: " str)
#define LOG_WARN(str) Serial.println("\e[33mWARNING\e[0m: " str)
#define LOG_ERR(str) Serial.println("\e[31mERROR\e[0m: " str " Halting..."); while(1) ping(ERROR_NOTE)

#define LOG_TASK(str) Serial.print("INFO: " str "... ")
#define LOG_TASK_SUCCESS Serial.println("\e[32mSuccess\e[0m")
#define LOG_TASK_FAILURE Serial.println("\e[31mFailure\e[0m")

#define CHECK_STATUS(method, err)\
status = method;\
if (status == 0) {\
  LOG_TASK_FAILURE;\
  LOG_ERR(err);\
}\
LOG_TASK_SUCCESS;

#endif // LOGGING_HGUARD
