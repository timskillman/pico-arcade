#pragma once

#include <climits>

//namespace pimoroni {

  //static const unsigned char PIN_UNUSED = CHAR_MAX;

  enum reg : uint8_t {
	NOP       = 0x00, 	///< No-op register
    SWRESET   = 0x01,	///< Software reset register
    RDDID     = 0x04,	///< Read display identification information
    RDDRST    = 0x09,	///< Read Display Status
    RDDPM     = 0x0A,	///< Enter Sleep Mode
    RDDMADCTL = 0x0B,	///< Read Display MADCTL
    RDDCOLMOD = 0x0C,	///< Read Display Pixel Format
    RDDIM     = 0x0D,	///< Read Display Image Format
    RDDSM     = 0x0E,
    RDDSDR    = 0x0F,	///< Read Display Self-Diagnostic Result
    SLPIN     = 0x10,
    SLPOUT    = 0x11,
    PTLON     = 0x12,
    NORON     = 0x13,
    INVOFF    = 0x20,	///< Display Inversion OFF
    INVON     = 0x21,	///< Display Inversion ON
    GAMSET    = 0x26,
    DISPOFF   = 0x28,
    DISPON    = 0x29,
    CASET     = 0x2A,	///< Column Address Set
    RASET     = 0x2B,	///< Page Address Set
    RAMWR     = 0x2C,	///< Memory Write
    RGBSET    = 0x2D,
    RAMRD     = 0x2E,	///< Memory Read
    PTLAR     = 0x30,	///< Partial Area
    SCRLAR    = 0x33,	///< Vertical Scrolling Definition
    TEOFF     = 0x34,
    TEON      = 0x35,
    MADCTRL   = 0x36,  	///< Memory Access Control
    VSCSAD    = 0x37,	///< Vertical Scrolling Start Address
    IDMOFF    = 0x38,  	///< Idle Mode Off
    IDMON     = 0x39,  	///< Idle Mode On
    COLMOD    = 0x3A,	///< COLMOD: Pixel Format Set

    FRMCTR1   = 0xB1,	///< Frame Rate Control (In Normal Mode/Full Colors)
    FRMCTR2   = 0xB2,	///< Frame Rate Control (In Idle Mode/8 colors)
    FRMCTR3   = 0xB3,	///< Frame Rate control (In Partial Mode/Full Colors)
    INVCTR    = 0xB4,	///< Display Inversion Control
    DISSET5   = 0xB6,	///< Display Function Control
	GCTRL     = 0xB7,
	VCOMS     = 0xBB,
	
    PWCTR1    = 0xC0,	///< Power Control 1
    PWCTR2    = 0xC1,	///< Power Control 2
    PWCTR3    = 0xC2,	///< Power Control 3
    PWCTR4    = 0xC3,	///< Power Control 4
    PWCTR5    = 0xC4,	///< Power Control 5
    VMCTR1    = 0xC5,	///< VCOM Control 1
	FRCTRL2   = 0xC6,
	VMCTR2    = 0xC7,	///< VCOM Control 2
	
	PWRCTRL1  = 0xD0,   ///??
    RDID1     = 0xDA,	///< Read ID 1
    RDID2     = 0xDB,	///< Read ID 2
    RDID3     = 0xDC,	///< Read ID 3
    RDID4     = 0xDD,	///< Read ID 4

    GMCTRP1   = 0xE0,	///< Positive Gamma Correction
    GMCTRN1   = 0xE1,	///< Negative Gamma Correction

    PWMTR6    = 0xFC
  };
  
  enum MADCTL : uint8_t {
      ROW_ORDER   = 0b10000000,
      COL_ORDER   = 0b01000000,
      SWAP_XY     = 0b00100000,  // AKA "MV"
      SCAN_ORDER  = 0b00010000,
      RGB         = 0b00001000,
      HORIZ_ORDER = 0b00000100
  };

  #define ROT_0      0
  #define ROT_90     MADCTL::SWAP_XY | MADCTL::HORIZ_ORDER | MADCTL::COL_ORDER
  #define ROT_180    MADCTL::SCAN_ORDER | MADCTL::HORIZ_ORDER | MADCTL::COL_ORDER | MADCTL::ROW_ORDER
  #define ROT_270    MADCTL::SWAP_XY | MADCTL::HORIZ_ORDER | MADCTL::ROW_ORDER
    
//}
