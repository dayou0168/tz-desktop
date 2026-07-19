#include "tz/tz_client_contract.h"

#include <QtCore/QStringView>

int main() {
	if (Tz::SignupNamesAccepted(QStringView(u"   "), QStringView(u"可空"))) {
		return 1;
	}
	if (!Tz::SignupNamesAccepted(QStringView(u"名字"), QStringView())) {
		return 2;
	}
	if (!Tz::SignupNamesAccepted(QStringView(u" 名字 "), QStringView(u" 姓氏 "))) {
		return 3;
	}
	if (QStringView(Tz::kProductName) != QStringView(u"TZ")) {
		return 4;
	}
	if (QStringView(Tz::kCompanyName) != QStringView(u"天泽集团")) {
		return 5;
	}
	if (QStringView(Tz::kCompanyAscii) != QStringView(u"tianze")) {
		return 6;
	}
	if (QStringView(Tz::kVersion) != QStringView(u"1.0.1")) {
		return 7;
	}
	if (QStringView(Tz::kDefaultLanguageId) != QStringView(u"zh-hans")) {
		return 8;
	}
	return 0;
}
