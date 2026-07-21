#include "tz/tz_internal_url.h"

#include <QtCore/QStringView>

namespace {

bool Rewrites(QStringView source, QStringView expected) {
	return Tz::RewriteInternalPublicUrlHost(source.toString()) == expected;
}

} // namespace

int main() {
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc/aze66"),
			QStringView(u"https://t.me/aze66"))) {
		return 1;
	}
	if (!Rewrites(
			QStringView(u"http://TG.TIANZE8.CC:80/aze66?ref=campaign#chat"),
			QStringView(u"http://t.me/aze66?ref=campaign#chat"))) {
		return 2;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc:443/aze66"),
			QStringView(u"https://t.me/aze66"))) {
		return 3;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc.evil.com/aze66"),
			QStringView(u"https://tg.tianze8.cc.evil.com/aze66"))) {
		return 4;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc@evil.com/aze66"),
			QStringView(u"https://tg.tianze8.cc@evil.com/aze66"))) {
		return 11;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc:444/aze66"),
			QStringView(u"https://tg.tianze8.cc:444/aze66"))) {
		return 5;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc./aze66"),
			QStringView(u"https://tg.tianze8.cc./aze66"))) {
		return 6;
	}
	if (!Rewrites(
			QStringView(u"https://user@tg.tianze8.cc/aze66"),
			QStringView(u"https://user@tg.tianze8.cc/aze66"))) {
		return 7;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc/"),
			QStringView(u"https://tg.tianze8.cc/"))) {
		return 8;
	}
	if (!Rewrites(
			QStringView(u"tz://join?invite=Abc_1-2.3"),
			QStringView(u"tz://join?invite=Abc_1-2.3"))) {
		return 9;
	}
	if (!Rewrites(
			QStringView(u"https://tg.tianze8.cc/aze66?profile=1"),
			QStringView(u"https://t.me/aze66?profile=1"))) {
		return 12;
	}
	const auto specialPaths = {
		QStringView(u"+Abc_1-2.3"),
		QStringView(u"joinchat/Abc_1-2.3"),
		QStringView(u"proxy?server=127.0.0.1&port=443"),
		QStringView(u"socks?server=127.0.0.1&port=1080"),
		QStringView(u"share/url?url=https%3A%2F%2Fexample.com"),
		QStringView(u"addstickers/example"),
	};
	for (const auto path : specialPaths) {
		if (!Rewrites(
				QStringView(u"https://tg.tianze8.cc/").toString()
					+ path.toString(),
				QStringView(u"https://t.me/").toString()
					+ path.toString())) {
			return 10;
		}
	}
	return 0;
}
