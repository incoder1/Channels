#ifndef __IO_CONVERT_HPP_INCLUDED__
#define __IO_CONVERT_HPP_INCLUDED__

#if !defined(CONV_ENGINE_ICONV) && !defined(CONV_ENGINE_IBM_ICU) && !defined(CONV_ENGINE_MLANG)
#	error "Please define character set conversation engine"
#endif

// Character conversation engine libiconv
#ifdef CONV_ENGINE_ICONV // CONV_ENGINE_ICONV
#	include "iconv_conv.hpp"
#elif CONV_ENGINE_IBM_ICU // CONV_ENGINE_IBM_ICU
#	include "ibm_icu_conv.hpp"
#elif defined(CONV_ENGINE_MLANG) // CONV_ENGINE_MLANG
#	ifndef _WIN32
#		error "MS MLang engine only provided on Microsoft Windows with Internet Explorrer installed"
#	else
#		include "msmlang_conv.hpp"
#	endif // error when attempt to use Mlang not on windows
#endif // conversation egine select


#endif // __IO_CONVERT_HPP_INCLUDED__
