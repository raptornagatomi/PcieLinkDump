//-------------------------------------------------------------------------------------------------
//
// Includes
//
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "MyStringLib.h"
#include "PciLib.h"

//-------------------------------------------------------------------------------------------------
//
// Function Prototypes
//
//-------------------------------------------------------------------------------------------------
int ParseParam(int argc, char *argv[], PCI_PHYSIC_LOC *PciPhysicLoc, unsigned long *PcieBase);
void DispHelp();
void PcieLinkInterpreter(PCIE_CAP_LINK_REGS *PcieLink);

//-------------------------------------------------------------------------------------------------
//
// Entrypoint
//
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
        unsigned long           PcieBase = 0;
        PCI_PHYSIC_LOC          PciPhysicLoc;
        unsigned long           *pMemLoc = 0;
        PCIE_CAP_LINK_REGS      PcieLink;

        if(ParseParam(argc, argv, &PciPhysicLoc, &PcieBase)) {
                pMemLoc = (unsigned long*)MmPciAddress(PcieBase, &PciPhysicLoc);

                // Get PCIE Capability Link registers (CAP, CTL, and STS)
                printf("\n");

                if(GetPcieLinkRegs((unsigned long)pMemLoc, &PcieLink)) {
                        PcieLinkInterpreter(&PcieLink);
                }
                else {
                        printf("PCIE Capability not found\n");
                }

                printf("\n");
        }
        else {
                DispHelp();
        }

        return 0;
}

//-------------------------------------------------------------------------------------------------
//
// ParseParam
//
//-------------------------------------------------------------------------------------------------
int ParseParam(int argc, char *argv[], PCI_PHYSIC_LOC *PciPhysicLoc, unsigned long *PcieBase)
{
        int     Valid = 1;
        int     i = 0;

        //
        // Check if input parameters are valid
        //
        if(argc != 5) {
                Valid = 0;
        }
        else {
                // Check if the input parameters are all hexadecimal numbers
                for(i = 1; i <= 4; i++) {
                        if(0 == IsHexNumString(argv[i])) {
                                Valid = 0;
                                break;
                        }
                }
        }

        //
        // Get PCIE Base Address for target device
        //
        if(Valid) {
                *PcieBase = HexStringToUInt(argv[1]);
                PciPhysicLoc->Bus = (unsigned int)HexStringToUInt(argv[2]);
                PciPhysicLoc->Dev = (unsigned int)HexStringToUInt(argv[3]);
                PciPhysicLoc->Func = (unsigned int)HexStringToUInt(argv[4]);
        }

        return Valid;
}

//-------------------------------------------------------------------------------------------------
//
// DispHelp
//
//-------------------------------------------------------------------------------------------------
void DispHelp()
{
        printf("\n");
        printf("PcieLink <PCIE Base Address> <Bus#> <Device#> <Function#>\n");
        printf("  - PCIE Base Address: PCIE MMIO Base Address\n");
        printf("  - Bus#             : Bus number of target bridge/device\n");
        printf("  - Device#          : Device number of target bridge/device\n");
        printf("  - Function#        : Function number of target bridge/device\n");
        printf("\nwhere all parameters are in hexadecimal format without\nthe leading \"0x\" or the tailing \"h\"\n");
        printf("\n");
}

//-------------------------------------------------------------------------------------------------
//
// PcieLinkInterpreter
//
//-------------------------------------------------------------------------------------------------
void PcieLinkInterpreter(PCIE_CAP_LINK_REGS *PcieLink)
{
        //
        // Link Capabilities
        //
        printf("LinkCap = 0x%08X\n", PcieLink->LinkCap.RegVal);

        // Max Link Speed
        printf("  - Max Link Speed: ");
        switch(PcieLink->LinkCap.Bits.MaxLnkSpeed) {
        case LinkSpeed_2_5_GTS:
                printf("2.5 GT/s\n");
                break;

        case LinkSpeed_5_0_GTS:
                printf("5.0 GT/s\n");
                break;

        case LinkSpeed_8_0_GTS:
                printf("8.0 GT/s\n");
                break;

        case LinkSpeed_16_0_GTS:
                printf("16.0 GT/s\n");
                break;

        default:
                printf("Unknown\n");
                break;
        }

        // Max Link Width
        printf("  - Max Link Width: ");
        if(0 != PcieLink->LinkCap.Bits.MaxLnkWidth)
                printf("x%d\n", PcieLink->LinkCap.Bits.MaxLnkWidth);
        else
                printf("Unknown\n");

        // ASPM Support
        printf("  - ASPM Support: ");
        switch(PcieLink->LinkCap.Bits.AspmSupport) {
        case AspmNotSupport:
                printf("Not Supported\n");
                break;

        case AspmL0s:
                printf("L0s\n");
                break;

        case AspmL1:
                printf("L1\n");
                break;

        case AspmL0sAndL1:
                printf("L0s and L1\n");
                break;

        default:
                printf("Unknown\n");
                break;
        }

        //
        // Link Control
        //
        printf("LinkCtl = 0x%04X\n", PcieLink->LinkCtl.RegVal);

        // ASPM Control
        printf("  - ASPM Control: ");
        switch(PcieLink->LinkCtl.Bits.AspmControl) {
        case AspmDisabled:
                printf("Disabled\n");
                break;

        case AspmL0sEnabled:
                printf("L0s Enabled\n");
                break;

        case AspmL1Enabled:
                printf("L1 Enabled\n");
                break;

        case AspmL0sAndL1Enabled:
                printf("L0s and L1 Enabled\n");
                break;

        default:
                printf("Unknown\n");
                break;
        }

        //
        // Link Status
        //
        printf("LinkSts = 0x%04X\n", PcieLink->LinkSts.RegVal);

        // Current Link Speed
        printf("  - Current Link Speed: ");
        switch(PcieLink->LinkSts.Bits.LnkSpeed) {
        case LinkSpeed_2_5_GTS:
                printf("2.5 GT/s\n");
                break;

        case LinkSpeed_5_0_GTS:
                printf("5.0 GT/s\n");
                break;

        case LinkSpeed_8_0_GTS:
                printf("8.0 GT/s\n");
                break;

        case LinkSpeed_16_0_GTS:
                printf("16.0 GT/s\n");
                break;

        default:
                printf("Unknown\n");
                break;
        }

        // Negotiated Link Width
        printf("  - Negotiated Link Width: ");
        if(0 != PcieLink->LinkSts.Bits.LnkWidth)
                printf("x%d\n", PcieLink->LinkSts.Bits.LnkWidth);
        else
                printf("Unknown\n");
}
