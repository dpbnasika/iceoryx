// Copyright (c) 2019 - 2021 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_HOOFS_LOG_LOGSTREAM_HPP
#define IOX_HOOFS_LOG_LOGSTREAM_HPP

#include "iceoryx_hoofs/cxx/type_traits.hpp"
#include "iceoryx_hoofs/log/logger.hpp"
#include "iox/string.hpp"

#include <string>

namespace iox
{
namespace log
{
class LogStream;

/// @brief Helper struct to log in hexadecimal format
template <typename T>
class LogHex
{
  public:
    friend class LogStream;

    template <typename = std::enable_if_t<std::is_arithmetic<T>::value || std::is_pointer<T>::value>>
    explicit constexpr LogHex(const T value) noexcept;

  private:
    T m_value;
};

/// @brief Log a number in hexadecimal format
/// @tparam[in] T the arithmetic data type of the value to log
/// @param[in] value to be logged
/// @return a helper struct which will be used by the LogStream
// AXIVION Next Construct AutosarC++19_03-M17.0.3 : The function is in the iox::log namespace which prevents easy misuse
template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
constexpr LogHex<T> hex(const T value) noexcept;

/// @brief Log a pointer in hexadecimal format
/// @param[in] ptr is the pointer to be logged
/// @return a helper struct which will be used by the LogStream
// AXIVION Next Construct AutosarC++19_03-M17.0.3 : The function is in the iox::log namespace which prevents easy misuse
LogHex<const void* const> hex(const void* const ptr) noexcept;

/// @brief Helper struct to log in octal format
template <typename T>
class LogOct
{
  public:
    friend class LogStream;

    template <typename = std::enable_if_t<std::is_integral<T>::value>>
    inline explicit constexpr LogOct(const T value) noexcept;

  private:
    T m_value;
};

/// @brief Log a number in octal format
/// @tparam[in] T the arithmetic data type of the value to log
/// @param[in] value to be logged
/// @return a helper struct which will be used by the LogStream
// AXIVION Next Construct AutosarC++19_03-M17.0.3 : The function is in the iox::log namespace which prevents easy misuse
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
inline constexpr LogOct<T> oct(const T value) noexcept;

/// @todo iox-#1755 implement LogBin and LogRawBuffer

/// @brief This class provides the public interface to the logger and is used with the 'IOX_LOG' macro. In order to add
/// support for custom data types 'operator<<' needs to be implement for the custom type.
/// @code
/// iox::log::LogStream& operator<<(iox::log::LogStream& stream, const MyType& myType) noexcept
/// {
///     stream << "MyType { member = " << myType.member << " }";
///     return stream;
/// }
/// @endcode
class LogStream
{
  public:
    /// @brief Constructor for a LogStream object with an externally provided logger
    /// @note This is not intended for public use! Use the 'IOX_LOG' macro instead
    /// @param[in] logger to be used by the LogStream instance
    /// @param[in] file the file of the log message. Please use the '__FILE__' compiler intrinsic
    /// @param[in] line the line of the log message. Please use the '__LINE__' compiler intrinsic
    /// @param[in] function the function of the log message. Please use the '__FUNCTION__' compiler intrinsic
    /// @param[in] logLevel is the log level for the log message
    // AXIVION Next Construct AutosarC++19_03-A3.9.1 : file, line and function are used in conjunction with '__FILE__',
    // '__LINE__' and '__FUNCTION__'; these are compiler intrinsic and cannot be changed to fixed width types in a
    // platform agnostic way
    LogStream(Logger& logger, const char* file, const int line, const char* function, LogLevel logLevel) noexcept;

    /// @brief Constructor for a LogStream object with the logger from iox::log::Logger::get
    /// @note This is not intended for public use! Use the 'IOX_LOG' macro instead
    /// @param[in] file the file of the log message. Please use the '__FILE__' compiler intrinsic
    /// @param[in] line the line of the log message. Please use the '__LINE__' compiler intrinsic
    /// @param[in] function the function of the log message. Please use the '__FUNCTION__' compiler intrinsic
    /// @param[in] logLevel is the log level for the log message
    // AXIVION Next Construct AutosarC++19_03-A3.9.1 : file, line and function are used in conjunction with '__FILE__',
    // '__LINE__' and '__FUNCTION__'; these are compiler intrinsic and cannot be changed to fixed width types in a
    // platform agnostic way
    LogStream(const char* file, const int line, const char* function, LogLevel logLevel) noexcept;

    /// @brief Constructor for a LogStream object with the logger from iox::log::Logger::get
    /// @note This is not intended for public use! Use the 'IOX_LOG' macro instead
    /// @param[in] file the file of the log message. Please use the '__FILE__' compiler intrinsic
    /// @param[in] line the line of the log message. Please use the '__LINE__' compiler intrinsic
    /// @param[in] function the function of the log message. Please use the '__FUNCTION__' compiler intrinsic
    /// @param[in] logLevel is the log level for the log message
    /// @todo iox-#1755 temporary workaround due to lazy evaluation issues with Axivion; should be removed when the
    /// lazy evaluation can be implemented in an way Axivion does not complain
    // AXIVION Next Construct AutosarC++19_03-A3.9.1 : file, line and function are used in conjunction with '__FILE__',
    // '__LINE__' and '__FUNCTION__'; these are compiler intrinsic and cannot be changed to fixed width types in a
    // platform agnostic way
    LogStream(const char* file, const int line, const char* function, LogLevel logLevel, bool doFlush) noexcept;

    virtual ~LogStream() noexcept;

    LogStream(const LogStream&) = delete;
    LogStream(LogStream&&) = delete;

    LogStream& operator=(const LogStream&) = delete;
    LogStream& operator=(LogStream&&) = delete;

    /// @brief Helper function to convert a r-value reference into a l-value reference in order to be able to extend the
    /// data types which can be logged with LogStream object. Without this, one would need to always log a builtin type
    /// before a custom type could be logged
    /// @return a reference to the LogStream instance
    LogStream& self() noexcept;

    /// @brief Logging support for C-style strings
    /// @param[in] cstr is the C-style string to log
    /// @return a reference to the LogStream instance
    // AXIVION Next Construct AutosarC++19_03-A3.9.1 : Logging support for C-style strings
    LogStream& operator<<(const char* cstr) noexcept;

    /// @brief Logging support for std::string
    /// @param[in] str is the std::string to log
    /// @return a reference to the LogStream instance
    /// @todo iox-#1755 instead of using std::string we could also accept everything with a c_str() method
    /// and avoid the std::string dependency; alternatively this could be implemented as free function
    LogStream& operator<<(const std::string& str) noexcept;

    /// @brief Logging support for boolean
    /// @param[in] val is the boolean to log
    /// @return a reference to the LogStream instance
    LogStream& operator<<(const bool val) noexcept;

    /// @brief Logging support for arithmetic numbers in decimal format
    /// @tparam[in] T is the arithmetic data type of the value to log
    /// @param[in] val is the number to log
    /// @return a reference to the LogStream instance
    template <typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, bool> = 0>
    LogStream& operator<<(const T val) noexcept;

    /// @brief Logging support for integral numbers in hexadecimal format
    /// @tparam[in] T is the integral data type of the value to log
    /// @param[in] val is the number to log
    /// @return a reference to the LogStream instance
    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, bool> = 0>
    LogStream& operator<<(const LogHex<T> val) noexcept;

    /// @brief Logging support for floating point numbers in hexadecimal format
    /// @tparam[in] T is the floating point data type of the value to log
    /// @param[in] val is the number to log
    /// @return a reference to the LogStream instance
    template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = 0>
    LogStream& operator<<(const LogHex<T> val) noexcept;

    /// @brief Logging support for pointer in hexadecimal format
    /// @param[in] val is the pointer to log
    /// @return a reference to the LogStream instance
    LogStream& operator<<(const LogHex<const void* const> val) noexcept;

    /// @brief Logging support for integral numbers in octal format
    /// @tparam[in] T is the integral data type of the value to log
    /// @param[in] val is the number to log
    /// @return a reference to the LogStream instance
    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, bool> = 0>
    LogStream& operator<<(const LogOct<T> val) noexcept;

    /// @brief Logging support for callable. This gives access to the LogStream instance which e.g. can be used in a
    /// loop
    /// @tparam[in] Callable with a signature 'iox::log::LogStream&(iox::log::LogStream&)'
    /// @param[in] c is the callable which receives a LogStream object for the actual logging
    /// @code
    /// IOX_LOG(INFO) << [] (auto& stream) -> auto& {
    ///     for(const auto& num: {13, 37, 42, 73}) {
    ///         stream << num << " ";
    ///     }
    ///     return stream;
    /// };
    /// @endcode
    template <typename Callable,
              typename = std::enable_if_t<cxx::is_invocable_r<LogStream&, Callable, LogStream&>::value>>
    LogStream& operator<<(const Callable& c) noexcept;

    /// @brief Logging support for LogLevel
    /// @param[in] value is the LogLevel to log
    /// @return a reference to the LogStream instance
    LogStream& operator<<(const LogLevel value) noexcept;

  private:
    void flush() noexcept;

    // JUSTIFICATION it is fine to use a reference since the LogStream object is intentionally not movable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    Logger& m_logger;
    bool m_isFlushed{false};

    /// @todo iox-#1755 workaround due to deactivation of lazy evaluation
    bool m_doFlush{true};
};

namespace internal
{
/// @brief This is an internal helper struct to fully remove the logger from the compiled binary
/// when 'IOX_MINIMAL_LOG_LEVEL == OFF'. It is not intended for direct usage.
struct LogStreamOff
{
    // AXIVION Next Construct AutosarC++19_03-A3.9.1 : file, line and function are used in conjunction with '__FILE__',
    // '__LINE__' and '__FUNCTION__'; these are compiler intrinsic and cannot be changed to fixed width types in a
    // platform agnostic way
    inline LogStreamOff(const char*, const int, const char*, LogLevel, bool) noexcept;
    inline LogStreamOff& self() noexcept;

    template <typename T>
    inline LogStreamOff& operator<<(T&&) noexcept;
};

template <LogLevel level>
struct LogStreamTypeSelector
{
    using type = iox::log::LogStream;
};

template <>
struct LogStreamTypeSelector<iox::log::LogLevel::OFF>
{
    using type = iox::log::internal::LogStreamOff;
};

using SelectedLogStream = typename LogStreamTypeSelector<MINIMAL_LOG_LEVEL>::type;
} // namespace internal

} // namespace log
} // namespace iox

#include "iceoryx_hoofs/internal/log/logstream.inl"

#endif // IOX_HOOFS_LOG_LOGSTREAM_HPP
