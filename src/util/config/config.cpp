#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <utility>

#include "config.h"

#include "../log/log.h"

#include "../util_env.h"

namespace dxvk {

  static bool isWhitespace(char ch) {
    return ch == ' ' || ch == '\x9' || ch == '\r';
  }

  
  static bool isValidKeyChar(char ch) {
    return (ch >= '0' && ch <= '9')
        || (ch >= 'A' && ch <= 'Z')
        || (ch >= 'a' && ch <= 'z')
        || (ch == '.' || ch == '_');
  }


  static size_t skipWhitespace(const std::string& line, size_t n) {
    while (n < line.size() && isWhitespace(line[n]))
      n += 1;
    return n;
  }


  struct ConfigContext {
    bool active;
  };


  static void parseUserConfigLine(Config& config, ConfigContext& ctx, const std::string& line) {
    std::stringstream key;
    std::stringstream value;

    // Extract the key
    size_t n = skipWhitespace(line, 0);

    if (n < line.size() && line[n] == '[') {
      n += 1;

      size_t e = line.size() - 1;
      while (e > n && line[e] != ']')
        e -= 1;

      while (n < e)
        key << line[n++];
      
      ctx.active = key.str() == env::getExeName();
    } else {
      while (n < line.size() && isValidKeyChar(line[n]))
        key << line[n++];
      
      // Check whether the next char is a '='
      n = skipWhitespace(line, n);
      if (n >= line.size() || line[n] != '=')
        return;

      // Extract the value
      bool insideString = false;
      n = skipWhitespace(line, n + 1);

      while (n < line.size()) {
        if (!insideString && isWhitespace(line[n]))
          break;

        if (line[n] == '"') {
          insideString = !insideString;
          n++;
        } else
          value << line[n++];
      }
      
      if (ctx.active)
        config.setOption(key.str(), value.str());
    }
  }


  Config::Config() { }
  Config::~Config() { }


  Config::Config(OptionMap&& options)
  : m_options(std::move(options)) { }


  void Config::merge(const Config& other) {
    for (auto& pair : other.m_options)
      m_options.insert(pair);
  }


  void Config::setOption(const std::string& key, const std::string& value) {
    m_options.insert_or_assign(key, value);
  }


  std::string Config::getOptionValue(const char* option) const {
    auto iter = m_options.find(option);

    return iter != m_options.end()
      ? iter->second : std::string();
  }


  bool Config::parseOptionValue(
    const std::string&  value,
          std::string&  result) {
    result = value;
    return true;
  }


  bool Config::parseOptionValue(
    const std::string&  value,
          bool&         result) {
    static const std::array<std::pair<const char*, bool>, 2> s_lookup = {{
      { "true",  true  },
      { "false", false },
    }};

    return parseStringOption(value,
      s_lookup.begin(), s_lookup.end(), result);
  }


  bool Config::parseOptionValue(
    const std::string&  value,
          int32_t&      result) {
    if (value.size() == 0)
      return false;
    
    // Parse sign, don't allow '+'
    int32_t sign = 1;
    size_t start = 0;

    if (value[0] == '-') {
      sign = -1;
      start = 1;
    }

    // Parse absolute number
    int32_t intval = 0;

    for (size_t i = start; i < value.size(); i++) {
      if (value[i] < '0' || value[i] > '9')
        return false;
      
      intval *= 10;
      intval += value[i] - '0';
    }

    // Apply sign and return
    result = sign * intval;
    return true;
  }
  
  
  bool Config::parseOptionValue(
    const std::string&  value,
          Tristate&     result) {
    static const std::array<std::pair<const char*, Tristate>, 3> s_lookup = {{
      { "true",  Tristate::True  },
      { "false", Tristate::False },
      { "auto",  Tristate::Auto  },
    }};

    return parseStringOption(value,
      s_lookup.begin(), s_lookup.end(), result);
  }


  template<typename I, typename V>
  bool Config::parseStringOption(
          std::string   str,
          I             begin,
          I             end,
          V&            value) {
    str = Config::toLower(str);

    for (auto i = begin; i != end; i++) {
      if (str == i->first) {
        value = i->second;
        return true;
      }
    }

    return false;
  }


  Config Config::getUserConfig() {
    Config config;

    // Load either $DXVK_CONFIG_FILE or $PWD/dxvk.conf
    std::string filePath = env::getEnvVar("DXVK_CONFIG_FILE");

    if (filePath == "")
      filePath = "dxvk.conf";
    
    // Open the file if it exists
    std::ifstream stream(str::tows(filePath.c_str()).c_str());

    if (!stream)
      return config;
    
    // Inform the user that we loaded a file, might
    // help when debugging configuration issues
    Logger::info(str::format("Found config file: ", filePath));

    // Initialize parser context
    ConfigContext ctx;
    ctx.active = true;

    // Parse the file line by line
    std::string line;

    while (std::getline(stream, line))
      parseUserConfigLine(config, ctx, line);
    
    return config;
  }


  void Config::logOptions() const {
    if (!m_options.empty()) {
      Logger::info("Effective configuration:");

      for (auto& pair : m_options)
        Logger::info(str::format("  ", pair.first, " = ", pair.second));
    }
  }

  std::string Config::toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
      [] (unsigned char c) { return (c >= 'A' && c <= 'Z') ? (c + 'a' - 'A') : c; });
    return str;
  }

}
