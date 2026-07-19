#pragma once

#include <QtCore/QString>
#include <QtCore/QStringView>

#include <algorithm>

namespace Tz {

inline constexpr auto kProductName = u"TZ";
inline constexpr auto kCompanyName = u"天泽集团";
inline constexpr auto kCompanyAscii = u"tianze";
inline constexpr auto kVersion = u"1.0.1";
inline constexpr auto kDefaultLanguageId = u"zh-hans";
inline constexpr auto kCompatibleLegacyStorageVersion = 6008004;
inline constexpr auto kLoginPasswordMinimumCodePoints = 8;
inline constexpr auto kLoginPasswordTitle = u"输入密码";
inline constexpr auto kLoginPasswordDescription = u"请输入账号密码。";
inline constexpr auto kLoginPasswordPlaceholder = u"密码";
inline constexpr auto kLoginPasswordTooShort = u"密码至少需要 8 个字符";
inline constexpr auto kLoginIdentityOrPasswordError = u"账号或密码错误";

[[nodiscard]] inline bool SignupNamesAccepted(
		QStringView firstName,
		QStringView) {
	return !firstName.trimmed().isEmpty();
}

[[nodiscard]] inline int UnicodeCodePointCount(QStringView value) {
	auto result = 0;
	for (auto i = 0; i != value.size(); ++i, ++result) {
		if (value[i].isHighSurrogate()
			&& (i + 1 != value.size())
			&& value[i + 1].isLowSurrogate()) {
			++i;
		}
	}
	return result;
}

[[nodiscard]] inline bool LoginPasswordAccepted(QStringView password) {
	return UnicodeCodePointCount(password) >= kLoginPasswordMinimumCodePoints;
}

[[nodiscard]] inline bool IsInviteHashCharacter(QChar value) {
	return ((value >= 'a') && (value <= 'z'))
		|| ((value >= 'A') && (value <= 'Z'))
		|| ((value >= '0') && (value <= '9'))
		|| (value == '.')
		|| (value == '_')
		|| (value == '-');
}

[[nodiscard]] inline QString ConvertInternalInviteUrl(QString url) {
	const auto convert = [&](QStringView prefix) -> QString {
		if (!QStringView(url).startsWith(prefix, Qt::CaseInsensitive)) {
			return QString();
		}
		auto hash = QStringView(url).mid(prefix.size());
		const auto delimiter = hash.indexOf(QChar('&'));
		const auto fragment = hash.indexOf(QChar('#'));
		const auto cut = (delimiter < 0)
			? fragment
			: (fragment < 0)
			? delimiter
			: std::min(delimiter, fragment);
		if (cut >= 0) {
			hash = hash.left(cut);
		}
		if (hash.isEmpty()
			|| !std::all_of(
				hash.begin(),
				hash.end(),
				&IsInviteHashCharacter)) {
			return QString();
		}
		return QStringView(u"tg://join?invite=").toString()
			+ hash.toString();
	};
	if (const auto local = convert(QStringView(u"tz://join?invite=")); !local.isEmpty()) {
		return local;
	}
	if (const auto local = convert(QStringView(u"https://tg.tianze8.cc/+")); !local.isEmpty()) {
		return local;
	}
	return url;
}

} // namespace Tz
