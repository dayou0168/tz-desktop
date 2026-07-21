#pragma once

namespace Main::SendAsPolicy {

enum class DialogKind {
	Other,
	BasicGroup,
	Megagroup,
	BroadcastChannel,
};

[[nodiscard]] constexpr bool CanChooseMessageIdentity(
		DialogKind dialog,
		bool canSend,
		int availableIdentities) {
	return dialog == DialogKind::BroadcastChannel
		&& canSend
		&& availableIdentities > 1;
}

} // namespace Main::SendAsPolicy
