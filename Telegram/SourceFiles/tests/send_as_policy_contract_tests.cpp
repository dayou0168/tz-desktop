#include "main/session/send_as_policy.h"

int main() {
	using Main::SendAsPolicy::CanChooseMessageIdentity;
	using Main::SendAsPolicy::DialogKind;

	if (CanChooseMessageIdentity(DialogKind::BasicGroup, true, 3)) {
		return 1;
	}
	if (CanChooseMessageIdentity(DialogKind::Megagroup, true, 3)) {
		return 2;
	}
	if (!CanChooseMessageIdentity(DialogKind::BroadcastChannel, true, 2)) {
		return 3;
	}
	if (CanChooseMessageIdentity(DialogKind::BroadcastChannel, false, 2)) {
		return 4;
	}
	if (CanChooseMessageIdentity(DialogKind::BroadcastChannel, true, 1)) {
		return 5;
	}
	if (CanChooseMessageIdentity(DialogKind::Other, true, 3)) {
		return 6;
	}

	return 0;
}
