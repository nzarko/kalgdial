#include "kalgdialview.h"
#include "kalgdial.h"
#include "pppstats.h"
#include "utils.h"

#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qtimer.h>
#include <qcolor.h>

#include <kapplication.h>
#include <kpushbutton.h>
#include <ktextbrowser.h>
#include <kaction.h>
#include <kstdaction.h>
#include <ktextbrowser.h>
#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>
#include <krun.h>
#include <kprocio.h>

static const unsigned char img0_kzdialviewbase[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x22,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x3a, 0x47, 0x0b, 0xc2, 0x00, 0x00, 0x06,
	0x67, 0x49, 0x44, 0x41, 0x54, 0x58, 0x85, 0xcd, 0x97, 0x51, 0x6c, 0x1c,
	0x47, 0x19, 0xc7, 0x7f, 0x3e, 0x8f, 0xed, 0xe9, 0x79, 0xe3, 0x8c, 0xed,
	0x4d, 0xb2, 0x8d, 0x4e, 0xe9, 0x36, 0x3a, 0xa1, 0x23, 0xba, 0x84, 0x2b,
	0x0a, 0x6d, 0x84, 0x4c, 0x14, 0xf1, 0x50, 0x9d, 0x82, 0x0b, 0x16, 0x8d,
	0xc0, 0x11, 0xa9, 0x30, 0x95, 0x8b, 0x2c, 0xe8, 0x43, 0x1e, 0xfa, 0x60,
	0xa4, 0x3c, 0xa4, 0x88, 0x87, 0x80, 0x10, 0x8a, 0x2a, 0x24, 0x2c, 0x64,
	0x89, 0x3c, 0x04, 0xa8, 0xa0, 0x48, 0x46, 0x8a, 0xa0, 0x88, 0xa8, 0x32,
	0x28, 0x42, 0xa6, 0xb8, 0xcd, 0x2a, 0x3a, 0x89, 0x25, 0xb9, 0x90, 0x55,
	0x7a, 0x0d, 0xdb, 0x66, 0xb1, 0xb7, 0xee, 0xc6, 0x8c, 0xcf, 0x73, 0x77,
	0x3c, 0x5c, 0x1d, 0xe2, 0xc6, 0xf6, 0x39, 0x46, 0x15, 0x7c, 0x2f, 0xbb,
	0x33, 0x9a, 0x19, 0xfd, 0xe6, 0x3f, 0xdf, 0xf7, 0xcd, 0x7c, 0xf0, 0x7f,
	0x62, 0x6d, 0x5b, 0x99, 0x94, 0x4a, 0x89, 0xc6, 0x5a, 0xfd, 0xf5, 0xba,
	0xd9, 0xd2, 0x7a, 0x0f, 0x0c, 0xb2, 0x02, 0xf0, 0xe3, 0x57, 0x03, 0xe6,
	0xa2, 0x18, 0xa3, 0x35, 0x71, 0x02, 0x09, 0x01, 0x16, 0x2e, 0xdf, 0x3f,
	0x79, 0x68, 0xcb, 0x40, 0x9b, 0x9a, 0xb0, 0x02, 0xf0, 0xd2, 0xf9, 0x8b,
	0xa4, 0x6d, 0x0b, 0x63, 0x72, 0xf4, 0x59, 0x92, 0x58, 0x6b, 0xfa, 0x54,
	0x8c, 0x4c, 0x3c, 0x62, 0x21, 0xb9, 0x58, 0x0a, 0xf1, 0x3d, 0x8f, 0x99,
	0xc9, 0xb3, 0x0f, 0x0c, 0x24, 0x36, 0x03, 0xf1, 0xd2, 0xf9, 0x8b, 0xf4,
	0x88, 0x88, 0x42, 0x3e, 0x83, 0x11, 0x8a, 0xc4, 0x08, 0x96, 0x0c, 0x64,
	0x84, 0x64, 0x11, 0x8d, 0x1f, 0xc4, 0xa8, 0x8c, 0x4b, 0x14, 0x68, 0x2c,
	0x47, 0x51, 0x18, 0x1d, 0x43, 0xc7, 0x21, 0xfe, 0x2b, 0x53, 0x8d, 0xcd,
	0xc2, 0xb4, 0xb7, 0x82, 0x18, 0x3f, 0x3b, 0xce, 0x13, 0x8f, 0xef, 0xe1,
	0xd3, 0x87, 0x8a, 0xd8, 0x3b, 0x14, 0x69, 0x4b, 0x22, 0x48, 0xa1, 0xfa,
	0x02, 0xae, 0x87, 0x1a, 0xd5, 0x63, 0x51, 0x4f, 0x1b, 0x76, 0x28, 0x89,
	0xae, 0x06, 0xd8, 0x69, 0x87, 0xe1, 0xcf, 0x16, 0x99, 0x4f, 0xa5, 0x50,
	0xfb, 0x72, 0xbc, 0xf5, 0xe7, 0xd7, 0x4f, 0x37, 0x1a, 0xf5, 0x17, 0xff,
	0x6b, 0x45, 0xf2, 0x07, 0x8b, 0x64, 0x33, 0x2e, 0x7d, 0xb2, 0xd9, 0x4e,
	0xb7, 0x83, 0xdc, 0x01, 0xd5, 0x46, 0x06, 0x77, 0x77, 0x44, 0x52, 0x0d,
	0x49, 0xab, 0x00, 0x50, 0x14, 0x0a, 0x92, 0x92, 0xef, 0xf3, 0x27, 0xbf,
	0x8c, 0x90, 0x09, 0xb6, 0xf4, 0x36, 0x23, 0x46, 0x73, 0xd3, 0x1b, 0xa9,
	0xf1, 0xfc, 0x0f, 0x43, 0x74, 0xa5, 0x80, 0x4e, 0x14, 0x73, 0xba, 0xd9,
	0xbf, 0x98, 0x68, 0xc2, 0xb7, 0x02, 0xae, 0x94, 0x3e, 0xd8, 0x83, 0x0c,
	0xb0, 0x12, 0x85, 0x85, 0x22, 0x34, 0x3e, 0x95, 0x38, 0xa4, 0x54, 0x11,
	0x28, 0x9d, 0x43, 0x69, 0x97, 0xa3, 0x43, 0xc5, 0x75, 0xa3, 0xec, 0x5e,
	0xdb, 0x50, 0x11, 0xc7, 0x96, 0x58, 0x76, 0xf3, 0xbf, 0x9a, 0x24, 0xdc,
	0xd4, 0x16, 0x26, 0x81, 0x25, 0x5c, 0xa4, 0x8a, 0x49, 0x52, 0x1e, 0x95,
	0xb2, 0xc4, 0x2f, 0xe7, 0x38, 0x50, 0x08, 0x90, 0xda, 0x25, 0xa3, 0x42,
	0x48, 0x62, 0x2c, 0xa1, 0xd9, 0x93, 0xc9, 0xd1, 0x2f, 0x2c, 0x7e, 0xb3,
	0x09, 0x45, 0xd6, 0xf4, 0x91, 0x54, 0x4a, 0x34, 0x06, 0x46, 0x67, 0xe8,
	0xe9, 0x74, 0x78, 0x2c, 0xd7, 0xc9, 0x7c, 0x55, 0x20, 0xe8, 0x64, 0xee,
	0x0e, 0x2c, 0xd6, 0x04, 0x0b, 0x0b, 0x60, 0x08, 0x49, 0x63, 0x58, 0xac,
	0x66, 0xd1, 0x75, 0xc5, 0xdb, 0x73, 0xf0, 0xc7, 0xe9, 0x2c, 0x71, 0xe2,
	0xd2, 0x23, 0x0c, 0x87, 0x0f, 0x48, 0xba, 0xa4, 0xe0, 0xb2, 0x7f, 0x95,
	0x27, 0x0a, 0x07, 0xb9, 0x7c, 0xf9, 0xca, 0x86, 0xbe, 0xb2, 0xae, 0x22,
	0x52, 0x07, 0xa4, 0x01, 0xbf, 0x9c, 0x43, 0x58, 0xd0, 0xb0, 0x25, 0x6d,
	0x12, 0xb6, 0x09, 0x30, 0x06, 0xda, 0x8c, 0x8b, 0xb2, 0x34, 0x8e, 0xed,
	0x31, 0x78, 0x10, 0xc2, 0xb8, 0x80, 0x1e, 0x88, 0x09, 0xc3, 0x90, 0x52,
	0x20, 0xf1, 0x4a, 0x97, 0x10, 0xda, 0xa6, 0x4f, 0x42, 0x25, 0x08, 0x5b,
	0x2a, 0xb2, 0x2e, 0x88, 0x50, 0x12, 0x23, 0x20, 0x09, 0x35, 0xc6, 0x6e,
	0xfe, 0x0b, 0xa3, 0xc9, 0x39, 0x92, 0x05, 0x0d, 0x7b, 0x77, 0x87, 0x48,
	0x19, 0x60, 0x4b, 0x20, 0x09, 0xb1, 0xf5, 0x34, 0xc6, 0x84, 0x44, 0x81,
	0x47, 0x12, 0xc4, 0xc4, 0xc6, 0xe1, 0x83, 0x53, 0xc5, 0xb6, 0x9d, 0xad,
	0x83, 0x14, 0x0f, 0x6a, 0x2c, 0xcb, 0xa7, 0x1c, 0xba, 0x04, 0x3e, 0x64,
	0xb2, 0x00, 0x92, 0x47, 0x33, 0x90, 0xb2, 0x34, 0xb9, 0x9d, 0x1e, 0x29,
	0x2a, 0x00, 0x44, 0x95, 0xe6, 0x57, 0x58, 0x0e, 0x96, 0x05, 0x58, 0x8a,
	0x8a, 0xa7, 0x31, 0x22, 0x24, 0xb4, 0x34, 0x54, 0x16, 0xb7, 0x0e, 0xf2,
	0xaa, 0xe7, 0x31, 0x78, 0x24, 0xc7, 0x27, 0xf3, 0x15, 0x16, 0x75, 0x8e,
	0x4a, 0xb9, 0x19, 0x36, 0xff, 0x70, 0x24, 0xda, 0x40, 0x2a, 0x5f, 0xa1,
	0x13, 0x8b, 0xaa, 0xd0, 0x04, 0x61, 0xcc, 0xa5, 0x52, 0x80, 0xa9, 0x80,
	0xb2, 0x35, 0xd9, 0x7c, 0x8e, 0x30, 0x6f, 0x23, 0x42, 0x43, 0x74, 0xd5,
	0xc3, 0xa6, 0x73, 0xeb, 0x20, 0x26, 0x99, 0x66, 0xf2, 0x42, 0xcc, 0xe0,
	0x40, 0xcc, 0xc0, 0x91, 0x84, 0xab, 0x7e, 0x81, 0x4a, 0x20, 0x51, 0x16,
	0x24, 0x42, 0x73, 0x4b, 0x5b, 0xe8, 0xc8, 0xe3, 0x95, 0x0b, 0x11, 0x97,
	0x2f, 0xde, 0x66, 0x7f, 0xe1, 0x31, 0x1c, 0x77, 0x37, 0xde, 0xec, 0x9b,
	0xdc, 0xbe, 0x74, 0x83, 0x3b, 0x8e, 0x87, 0x59, 0x54, 0xec, 0xee, 0x51,
	0x44, 0xa5, 0xa0, 0x25, 0xc8, 0xba, 0xe9, 0x37, 0x95, 0x12, 0x8d, 0xe1,
	0x17, 0x8a, 0x64, 0x0a, 0x0e, 0x76, 0x36, 0x24, 0x2c, 0x0f, 0x13, 0x07,
	0xc3, 0x1c, 0x1d, 0x80, 0x2b, 0xa1, 0x47, 0xe2, 0x9f, 0xe3, 0xc6, 0xec,
	0x0d, 0x3e, 0x37, 0xf8, 0x34, 0x4f, 0x16, 0x9f, 0x24, 0x8e, 0x23, 0x8c,
	0xa9, 0x61, 0xdb, 0xbb, 0x08, 0x2b, 0x21, 0xbf, 0x9c, 0x3a, 0x4f, 0xd9,
	0xbf, 0xc6, 0xaf, 0x2f, 0xfc, 0x0e, 0x68, 0x7d, 0xef, 0x6c, 0x98, 0x47,
	0x94, 0x54, 0x94, 0x2e, 0xce, 0x60, 0x87, 0x0e, 0xbe, 0x97, 0xc3, 0x92,
	0xf0, 0x8b, 0x70, 0x8a, 0xe5, 0xe0, 0x27, 0x1c, 0x3f, 0x76, 0x9c, 0xe3,
	0xc5, 0x11, 0x00, 0x82, 0xc0, 0xbf, 0x3b, 0xe7, 0xbd, 0x78, 0x9e, 0xed,
	0xaa, 0x97, 0x67, 0x4e, 0x8c, 0xb0, 0xa4, 0x0d, 0xc0, 0x5d, 0x98, 0x2d,
	0x29, 0xb2, 0xa2, 0xca, 0xd8, 0xa9, 0x61, 0xb4, 0x86, 0x4a, 0xe9, 0x10,
	0xdd, 0xe2, 0xb7, 0x7c, 0x6b, 0xfc, 0x45, 0x84, 0x6c, 0x23, 0x49, 0xe6,
	0x5b, 0x2e, 0x5e, 0x33, 0x60, 0x59, 0x8a, 0x47, 0x1e, 0x75, 0x71, 0x76,
	0x35, 0x23, 0x67, 0x3d, 0x65, 0x5a, 0x82, 0x0c, 0x14, 0x8f, 0xd0, 0x2f,
	0xba, 0x78, 0x66, 0xe4, 0x6b, 0x64, 0x1c, 0x17, 0x6d, 0xe2, 0x96, 0x00,
	0x1f, 0x86, 0x01, 0xd8, 0xae, 0x7a, 0xe9, 0xb6, 0xb6, 0xf3, 0xf1, 0x5c,
	0x6e, 0x4d, 0xa0, 0x0d, 0x7d, 0xe4, 0xf1, 0x23, 0x87, 0xf8, 0xc1, 0x77,
	0xbe, 0x87, 0x94, 0x5d, 0x9b, 0x52, 0x60, 0x23, 0x13, 0x22, 0x4d, 0xbf,
	0xbd, 0x93, 0x85, 0x38, 0xc6, 0x2f, 0xfb, 0x7c, 0xf5, 0xc4, 0xc8, 0x2a,
	0x98, 0xfb, 0x7c, 0x64, 0xe5, 0x82, 0x9a, 0x9a, 0x9a, 0xc2, 0xb6, 0xfb,
	0xe8, 0x10, 0x9d, 0x5b, 0x86, 0xa8, 0x19, 0x68, 0x17, 0xe0, 0xba, 0x39,
	0x6a, 0x66, 0x89, 0x6b, 0xe5, 0x1b, 0x5c, 0x29, 0x79, 0xa4, 0x65, 0xfa,
	0x7e, 0xd0, 0x7b, 0x1b, 0xfb, 0xf6, 0xe5, 0x1b, 0x27, 0x46, 0xbe, 0xc2,
	0xf1, 0x63, 0x5f, 0xa2, 0x7b, 0xdb, 0x76, 0xe6, 0xe7, 0x63, 0x6e, 0x55,
	0x02, 0xda, 0x5b, 0x3e, 0x16, 0xd6, 0x06, 0xe8, 0xb7, 0x6d, 0xfa, 0x1c,
	0x97, 0x6b, 0xa5, 0x59, 0xae, 0x97, 0x6f, 0x32, 0x1f, 0xcf, 0xd1, 0x25,
	0x3b, 0xe8, 0x92, 0xf7, 0xe7, 0x95, 0x55, 0x47, 0x93, 0xd9, 0xe3, 0x36,
	0xa4, 0x14, 0xf4, 0x2a, 0xc5, 0xd8, 0xd8, 0x18, 0x4f, 0x3d, 0x35, 0x84,
	0x59, 0xd6, 0x84, 0xe1, 0x3b, 0x2d, 0x55, 0x59, 0xf1, 0x85, 0x76, 0x01,
	0x96, 0xd5, 0x4b, 0xaf, 0x52, 0xdc, 0x0e, 0xe7, 0xf9, 0xc3, 0xa5, 0x69,
	0xba, 0x64, 0xc7, 0x7f, 0x76, 0x2e, 0x9a, 0x20, 0xcf, 0x8d, 0x8e, 0xad,
	0x3a, 0x9a, 0x55, 0xb7, 0x6f, 0x4f, 0xbf, 0x7d, 0x3a, 0x7f, 0x20, 0xc7,
	0x67, 0x0e, 0x1f, 0xe6, 0xe5, 0x9f, 0xfd, 0x9c, 0x5b, 0xb7, 0x02, 0x1e,
	0x76, 0x32, 0xe4, 0xf3, 0x9f, 0xa0, 0x56, 0x4f, 0xf1, 0xcf, 0x28, 0xa6,
	0x0d, 0x48, 0xdd, 0xf3, 0x8a, 0xa9, 0x19, 0xa8, 0x56, 0x61, 0xa9, 0xba,
	0x4c, 0xaf, 0xea, 0x27, 0x93, 0x71, 0x79, 0xf7, 0xdd, 0x0a, 0x6f, 0xbc,
	0xe9, 0xf1, 0xb7, 0xb2, 0x4f, 0x57, 0x67, 0x07, 0x55, 0xb3, 0x4c, 0xad,
	0x5e, 0xa7, 0xab, 0x53, 0xb2, 0x4d, 0xf5, 0xf0, 0xec, 0xc8, 0xe8, 0xc6,
	0xce, 0x9a, 0xd9, 0xe3, 0x36, 0xbe, 0x3e, 0x34, 0xc8, 0xb8, 0xd6, 0x8c,
	0x5e, 0x9a, 0xe1, 0xaa, 0xa3, 0xa8, 0x25, 0x9a, 0x2f, 0x0f, 0x0f, 0xf3,
	0xd4, 0xe0, 0x10, 0x0f, 0x49, 0x49, 0x14, 0xbd, 0x43, 0x14, 0xcd, 0xb3,
	0x6c, 0x96, 0xa9, 0x99, 0x1a, 0x52, 0x4a, 0xa4, 0x04, 0x27, 0xe3, 0xf2,
	0x76, 0x10, 0xf0, 0x86, 0xe7, 0xf1, 0x5e, 0xb2, 0xc0, 0x43, 0xa2, 0x13,
	0x44, 0x3b, 0x1d, 0x74, 0xd0, 0x26, 0x20, 0x25, 0x25, 0x2f, 0x3c, 0x7f,
	0x12, 0x58, 0x3b, 0x84, 0x57, 0x75, 0xec, 0xdb, 0x97, 0x6f, 0xec, 0xb1,
	0x15, 0xcf, 0x56, 0x42, 0x66, 0xf6, 0xba, 0x9c, 0xf8, 0xe2, 0x10, 0x93,
	0x93, 0x3f, 0xc5, 0xb7, 0xbb, 0x79, 0x3f, 0x8e, 0x79, 0x6e, 0xe4, 0x24,
	0x27, 0x46, 0x8e, 0x11, 0x45, 0x11, 0xd7, 0xcb, 0x3e, 0x96, 0xa5, 0xb0,
	0xed, 0x5e, 0xaa, 0xa6, 0xc1, 0xeb, 0xb3, 0x33, 0xdc, 0x28, 0xff, 0x9d,
	0x65, 0x0c, 0x1d, 0x08, 0x96, 0x31, 0x4d, 0x18, 0xe0, 0xd4, 0xa9, 0xd3,
	0xeb, 0x02, 0xac, 0x09, 0x02, 0x90, 0xd9, 0x9b, 0x6d, 0x7c, 0x5e, 0xed,
	0x40, 0xf6, 0x75, 0x33, 0x7e, 0x33, 0x60, 0xc2, 0x68, 0xb2, 0xe3, 0xe3,
	0x94, 0xa6, 0x2e, 0xf0, 0x9a, 0x4e, 0x38, 0x90, 0xcd, 0xf1, 0x85, 0xa1,
	0xa7, 0xd9, 0x9f, 0xdf, 0x0f, 0xc0, 0xec, 0xec, 0x0c, 0xbe, 0xef, 0xa3,
	0xb5, 0xe6, 0x5f, 0x3a, 0x41, 0x08, 0xc1, 0x92, 0xd6, 0xb4, 0x0b, 0x81,
	0x10, 0x82, 0xef, 0x9e, 0x39, 0xbb, 0xa9, 0xb2, 0x62, 0xcd, 0x01, 0x2b,
	0x21, 0x3c, 0x56, 0xf8, 0x14, 0x51, 0xf6, 0x61, 0xce, 0x88, 0x34, 0xc1,
	0xf4, 0x34, 0xbf, 0x1a, 0x2a, 0xf2, 0x7e, 0x39, 0xe0, 0xaf, 0x71, 0xcc,
	0xae, 0x8c, 0xc3, 0x23, 0x8e, 0x43, 0x97, 0x90, 0x20, 0xa0, 0x5d, 0x08,
	0x6a, 0xc6, 0xd0, 0x2e, 0x04, 0xdb, 0xac, 0x3e, 0x4e, 0x9f, 0xfe, 0x76,
	0x4b, 0x15, 0x5a, 0x82, 0xac, 0x05, 0xa4, 0x9d, 0x6e, 0x8e, 0x15, 0x8b,
	0xe4, 0x27, 0xce, 0x31, 0xa1, 0x14, 0xbf, 0x37, 0x9a, 0xe2, 0xe0, 0x51,
	0xee, 0xc4, 0x73, 0x77, 0xc7, 0x77, 0x49, 0xc9, 0x42, 0x1c, 0xf3, 0xa3,
	0x89, 0x73, 0x0f, 0x5c, 0xed, 0x6d, 0x98, 0x21, 0x56, 0x16, 0x9b, 0xf0,
	0xfe, 0xd2, 0x00, 0x70, 0x62, 0x8d, 0x05, 0xa0, 0x2c, 0x88, 0x34, 0x6d,
	0xa6, 0x7e, 0x57, 0x89, 0x24, 0xd1, 0x9c, 0x3d, 0x3b, 0xb1, 0x6a, 0xde,
	0x83, 0xd8, 0xba, 0xe5, 0xc4, 0x87, 0x81, 0xea, 0x75, 0xd3, 0x76, 0x66,
	0x66, 0x86, 0x97, 0xe5, 0x36, 0x7c, 0x65, 0x71, 0x27, 0x69, 0xfa, 0x04,
	0xc0, 0x92, 0xd6, 0x4c, 0x4e, 0x9e, 0xbb, 0x3b, 0xee, 0x41, 0x21, 0xb6,
	0x64, 0x7d, 0x3b, 0xed, 0x46, 0x2a, 0x25, 0x1a, 0xd9, 0x8f, 0x65, 0x1b,
	0xdf, 0xfc, 0xc6, 0x68, 0x23, 0x95, 0x12, 0x8d, 0xcd, 0xd4, 0x2d, 0x1f,
	0x99, 0xfd, 0xcf, 0x01, 0x3e, 0x2a, 0xfb, 0x37, 0x19, 0xfb, 0xe5, 0xdd,
	0xb2, 0xfb, 0x72, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44,
	0xae, 0x42, 0x60, 0x82
};

extern kalgdial* p_kppp;

KAlgDialView::KAlgDialView(QWidget *parent, const char *name, WFlags fl)
	: QWidget( parent, name, fl )
{
	//Initialize UI
	QImage img;
	img.loadFromData( img0_kzdialviewbase, sizeof( img0_kzdialviewbase ), "PNG" );
	image0 = img;
	if ( !name )
		setName( "KAlgDialView" );
	setIcon( image0 );
	KAlgDialViewBaseLayout = new QVBoxLayout( this, 0, 6, "KAlgDialViewLayout"); 

	frame4 = new QFrame( this, "frame4" );
	frame4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, frame4->sizePolicy().hasHeightForWidth() ) );
	frame4->setFrameShape( QFrame::PopupPanel );
	frame4->setFrameShadow( QFrame::Sunken );
	frame4->setLineWidth( 2 );
	frame4->setMargin( 0 );
	frame4Layout = new QVBoxLayout( frame4, 11, 6, "frame4Layout"); 

	m_textInfo = new KTextBrowser( frame4, "m_textInfo" );
	frame4Layout->addWidget( m_textInfo );

	layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

	textLabel2 = new QLabel( frame4, "textLabel2" );
	layout7->addWidget( textLabel2 );

	m_timeShow = new QLabel( frame4, "m_timeShow" );
	m_timeShow->setAlignment( int( QLabel::AlignCenter ) );
	layout7->addWidget( m_timeShow );
	frame4Layout->addLayout( layout7 );

	layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

	textLabel3 = new QLabel( frame4, "textLabel3" );
	layout9->addWidget( textLabel3 );

	m_speedInfo = new QLabel( frame4, "m_speedInfo" );
	m_speedInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout9->addWidget( m_speedInfo );
	frame4Layout->addLayout( layout9 );

	layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

	label4 = new QLabel( frame4, "label4" );
	layout10->addWidget( label4 );

	m_ibytesInfo = new QLabel( frame4, "m_ibytesInfo" );
	m_ibytesInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout10->addWidget( m_ibytesInfo );
	frame4Layout->addLayout( layout10 );

	layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

	textLabel5 = new QLabel( frame4, "textLabel5" );
	layout11->addWidget( textLabel5 );

	m_obytesInfo = new QLabel( frame4, "m_obytesInfo" );
	m_obytesInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout11->addWidget( m_obytesInfo );
	frame4Layout->addLayout( layout11 );

	layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

	textLabel6 = new QLabel( frame4, "textLabel6" );
	layout12->addWidget( textLabel6 );

	m_tbytesInfo = new QLabel( frame4, "m_tbytesInfo" );
	m_tbytesInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout12->addWidget( m_tbytesInfo );
	frame4Layout->addLayout( layout12 );

	layout7_2 = new QHBoxLayout( 0, 0, 6, "layout7_2"); 

	textLabel1 = new QLabel( frame4, "textLabel1" );
	layout7_2->addWidget( textLabel1 );

	m_ipacketsInfo = new QLabel( frame4, "m_ipacketsInfo" );
	m_ipacketsInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout7_2->addWidget( m_ipacketsInfo );
	frame4Layout->addLayout( layout7_2 );

	layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

	textLabel3_2 = new QLabel( frame4, "textLabel3_2" );
	layout8->addWidget( textLabel3_2 );

	m_opacketsInfo = new QLabel( frame4, "m_opacketsInfo" );
	m_opacketsInfo->setAlignment( int( QLabel::AlignCenter ) );
	layout8->addWidget( m_opacketsInfo );
	frame4Layout->addLayout( layout8 );
	KAlgDialViewBaseLayout->addWidget( frame4 );
	languageChange();
	resize( QSize(473, 438).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	
	// Initialize the rest objects.
	try_con = 1;
    hour = min = sec =0;
	var_run_pid = new char[4096];
    _shell_cmd = ""; /*"sudo";*/
    _connect_cmd = "/usr/sbin/pppd";
    _provider = "ACN";
    _disconnect_cmd = "/usr/bin/poff";
	m_textInfo->setText( "Ready" );
    isConnected = false;
    elapsed_time = new QTime(hour, min,sec);
	con_timer = new QTimer(this);
	connect ( con_timer, SIGNAL(timeout()), SLOT(connect_timer_slot() ));
	
	/*connect (connectBtn, SIGNAL (clicked() ) ,this, SLOT(connectSlot()) );
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelSlot()) );*/
}

KAlgDialView::~KAlgDialView()
{
	delete [] var_run_pid;
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KAlgDialView::languageChange()
{
	setCaption( tr2i18n( "KAlgDial" ) );
	textLabel2->setText( tr2i18n( "Connection Time :" ) );
	m_timeShow->setText( tr2i18n( "00:00:00" ) );
	textLabel3->setText( tr2i18n( "Speed :" ) );
	m_speedInfo->setText( tr2i18n( "0 kb/s" ) );
	label4->setText( tr2i18n( "Bytes in :" ) );
	m_ibytesInfo->setText( tr2i18n( "0" ) );
	textLabel5->setText( tr2i18n( "Bytes out :" ) );
	m_obytesInfo->setText( tr2i18n( "0" ) );
	textLabel6->setText( tr2i18n( "Total Bytes :" ) );
	m_tbytesInfo->setText( tr2i18n( "0" ) );
	textLabel1->setText( tr2i18n( "Packets in : " ) );
	m_ipacketsInfo->setText( tr2i18n( "0" ) );
	textLabel3_2->setText( tr2i18n( "Packets out : " ) );
	m_opacketsInfo->setText( tr2i18n( "0" ) );
}

void KAlgDialView::connectSlot()
{
	//TODO :
	/*proc = new KProcess (this);
	*proc << "my_executable";
	*proc << "These" << "are" << "the" << "command" << "line" << "args";
    QApplication::connect(proc, SIGNAL(processExited(KProcess *)),
                         pointer_to_my_object, SLOT(my_objects_slot(KProcess *)));
	proc->start();*/
	proc_io = new KProcIO();
	*proc_io << _connect_cmd;
	*proc_io << "call" << _provider << "updetach";
	KApplication::connect(proc_io, SIGNAL(readReady(KProcIO* )), this,
						  SLOT(writeProcInfo(KProcIO *)));

	//QString cmd_str =  _connect_cmd +" call " + _provider + " updetach ";
	
	//_connect_cmd_id = KRun::runCommand(cmd_str);
	if ( proc_io->start(KProcIO::NotifyOnExit, TRUE) )
	{
		m_textInfo->setText( QString("Trying connect to %1").arg(_provider) );
		m_textInfo->moveCursor(QTextEdit::MoveLineEnd, FALSE);
		_connect_cmd_id = proc_io->pid();
		kdDebug() << _connect_cmd << " pid = " << _connect_cmd_id << endl;
	}
	else
		m_textInfo->setText(QString("Can't start %1").arg(_connect_cmd ) );
	
	sleep(2);

	m_textInfo->insert(QString("."));
	con_timer->start(5000, FALSE);
}

/*!
    \fn KAlgDialView::connect_timer_slot()
 */
 void KAlgDialView::connect_timer_slot()
{
	QString message;
	if ( !pppdRunning() )
	{
 		try_con++;
// 		//Connect to the internet
// 		QString cmd_str = _shell_cmd + " " + _connect_cmd;
// 		//m_textInfo->setText( "Trying to connect ..." );
// 		_connect_cmd_id = KRun::runCommand(cmd_str);
// 		kdDebug() << cmd_str << " pid : " << _connect_cmd_id << endl;
// 		kdDebug() << "Attempt " << try_con << "!" << endl;
// 		//message = QString("Attempt %1! \n").arg(try_con);
// 		//m_textInfo->append(message);
// 		m_textInfo->insert(QString("."));
		sleep(1);
		if ( try_con > 10 )
		{
			QString message = "\nCan't connect with your ISP. Check your settings or ";
			message += " communicate with your provider to check if everthing it's OK!\n ";
			QColor old_color = m_textInfo->color(); //Save the old color.
			m_textInfo->setColor(Qt::red);
			m_textInfo->append(message);
			m_textInfo->setColor(Qt::black);
			con_timer->stop();
		}
	}
	else
	{
		//m_textInfo->append(QString("\n"));
		isConnected = true;
		ppp_pid_path(var_run_pid, pppInterfaceNumber());
		//save the timer unique ID;
		//We need this ID to kill the timer later.
		_timerid = startTimer(1000);
		emit beginDocking();
		con_timer->stop();
	}
}


void KAlgDialView::disconnectSlot()
{
    if ( isConnected )
    {
        //Shutdown the connection
        QString cmd_str = _shell_cmd + " " + _disconnect_cmd;
        _disconnect_cmd_id = KRun::runCommand(cmd_str);
        killTimer(_timerid);
        isConnected ^= 1;
		emit pppDisconnect();
    }
    else
	{
		m_textInfo->append( "KAlgDialView::cancelSlot() --> No active connection found!\n");
        emit quit_app();
	}
}

void KAlgDialView::timerEvent(QTimerEvent *)
{
	int x;
    long current_time, connect_time=0;
	struct stat st;
    current_time = time (NULL);	/* Get the current time */
	if (!stat (var_run_pid, &st))
    {
        connect_time = st.st_mtime;	/* Get the time that PPP run */
    }
    x = current_time - connect_time;	/* Online Time */
    
	hour = (x / 3600) % 24;
	min = (x / 60) % 60;
	sec = x % 60;
    
    elapsed_time->setHMS(hour, min, sec);
    QString time_string = elapsed_time->toString();
    m_timeShow->setText(time_string);
	
	//Show the rest of info.
	showInfo();
}

/*!
    \fn KAlgDialView::showInfo()
 */
void KAlgDialView::showInfo()
{
	static int last_bytes_in = 0;
	float baud;
	
	//Start inform labels.
	m_ibytesInfo->setText( QString::number( p_kppp->stats->ibytes));
	m_obytesInfo->setText( QString::number( p_kppp->stats->obytes));
	
	p_kppp->stats->totalbytes = p_kppp->stats->ibytes + p_kppp->stats->obytes;
	m_tbytesInfo->setText( QString::number( p_kppp->stats->totalbytes));
	
	//We can do this because this function will call every 1 sec 
	baud = (float)(p_kppp->stats->ibytes - last_bytes_in) ;
	QString baud_info = i18n("%1 kb/sec")
			.arg(KGlobal::locale()->formatNumber((float)baud / 1024.0, 1));
	m_speedInfo->setText( baud_info );
	last_bytes_in = p_kppp->stats->ibytes; 
	
	// packets in out
	m_ipacketsInfo->setText( QString::number( p_kppp->stats->ipackets));
	m_opacketsInfo->setText( QString::number( p_kppp->stats->opackets));
}
/*!
    \fn KAlgDialView::colorMessage(QColor , QString)
 */
 void KAlgDialView::colorMessage(QColor new_color, QString message)
{
	QColor old_color = m_textInfo->color(); //Save the old color.
	m_textInfo->setColor(new_color);
	m_textInfo->append(message);
	m_textInfo->setColor(Qt::black);
}

/*!
    \fn KAlgDialView::writeProcInfo(KProcIO *)
 */
 void KAlgDialView::writeProcInfo(KProcIO *pio)
{
	QString buffer;
	while ( pio->readln(buffer) != -1)
		m_textInfo->append(buffer);
}

#include "kalgdialview.moc"


