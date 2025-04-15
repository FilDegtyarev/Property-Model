#include "PrettyPrint.h"
namespace NSPropertyModel::detail {
DebugStream::DebugStream(std::ostream& stream) : stream_(stream) {};

DebugStream operator<<(std::ostream& out, detail::PMDebug) {
	return DebugStream(out);
}

std::ostream& DebugStream::operator<<(detail::PMRelease) {
	return stream_.get();
}

ReleaseStream::ReleaseStream(std::ostream& os) : stream_(os) {};

ReleaseStream operator<<(std::ostream& out, detail::PMRelease) {
	return out;
}
} // namespace NSPropertyModel::detail
