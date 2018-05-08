//-------------------------------------------------------------------------------------------------
//
// HexStringToUInt
//
//-------------------------------------------------------------------------------------------------
#include "PciLib.h"
#include <stdio.h>

//-------------------------------------------------------------------------------------------------
//
// MmPciAddress
//
//-------------------------------------------------------------------------------------------------
unsigned long MmPciAddress(unsigned long PcieBase, PCI_PHYSIC_LOC *PciPhysicLoc)
{
        return (PcieBase + (PciPhysicLoc->Bus << 20) + (PciPhysicLoc->Dev << 15) + (PciPhysicLoc->Func << 12));
}

//-------------------------------------------------------------------------------------------------
//
// GetPcieLinkRegs
//
//-------------------------------------------------------------------------------------------------
int GetPcieLinkRegs(unsigned long MmAddr, PCIE_CAP_LINK_REGS *PcieLink)
{
        int            Valid = 1;
        unsigned long   PcieCapOffset = FindPcieCapOffset(MmAddr);

        if(INVALID_PCIE_CAP_OFFSET == PcieCapOffset) {
                Valid = 0;
        }
        else {
                PcieLink->LinkCap.RegVal = *((unsigned long*)(MmAddr + PcieCapOffset + 0x0C));
                PcieLink->LinkCtl.RegVal = *((unsigned short*)(MmAddr + PcieCapOffset + 0x10));
                PcieLink->LinkSts.RegVal = *((unsigned short*)(MmAddr + PcieCapOffset + 0x12));
        }

        return Valid;
}

//-------------------------------------------------------------------------------------------------
//
// FindPcieCapbilitiesOffset
//
//-------------------------------------------------------------------------------------------------
unsigned long FindPcieCapOffset(unsigned long MmAddr)
{
        PCI_CAP_HEADER  PciCapHdr;
        unsigned long   Offset = CAPBILITIES_LIST_START_OFFSET;

        PciCapHdr.CapId = 0;
        PciCapHdr.NextCap = *((unsigned char*)(MmAddr + Offset));

        do {
                Offset = PciCapHdr.NextCap;
                PciCapHdr.CapId = *((unsigned char*)(MmAddr + Offset));
                PciCapHdr.NextCap = *((unsigned char*)(MmAddr + Offset + 1));
        } while((PciCapHdr.CapId != 0x10) && (PciCapHdr.NextCap != 0x00));

        if(0x10 != PciCapHdr.CapId)
                Offset = INVALID_PCIE_CAP_OFFSET;               // Means PCIE capability not found

        return Offset;
}
