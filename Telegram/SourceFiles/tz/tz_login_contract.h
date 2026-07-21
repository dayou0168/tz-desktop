#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringView>

#include <utility>

namespace Tz {

[[nodiscard]] inline QString NormalizeLoginPhone(QStringView phone) {
	auto result = QString();
	result.reserve(phone.size());
	for (const auto ch : phone) {
		if ((ch >= QChar('0')) && (ch <= QChar('9'))) {
			result.append(ch);
		}
	}
	return result;
}

[[nodiscard]] inline QByteArray ConsumeLoginChallenge(QByteArray &challenge) {
	return std::exchange(challenge, QByteArray());
}

} // namespace Tz
