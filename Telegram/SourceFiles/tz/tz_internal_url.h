#pragma once

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QUrl>

namespace Tz {

[[nodiscard]] inline QString RewriteInternalPublicUrlHost(QString url) {
	const auto parsed = QUrl(url, QUrl::StrictMode);
	if (!parsed.isValid() || !parsed.userInfo().isEmpty()) {
		return url;
	}
	const auto scheme = parsed.scheme();
	const auto http = !scheme.compare(
		QStringView(u"http"),
		Qt::CaseInsensitive);
	const auto https = !scheme.compare(
		QStringView(u"https"),
		Qt::CaseInsensitive);
	if ((!http && !https)
		|| parsed.host().compare(
			QStringView(u"tg.tianze8.cc"),
			Qt::CaseInsensitive)
		|| (parsed.path().size() <= 1)) {
		return url;
	}
	const auto port = parsed.port(-1);
	if ((port != -1) && (port != (https ? 443 : 80))) {
		return url;
	}

	auto result = parsed;
	result.setHost(QStringView(u"t.me").toString());
	result.setPort(-1);
	return result.toString(QUrl::FullyEncoded);
}

} // namespace Tz
