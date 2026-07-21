#pragma once

#include <QtCore/QStringView>

namespace GroupCreation {

enum class Mode {
	InvalidTitle,
	EmptyMegagroup,
	BasicGroupWithMembers,
};

struct Plan {
	Mode mode = Mode::InvalidTitle;
	int memberCount = 0;

	[[nodiscard]] bool createsGroup() const {
		return mode != Mode::InvalidTitle;
	}

	[[nodiscard]] bool invitesMembers() const {
		return mode == Mode::BasicGroupWithMembers;
	}
};

[[nodiscard]] inline bool HasValidTitle(QStringView title) {
	return !title.trimmed().isEmpty();
}

[[nodiscard]] inline Plan MakePlan(QStringView title, int memberCount) {
	if (!HasValidTitle(title) || memberCount < 0) {
		return {};
	}
	return {
		.mode = memberCount
			? Mode::BasicGroupWithMembers
			: Mode::EmptyMegagroup,
		.memberCount = memberCount,
	};
}

[[nodiscard]] inline bool IsInvitePrivacyError(
		QStringView error,
		int memberCount) {
	return memberCount > 0
		&& error.compare(QStringView(u"USER_PRIVACY_RESTRICTED")) == 0;
}

} // namespace GroupCreation
