#ifndef _PciLibH_
#define _PciLibH_
//-------------------------------------------------------------------------------------------------
//
// Includes
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
// Constants
//
//-------------------------------------------------------------------------------------------------
#define CAPBILITIES_LIST_START_OFFSET                   0x34
#define INVALID_PCIE_CAP_OFFSET                         0xFFFFFFFF

//-------------------------------------------------------------------------------------------------
//
// Type Definition
//
//-------------------------------------------------------------------------------------------------
#pragma pack(1)
typedef struct {
        unsigned short    Bus;
        unsigned short    Dev;
        unsigned short    Func;
} PCI_PHYSIC_LOC;

typedef struct {
        unsigned char   CapId;
        unsigned char   NextCap;
} PCI_CAP_HEADER;

typedef union {
        unsigned long RegVal;
        struct {
                unsigned long  MaxLnkSpeed     : 4;     // bit 0..3
                unsigned long  MaxLnkWidth     : 6;     // bit 4, 5, 6, 7, 8, 9
                unsigned long  AspmSupport     : 2;     // bit 10, 11
                unsigned long  ExL0Latency     : 3;     // bit 12, 13, 14
                unsigned long  ExL1Latency     : 3;     // bit 15, 16, 17
                unsigned long  ClockPm         : 1;     // bit 18
                unsigned long  SurpDownErr     : 1;     // bit 19
                unsigned long  DllLnkActive    : 1;     // bit 20
                unsigned long  BandwNotify     : 1;     // bit 21
                unsigned long  Reserved        : 2;     // bit 22,23
                unsigned long  PortNum         : 8;     // bit 24,25,26,27,28,29,30,31
        } Bits;
} PCIE_LNK_CAP_REG;

typedef union {
        unsigned short RegVal;
        struct {
                unsigned short AspmControl        : 2;    // bit 0,1
                unsigned short Reserved0          : 1;    // bit 2
                unsigned short RdComplBound       : 1;    // bit 3
                unsigned short LnkDisable         : 1;    // bit 4
                unsigned short RetrainLnk         : 1;    // bit 5
                unsigned short CommonClk          : 1;    // bit 6
                unsigned short ExtSynch           : 1;    // bit 7
                unsigned short ClockPm            : 1;    // bit 8
                unsigned short HwAutoWdtDis       : 1;    // bit 9
                unsigned short BandwMgmtInt       : 1;    // bit 10
                unsigned short AutoBandwInt       : 1;    // bit 11
                unsigned short Reserved1          : 4;    // bit 12,13,14,15
        } Bits;
} PCIE_LNK_CTL_REG;

typedef union {
        unsigned short RegVal;
        struct {
                unsigned short LnkSpeed           : 4;    // bit 0,1,2,3
                unsigned short LnkWidth           : 6;    // bit 4,5,6,7,8,9
                unsigned short TrainingErr        : 1;    // bit 10
                unsigned short LnkTraining        : 1;    // bit 11
                unsigned short CommonClk          : 1;    // bit 12
                unsigned short DllLnkActive       : 1;    // bit 13
                unsigned short BandwMgmtSts       : 1;    // bit 14
                unsigned short AutoBandwSts       : 1;    // bit 15
        } Bits;
} PCIE_LNK_STS_REG;

typedef struct {
        PCIE_LNK_CAP_REG        LinkCap;
        PCIE_LNK_CTL_REG        LinkCtl;
        PCIE_LNK_STS_REG        LinkSts;
} PCIE_CAP_LINK_REGS;
#pragma pack()

//-------------------------------------------------------------------------------------------------
//
// Enumeration
//
//-------------------------------------------------------------------------------------------------
// Maximum Link Speed
enum {
        LinkSpeed_2_5_GTS = 1,
        LinkSpeed_5_0_GTS,
        LinkSpeed_8_0_GTS,
        LinkSpeed_16_0_GTS
};

// ASPM Support
enum {
        AspmNotSupport = 0,
        AspmL0s,
        AspmL1,
        AspmL0sAndL1
};

// ASPM Control
enum {
        AspmDisabled = 0,
        AspmL0sEnabled,
        AspmL1Enabled,
        AspmL0sAndL1Enabled
};

// L0s Exit Latency
enum {
        L0sExL_LT64ns,                  // 000b: Less than 64 ns
        L0sExL_GE64ns_LT128ns,          // 001b: 64 ns to less than 128 ns
        L0sExL_GE128ns_LT256ns,         // 010b: 128 ns to less than 256 ns
        L0sExL_GE256ns_LT512ns,         // 011b: 256 ns to less than 512 ns
        L0sExL_GE512ns_LT1us,           // 100b: 512 ns to less than 1 us
        L0sExL_GE1us_LT2us,             // 101b: 1 us to less than 2 us
        L0sExL_GE2us_LE4us,             // 110b: 2 us-4 us
        L0sExL_GT4us                    // 111b: More than 4 us
};

// L1 Exit Latency
enum {
        L1ExL_LT1us,                    // 000b: Less than 1us
        L1ExL_GE1us_LT2us,              // 001b: 1 us to less than 2 us
        L1ExL_GE2us_LT4us,              // 010b: 2 us to less than 4 us
        L1ExL_GE4us_LT8us,              // 011b: 4 us to less than 8 us
        L1ExL_GE8us_LT16us,             // 100b: 8 us to less than 16 us
        L1ExL_GE16us_LT32us,            // 101b: 16 us to less than 32 us
        L1ExL_GE32us_LT64us,            // 110b: 32 us-64 us
        L1ExL_GT64us                    // 111b: More than 64 us
};

//-------------------------------------------------------------------------------------------------
//
// Function Prototypes
//
//-------------------------------------------------------------------------------------------------
unsigned long MmPciAddress(unsigned long PcieBase, PCI_PHYSIC_LOC *PciPhysicLoc);
int GetPcieLinkRegs(unsigned long MmAddr, PCIE_CAP_LINK_REGS *PcieLink);
unsigned long FindPcieCapOffset(unsigned long MmAddr);

#endif

