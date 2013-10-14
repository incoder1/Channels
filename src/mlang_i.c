

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 14 20:30:58 2013
 */
/* Compiler settings for mlang.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__
typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;
#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_MultiLanguage,0x275c23e0,0x3747,0x11d0,0x9f,0xea,0x00,0xaa,0x00,0x3f,0x86,0x46);


MIDL_DEFINE_GUID(IID, IID_IMLangStringBufW,0xD24ACD21,0xBA72,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangStringBufA,0xD24ACD23,0xBA72,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangString,0xC04D65CE,0xB70D,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangStringWStr,0xC04D65D0,0xB70D,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangStringAStr,0xC04D65D2,0xB70D,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(CLSID, CLSID_CMLangString,0xC04D65CF,0xB70D,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangLineBreakConsole,0xF5BE2EE1,0xBFD7,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IEnumCodePage,0x275c23e3,0x3747,0x11d0,0x9f,0xea,0x00,0xaa,0x00,0x3f,0x86,0x46);


MIDL_DEFINE_GUID(IID, IID_IEnumRfc1766,0x3dc39d1d,0xc030,0x11d0,0xb8,0x1b,0x00,0xc0,0x4f,0xc9,0xb3,0x1f);


MIDL_DEFINE_GUID(IID, IID_IEnumScript,0xAE5F1430,0x388B,0x11d2,0x83,0x80,0x00,0xC0,0x4F,0x8F,0x5D,0xA1);


MIDL_DEFINE_GUID(IID, IID_IMLangConvertCharset,0xd66d6f98,0xcdaa,0x11d0,0xb8,0x22,0x00,0xc0,0x4f,0xc9,0xb3,0x1f);


MIDL_DEFINE_GUID(CLSID, CLSID_CMLangConvertCharset,0xd66d6f99,0xcdaa,0x11d0,0xb8,0x22,0x00,0xc0,0x4f,0xc9,0xb3,0x1f);


MIDL_DEFINE_GUID(IID, IID_IMultiLanguage,0x275c23e1,0x3747,0x11d0,0x9f,0xea,0x00,0xaa,0x00,0x3f,0x86,0x46);


MIDL_DEFINE_GUID(IID, IID_IMultiLanguage2,0xDCCFC164,0x2B38,0x11d2,0xB7,0xEC,0x00,0xC0,0x4F,0x8F,0x5D,0x9A);


MIDL_DEFINE_GUID(IID, IID_IMLangCodePages,0x359F3443,0xBD4A,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangFontLink,0x359F3441,0xBD4A,0x11D0,0xB1,0x88,0x00,0xAA,0x00,0x38,0xC9,0x69);


MIDL_DEFINE_GUID(IID, IID_IMLangFontLink2,0xDCCFC162,0x2B38,0x11d2,0xB7,0xEC,0x00,0xC0,0x4F,0x8F,0x5D,0x9A);


MIDL_DEFINE_GUID(IID, IID_IMultiLanguage3,0x4e5868ab,0xb157,0x4623,0x9a,0xcc,0x6a,0x1d,0x9c,0xae,0xbe,0x04);


MIDL_DEFINE_GUID(CLSID, CLSID_CMultiLanguage,0x275c23e2,0x3747,0x11d0,0x9f,0xea,0x00,0xaa,0x00,0x3f,0x86,0x46);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



