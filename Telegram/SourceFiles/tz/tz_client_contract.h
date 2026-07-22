#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringView>

#include <algorithm>
#include <utility>

namespace Tz {

inline constexpr auto kProductName = u"TZ";
inline constexpr auto kCompanyName = u"天泽集团";
inline constexpr auto kCompanyAscii = u"tianze";
inline constexpr auto kInternalPublicUrl = u"https://tg.tianze8.cc/";
inline constexpr auto kVersion = u"1.0.3";
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

[[nodiscard]] inline bool IsVisibleBrandIdentifierCharacter(QChar value) {
	return value.isLetterOrNumber() || (value == '_');
}

[[nodiscard]] inline bool IsOfficialTelegramHost(QStringView host) {
	const auto matches = [&](QStringView expected) {
		return host.compare(expected, Qt::CaseInsensitive) == 0;
	};
	const auto isSubdomainOf = [&](QStringView expected) {
		return host.endsWith(expected, Qt::CaseInsensitive)
			&& (host.size() > expected.size())
			&& (host[host.size() - expected.size() - 1] == '.');
	};
	return matches(QStringView(u"t.me"))
		|| matches(QStringView(u"telegram.me"))
		|| isSubdomainOf(QStringView(u"telegram.me"))
		|| matches(QStringView(u"telegram.dog"))
		|| isSubdomainOf(QStringView(u"telegram.dog"))
		|| matches(QStringView(u"telegram.org"))
		|| isSubdomainOf(QStringView(u"telegram.org"))
		|| matches(QStringView(u"telegramdesktop.github.io"));
}

[[nodiscard]] inline QString InternalizeVisibleUrls(QString text) {
	auto offset = 0;
	while (offset < text.size()) {
		const auto http = text.indexOf(
			QStringView(u"http://"),
			offset,
			Qt::CaseInsensitive);
		const auto https = text.indexOf(
			QStringView(u"https://"),
			offset,
			Qt::CaseInsensitive);
		const auto start = (http < 0)
			? https
			: (https < 0)
			? http
			: std::min(http, https);
		if (start < 0) {
			break;
		}
		const auto schemeSize = text.mid(start).startsWith(
			QStringView(u"https://"),
			Qt::CaseInsensitive)
			? 8
			: 7;
		auto end = start + schemeSize;
		while (end < text.size()
			&& !text[end].isSpace()
			&& !QStringView(u"<>\"'()[]{}").contains(text[end])) {
			++end;
		}
		auto urlEnd = end;
		while (urlEnd > start
			&& QStringView(u".,;!?").contains(text[urlEnd - 1])) {
			--urlEnd;
		}
		auto hostEnd = start + schemeSize;
		while (hostEnd < urlEnd
			&& !QStringView(u"/:?#").contains(text[hostEnd])) {
			++hostEnd;
		}
		const auto host = QStringView(text).mid(
			start + schemeSize,
			hostEnd - start - schemeSize);
		if (IsOfficialTelegramHost(host)) {
			text.replace(
				start,
				urlEnd - start,
				QStringView(kInternalPublicUrl).toString());
			offset = start + QStringView(kInternalPublicUrl).size();
		} else {
			offset = end;
		}
	}
	return text;
}

[[nodiscard]] inline bool IsInsideVisibleUrl(
		QStringView text,
		int position) {
	const auto http = text.lastIndexOf(
		QStringView(u"http://"),
		position,
		Qt::CaseInsensitive);
	const auto https = text.lastIndexOf(
		QStringView(u"https://"),
		position,
		Qt::CaseInsensitive);
	const auto start = std::max(http, https);
	if (start < 0) {
		return false;
	}
	for (auto i = start; i != position; ++i) {
		if (text[i].isSpace()
			|| QStringView(u"<>\"'()[]{}").contains(text[i])) {
			return false;
		}
	}
	return true;
}

[[nodiscard]] inline QString VisibleBrandText(QString text) {
	text = InternalizeVisibleUrls(std::move(text));
	const auto replace = [&](QStringView brand) {
		auto searchFrom = 0;
		while (searchFrom < text.size()) {
			const auto position = text.indexOf(
				brand,
				searchFrom,
				Qt::CaseInsensitive);
			if (position < 0) {
				break;
			}
			const auto end = position + brand.size();
			const auto endIndex = static_cast<int>(end);
			const auto startsInsideIdentifier = (position > 0)
				&& IsVisibleBrandIdentifierCharacter(text[position - 1]);
			const auto endsInsideIdentifier = (end < text.size())
				&& IsVisibleBrandIdentifierCharacter(text[endIndex]);
			const auto isPlaceholder = (position > 0)
				&& (end < text.size())
				&& (text[position - 1] == '{')
				&& (text[endIndex] == '}');
			if (!startsInsideIdentifier
				&& !endsInsideIdentifier
				&& !isPlaceholder
				&& !IsInsideVisibleUrl(QStringView(text), position)) {
				text.replace(
					position,
					brand.size(),
					QStringView(kCompanyName).toString());
				searchFrom = position + QStringView(kCompanyName).size();
			} else {
				searchFrom = position + 1;
			}
		}
	};
	replace(QStringView(u"Telegram Desktop"));
	replace(QStringView(u"Telegram"));
	return text;
}

[[nodiscard]] inline QByteArray VisibleBrandUtf8(QByteArray text) {
	return VisibleBrandText(QString::fromUtf8(text)).toUtf8();
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
