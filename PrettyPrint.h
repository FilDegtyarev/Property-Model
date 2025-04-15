#pragma once
#include "PropertyModel.h"
#include <iostream>
#include <ostream>

namespace NSPropertyModel::detail {
class DebugStream {
  public:
	DebugStream(std::ostream& stream);

	template<typename... DataArgs, typename... ValueArgs, typename... OutputArgs>
	std::ostream& operator<<(const PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>& pm) {
		std::ostream& os = stream_.get();
		os << "\nProperty Model current state:\n";
		os << "-----------------------------\n";
		os << "Data:\n";
		int i = 0;
		((os << '\t' << pm->data_[i] << ": " << std::any_cast<DataArgs>(pm->variables_.at(pm->data_[i++])->value())
			 << '\n'),
		 ...);

		os << "Value:\n";
		i = 0;
		((os << '\t' << pm->value_[i] << ": " << std::any_cast<ValueArgs>(pm->variables_.at(pm->value_[i++])->value())
			 << '\n'),
		 ...);

		os << "Output:\n";
		i = 0;
		((os << '\t' << pm->output_[i] << ": "
			 << std::any_cast<OutputArgs>(pm->variables_.at(pm->output_[i++])->value()) << '\n'),
		 ...);
		os << "-----------------------------\n";
		return os;
	}
	std::ostream& operator<<(detail::PMRelease);

  private:
	std::reference_wrapper<std::ostream> stream_;
};

DebugStream operator<<(std::ostream& out, detail::PMDebug);

class ReleaseStream {
  public:
	ReleaseStream(std::ostream& os);

	template<typename... DataArgs, typename... ValueArgs, typename... OutputArgs>
	std::ostream& operator<<(const PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>& pm) {
		std::ostream& os = stream_.get();
		int i = 0;
		((os << pm->output_[i] << ": " << std::any_cast<OutputArgs>((pm->variables_.at(pm->output_[i++]))->value())
			 << '\n'),
		 ...);
		return os;
	}

  private:
	std::reference_wrapper<std::ostream> stream_;
};

ReleaseStream operator<<(std::ostream& out, detail::PMRelease);
} // namespace NSPropertyModel::detail
