#include "roboutils/util/locales.h"

using namespace RoboUtils;

template <int LC>
Locale<LC>::Locale(const char *loc)
 : loc_{std::setlocale(LC, nullptr)}
{
  std::setlocale(LC, loc);
}

template <int LC>
Locale<LC>::~Locale()
{
  std::setlocale(LC,loc_.c_str());
}

// Explicit instantiation
template class RoboUtils::Locale<LC_NUMERIC>;
template class RoboUtils::Locale<LC_TIME>;