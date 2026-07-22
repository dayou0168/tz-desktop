#include "mtproto/mtproto_dc_options.h"

#include <QtCore/QDataStream>

namespace {

constexpr auto kMainDcId = 2;
constexpr auto kMainDcAddress = "tztg.tianze8.cc";
constexpr auto kMainDcPort = 2398;
constexpr auto kOldMainDcAddress = "47.79.233.204";

using DcOptions = MTP::DcOptions;
using DcId = MTP::DcId;
using Endpoint = DcOptions::Endpoint;
using Flag = DcOptions::Flag;
using Flags = DcOptions::Flags;
using Variants = DcOptions::Variants;

struct CachedEndpoint {
	DcId id = 0;
	Flags flags;
	QByteArray address;
	int port = 0;
	QByteArray secret;
};

auto SerializeVersion2(const std::vector<CachedEndpoint> &endpoints) {
	auto result = QByteArray();
	auto stream = QDataStream(&result, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_5_1);
	stream << qint32(-2) << qint32(endpoints.size());
	for (const auto &endpoint : endpoints) {
		stream << qint32(endpoint.id)
			<< qint32(endpoint.flags)
			<< qint32(endpoint.port)
			<< qint32(endpoint.address.size());
		stream.writeRawData(endpoint.address.constData(), endpoint.address.size());
		stream << qint32(endpoint.secret.size());
		stream.writeRawData(endpoint.secret.constData(), endpoint.secret.size());
	}
	stream << qint32(0);
	return result;
}

auto MakeOption(
		DcId id,
		Flags flags,
		const char *address,
		int port,
		QByteArray secret = {}) {
	return MTP_dcOption(
		MTP_flags(flags),
		MTP_int(id),
		MTP_string(address),
		MTP_int(port),
		MTP_bytes(std::move(secret)));
}

bool Matches(
		const Endpoint &endpoint,
		DcId id,
		Flags flags,
		const char *address,
		int port,
		const QByteArray &secret = {}) {
	return endpoint.id == id
		&& endpoint.flags.value() == flags.value()
		&& endpoint.ip == address
		&& endpoint.port == port
		&& endpoint.secret == bytes::make_vector(secret);
}

auto EndpointCount(const Variants &variants) {
	auto result = 0;
	for (const auto &byAddress : variants.data) {
		for (const auto &list : byAddress) {
			result += list.size();
		}
	}
	return result;
}

bool HasEndpoint(
		const Variants &variants,
		Variants::Address addressType,
		Variants::Protocol protocol,
		DcId id,
		Flags flags,
		const char *address,
		int port,
		const QByteArray &secret = {}) {
	for (const auto &endpoint : variants.data[addressType][protocol]) {
		if (Matches(endpoint, id, flags, address, port, secret)) {
			return true;
		}
	}
	return false;
}

bool IsFixedMainLookup(const DcOptions &options) {
	const auto variants = options.lookup(
		kMainDcId,
		MTP::DcType::Regular,
		false);
	const auto flags = Flag::f_static | Flag::f_tcpo_only;
	return EndpointCount(variants) == 1
		&& HasEndpoint(
			variants,
			Variants::IPv4,
			Variants::Tcp,
			kMainDcId,
			flags,
			kMainDcAddress,
			kMainDcPort);
}

bool TestBuiltInOptions() {
	const auto production = DcOptions(MTP::Environment::Production);
	const auto test = DcOptions(MTP::Environment::Test);
	return IsFixedMainLookup(production) && IsFixedMainLookup(test);
}

bool TestVersion2CacheMigration() {
	const auto oldFlags = Flag::f_static | Flag::f_tcpo_only;
	const auto otherFlags = Flags(Flag::f_tcpo_only);
	const auto serialized = SerializeVersion2({
		{ kMainDcId, oldFlags, kOldMainDcAddress, kMainDcPort, {} },
		{ 1, otherFlags, "192.0.2.1", 443, {} },
	});
	auto options = DcOptions(MTP::Environment::Production);
	if (!options.constructFromSerialized(serialized)
		|| !IsFixedMainLookup(options)) {
		return false;
	}
	const auto other = options.lookup(1, MTP::DcType::Regular, false);
	if (EndpointCount(other) != 1
		|| !HasEndpoint(
			other,
			Variants::IPv4,
			Variants::Tcp,
			1,
			otherFlags,
			"192.0.2.1",
			443)) {
		return false;
	}
	const auto migrated = options.serialize();
	if (migrated.contains(kOldMainDcAddress)) {
		return false;
	}
	auto reloaded = DcOptions(MTP::Environment::Production);
	return reloaded.constructFromSerialized(migrated)
		&& IsFixedMainLookup(reloaded);
}

bool TestServerOptionsNormalization() {
	const auto mediaSecret = QByteArray(16, 'm');
	const auto mainCdnSecret = QByteArray(16, 'd');
	const auto otherSecret = QByteArray(16, 'o');
	const auto cdnSecret = QByteArray(16, 'c');
	const auto mediaFlags = Flag::f_media_only | Flag::f_ipv6 | Flag::f_secret;
	const auto otherFlags = Flag::f_ipv6 | Flag::f_secret;
	const auto cdnFlags = Flag::f_cdn | Flag::f_ipv6 | Flag::f_secret;
	auto server = QVector<MTPDcOption>{
		MakeOption(kMainDcId, Flags(), kOldMainDcAddress, 80),
		MakeOption(kMainDcId, Flags(), "198.51.100.2", 443),
		MakeOption(
			kMainDcId,
			Flag::f_ipv6 | Flag::f_tcpo_only,
			"2001:db8::2",
			443),
		MakeOption(
			kMainDcId,
			Flag::f_secret,
			"198.51.100.3",
			444,
			QByteArray(16, 's')),
		MakeOption(
			kMainDcId,
			mediaFlags,
			"2001:db8::20",
			445,
			mediaSecret),
		MakeOption(
			kMainDcId,
			cdnFlags,
			"2001:db8::25",
			450,
			mainCdnSecret),
		MakeOption(4, otherFlags, "2001:db8::4", 446, otherSecret),
		MakeOption(5, cdnFlags, "2001:db8::5", 447, cdnSecret),
	};
	auto options = DcOptions(MTP::Environment::Production);
	options.setFromList(MTP_vector<MTPDcOption>(server));
	if (!IsFixedMainLookup(options)) {
		return false;
	}
	const auto media = options.lookup(
		kMainDcId,
		MTP::DcType::MediaCluster,
		false);
	if (!HasEndpoint(
			media,
			Variants::IPv6,
			Variants::Tcp,
			kMainDcId,
			mediaFlags,
			"2001:db8::20",
			445,
			mediaSecret)) {
		return false;
	}
	const auto mainCdn = options.lookup(kMainDcId, MTP::DcType::Cdn, false);
	if (EndpointCount(mainCdn) != 1
		|| !HasEndpoint(
			mainCdn,
			Variants::IPv6,
			Variants::Tcp,
			kMainDcId,
			cdnFlags,
			"2001:db8::25",
			450,
			mainCdnSecret)) {
		return false;
	}
	const auto other = options.lookup(4, MTP::DcType::Regular, false);
	if (EndpointCount(other) != 1
		|| !HasEndpoint(
			other,
			Variants::IPv6,
			Variants::Tcp,
			4,
			otherFlags,
			"2001:db8::4",
			446,
			otherSecret)) {
		return false;
	}
	const auto cdn = options.lookup(5, MTP::DcType::Cdn, false);
	if (EndpointCount(cdn) != 1
		|| !HasEndpoint(
			cdn,
			Variants::IPv6,
			Variants::Tcp,
			5,
			cdnFlags,
			"2001:db8::5",
			447,
			cdnSecret)) {
		return false;
	}

	const auto addedMediaFlags = Flag::f_media_only | Flag::f_tcpo_only;
	auto added = QVector<MTPDcOption>{
		MakeOption(kMainDcId, Flags(), "203.0.113.2", 448),
		MakeOption(
			kMainDcId,
			addedMediaFlags,
			"203.0.113.20",
			449),
	};
	options.addFromList(MTP_vector<MTPDcOption>(added));
	if (!IsFixedMainLookup(options)) {
		return false;
	}
	const auto mediaAfterAdd = options.lookup(
		kMainDcId,
		MTP::DcType::MediaCluster,
		false);
	return HasEndpoint(
			mediaAfterAdd,
			Variants::IPv6,
			Variants::Tcp,
			kMainDcId,
			mediaFlags,
			"2001:db8::20",
			445,
			mediaSecret)
		&& HasEndpoint(
			mediaAfterAdd,
			Variants::IPv4,
			Variants::Tcp,
			kMainDcId,
			addedMediaFlags,
			"203.0.113.20",
			449);
}

} // namespace

int main() {
	if (!TestBuiltInOptions()) {
		return 1;
	}
	if (!TestVersion2CacheMigration()) {
		return 2;
	}
	if (!TestServerOptionsNormalization()) {
		return 3;
	}
	return 0;
}
