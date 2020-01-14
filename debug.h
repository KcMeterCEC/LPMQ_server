#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>

#define LOG_TMP(prefix, v) (std::cout << prefix << ": " << __FILE__ << "> " << __FUNCTION__ << "> " << __LINE__ << ": " << v << std::endl)

#define LOG_INFO(v) (std::cout << "[INFO]: " << v << std::endl)

#define LOG_ERR(v)  LOG_TMP("ERR", v)

#ifdef DEBUG
#define LOG_DBG(v)  LOG_TMP("DEBUG", v)
#else
#define LOG_DBG(v)
#endif

#endif