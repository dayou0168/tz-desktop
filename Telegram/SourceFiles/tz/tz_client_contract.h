#pragma once

#include <QtCore/QStringView>

namespace Tz {

inline constexpr auto kProductName = u"TZ";
inline constexpr auto kCompanyName = u"天泽集团";
inline constexpr auto kCompanyAscii = u"tianze";
inline constexpr auto kVersion = u"1.0.1";
inline constexpr auto kDefaultLanguageId = u"zh-hans";
inline constexpr auto kCompatibleLegacyStorageVersion = 6008004;

[[nodiscard]] inline bool SignupNamesAccepted(
		QStringView firstName,
		QStringView) {
	return !firstName.trimmed().isEmpty();
}

} // namespace Tz
