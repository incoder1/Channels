#ifndef CONVERT_HPP_INCLUDED
#define CONVERT_HPP_INCLUDED

#ifdef CONV_ENGINE_ICONV
	#include "iconv_conv.hpp"
#endif

#ifdef CONV_ENGINE_IBM_ICU
	#include "ibm_icu_conv.hpp"
#endif

#endif // CONVERT_HPP_INCLUDED
