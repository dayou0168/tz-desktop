#include "boxes/group_creation_contract.h"

#include <QtCore/QStringView>

int main() {
	const auto empty = GroupCreation::MakePlan(QStringView(u"Group"), 0);
	if (!empty.createsGroup()
			|| empty.invitesMembers()
			|| empty.mode != GroupCreation::Mode::EmptyMegagroup
			|| empty.memberCount != 0) {
		return 1;
	}

	const auto one = GroupCreation::MakePlan(QStringView(u"Group"), 1);
	if (!one.createsGroup()
			|| !one.invitesMembers()
			|| one.mode != GroupCreation::Mode::BasicGroupWithMembers
			|| one.memberCount != 1) {
		return 2;
	}

	const auto many = GroupCreation::MakePlan(QStringView(u"Group"), 3);
	if (!many.createsGroup()
			|| !many.invitesMembers()
			|| many.memberCount != 3) {
		return 3;
	}

	if (GroupCreation::MakePlan(QStringView(u" \t\n "), 0).createsGroup()) {
		return 4;
	}
	if (GroupCreation::MakePlan(QStringView(u" \t\n "), 2).createsGroup()) {
		return 5;
	}

	if (GroupCreation::IsInvitePrivacyError(
			QStringView(u"USER_PRIVACY_RESTRICTED"),
			0)) {
		return 6;
	}
	if (!GroupCreation::IsInvitePrivacyError(
			QStringView(u"USER_PRIVACY_RESTRICTED"),
			1)) {
		return 7;
	}
	if (GroupCreation::IsInvitePrivacyError(
			QStringView(u"USERS_TOO_FEW"),
			1)) {
		return 8;
	}

	return 0;
}
