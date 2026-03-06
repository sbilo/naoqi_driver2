/*
 * Copyright 2015 Aldebaran
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file replaces qicore/logmessage.hpp from libqicore so that the driver
 * can use modern libqi (>= 4.x) without a dependency on libqicore.
 * The struct layout mirrors qi::LogMessage as defined in libqicore.
*/

#pragma once

#include <qi/log.hpp>
#include <qi/anyobject.hpp>
#include <qi/clock.hpp>
#include <qi/os.hpp>
#include <qi/type/detail/structtypeinterface.hxx>
#include <tuple>

QI_TYPE_ENUM(qi::LogLevel)

namespace qi
{
namespace detail
{
  namespace name
  {
    inline const char* LogMessage_timestamp()  { return "timestamp"; }
    inline const char* LogMessage_systemDate() { return "systemDate"; }
    inline const char* LogMessage_date()       { return "date"; }
  }
}

struct LogMessage
{
  std::string source;                       // File:function:line
  qi::LogLevel level = qi::LogLevel_Info;   // Level of verbosity of the message
  std::string category;                     // Category of the message
  std::string location;                     // machineID:PID
  std::string message;                      // The message itself
  unsigned int id = 0;                      // Unique message ID
  qi::Clock::time_point date;               // Steady clock timestamp
  qi::SystemClock::time_point systemDate;   // Wall clock timestamp
};
} // namespace qi

// Conversion handlers for backward compat with older robots that send
// a "timestamp" (qi::os::timeval) field instead of date/systemDate.
inline bool qi_LogMessage_fromOld(
    std::map<std::string, ::qi::AnyValue>& fields,
    const std::vector<std::tuple<std::string, qi::TypeInterface*>>& missing,
    const std::map<std::string, ::qi::AnyReference>& dropfields)
{
  try
  {
    if (dropfields.size() == 1)
    {
      auto dropIt = dropfields.find(qi::detail::name::LogMessage_timestamp());
      if (dropIt != dropfields.end() && missing.size() == 2 &&
          ((std::get<0>(missing.at(0)) == qi::detail::name::LogMessage_date() &&
            std::get<0>(missing.at(1)) == qi::detail::name::LogMessage_systemDate()) ||
           (std::get<0>(missing.at(1)) == qi::detail::name::LogMessage_date() &&
            std::get<0>(missing.at(0)) == qi::detail::name::LogMessage_systemDate())))
      {
        const qi::os::timeval timestamp = dropIt->second.to<qi::os::timeval>();
        fields[qi::detail::name::LogMessage_date()] = qi::AnyValue(qi::Clock::time_point());
        fields[qi::detail::name::LogMessage_systemDate()] = qi::AnyValue(
            qi::SystemClock::time_point(qi::Seconds(timestamp.tv_sec) +
                                        qi::MicroSeconds(timestamp.tv_usec)));
        return true;
      }
    }
  }
  catch (const std::exception&) {}
  return false;
}

inline bool qi_LogMessage_toOld(
    std::map<std::string, ::qi::AnyValue>& fields,
    const std::vector<std::tuple<std::string, qi::TypeInterface*>>& missing,
    const std::map<std::string, ::qi::AnyReference>& dropfields)
{
  try
  {
    if (missing.size() == 1 &&
        std::get<0>(missing.front()) == qi::detail::name::LogMessage_timestamp())
    {
      if (dropfields.size() == 2)
      {
        auto systemDateIt = dropfields.find(qi::detail::name::LogMessage_systemDate());
        if (systemDateIt != dropfields.end())
        {
          const qi::SystemClock::time_point systemDate =
              systemDateIt->second.to<qi::SystemClock::time_point>();
          fields[qi::detail::name::LogMessage_timestamp()] =
              qi::AnyValue(qi::os::timeval(systemDate.time_since_epoch()));
          return true;
        }
      }
    }
  }
  catch (const std::exception&) {}
  return false;
}

QI_TYPE_STRUCT_EXTENSION_CONVERT_HANDLERS(::qi::LogMessage, qi_LogMessage_fromOld, qi_LogMessage_toOld);
QI_TYPE_STRUCT(::qi::LogMessage, source, level, category, location, message, id, date, systemDate);
