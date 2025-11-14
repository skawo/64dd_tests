//Zelda 64 Dawn & Dusk - Expansion Disk
//By LuigiBlood

//Uses ARM9 bass

//Ocarina of Time Expansion Hook
print "- Assemble Expansion Disk Code...\n"

seekDisk(0)

insert DISKCODE,"../DiskC/code1.bin"

if (origin() > (0x785C8 + 0x1060)) {
  print (origin() - 0x785C8)
  error "\n\nFATAL ERROR: MAIN DISK CODE IS TOO LARGE.\nPlease reduce it and load the rest during 64DD Hook Initialization Code.\n"
}

//Initial loading from OoT File Start
seekDisk(0x1060)
dw (0)	                    //Source Start
dw (0x1120 - 0)		        //Source End
dw (0x80400000 | {KSEG1})   //Dest Start
dw (0x80401120 | {KSEG1})	//Dest End
dw (0x80400000 | {KSEG1})	//Hook Table Address

