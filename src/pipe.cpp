#include "prchdrs.h"
#include <pipe.hpp>

namespace io {

	Pipe::Pipe(PipeSinkRoutine routine) BOOST_NOEXCEPT_OR_NOTHROW:
		object(),
		sinkRoutine_(routine)
	{}

	void Pipe::call_sink_routine(const SWriteChannel& channel) const {
		sinkRoutine_(channel);
	}

	Pipe::~Pipe() BOOST_NOEXCEPT_OR_NOTHROW
	{}

}

