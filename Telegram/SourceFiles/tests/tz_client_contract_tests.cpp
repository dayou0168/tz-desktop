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
	if (Tz::LoginPasswordAccepted(QStringView(u"1234567"))) {
		return 9;
	}
	if (!Tz::LoginPasswordAccepted(QStringView(u"12345678"))) {
		return 10;
	}
	if (!Tz::LoginPasswordAccepted(QStringView(u"密码密码密码密码密码密码密码密码"))) {
		return 11;
	}
	if (Tz::LoginPasswordAccepted(QStringView(u"😀😀😀😀😀😀😀"))) {
		return 12;
	}
	if (!Tz::LoginPasswordAccepted(QStringView(u"😀😀😀😀😀😀😀😀"))) {
		return 13;
	}
	if (Tz::ConvertInternalInviteUrl(
			QStringView(u"tz://join?invite=Abc_1-2.3").toString())
			!= QStringView(u"tg://join?invite=Abc_1-2.3").toString()) {
		return 14;
	}
	if (Tz::ConvertInternalInviteUrl(
			QStringView(u"https://tg.tianze8.cc/+Abc_1-2.3").toString())
			!= QStringView(u"tg://join?invite=Abc_1-2.3").toString()) {
		return 15;
	}
	if (Tz::ConvertInternalInviteUrl(
			QStringView(u"https://example.com/+Abc_1-2.3").toString())
			!= QStringView(u"https://example.com/+Abc_1-2.3").toString()) {
		return 16;
	}
	if (Tz::ConvertInternalInviteUrl(
			QStringView(u"tz://join?invite=bad/value").toString())
			!= QStringView(u"tz://join?invite=bad/value").toString()) {
		return 17;
	}
	if (Tz::VisibleBrandText(
			QStringView(u"Telegram Desktop / Telegram / telegram").toString())
			!= QStringView(u"天泽集团 / 天泽集团 / 天泽集团").toString()) {
		return 18;
	}
	if (Tz::VisibleBrandText(
			QStringView(u"Telegram Premium").toString())
			!= QStringView(u"天泽集团 Premium").toString()) {
		return 19;
	}
	if (Tz::VisibleBrandText(
			QStringView(u"https://telegram.org/faq").toString())
			!= QStringView(Tz::kInternalPublicUrl).toString()) {
		return 20;
	}
	if (Tz::VisibleBrandText(
			QStringView(u"Read https://telegram.org/faq.").toString())
			!= QStringView(u"Read https://tg.tianze8.cc/.").toString()) {
		return 21;
	}
	if (Tz::VisibleBrandText(
			QStringView(u"https://example.com/Telegram").toString())
			!= QStringView(u"https://example.com/Telegram").toString()) {
		return 22;
	}
	if (Tz::VisibleBrandText(
			QStringView(
				u"TelegramWebviewProxy TelegramBeta_ close_telegram").toString())
			!= QStringView(
				u"TelegramWebviewProxy TelegramBeta_ close_telegram").toString()) {
		return 23;
	}
	if (Tz::VisibleBrandText(QStringView(u"Hide {telegram}").toString())
			!= QStringView(u"Hide {telegram}").toString()) {
		return 24;
	}
	return 0;
}
