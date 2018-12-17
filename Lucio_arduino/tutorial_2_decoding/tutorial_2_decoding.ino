using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
 
[StructLayout(LayoutKind.Explicit, Pack = 1)]
public unsafe struct IRPacket
{
    [FieldOffset(0)]
    public fixed byte Start[40];
    // 01001010 01110101 11000011 01100100 10011011  hard coded does not seem to change ever
     
    [FieldOffset(40)]
    public fixed byte UpDownSwingManual2sCompiment[1];
    [FieldOffset(41)]
    public fixed byte Allergon2sCompliment[1];
    [FieldOffset(42)]
    public fixed byte Unknown42[2]; // Both 11  (2s compilment to offset 50)
    [FieldOffset(44)]
    public fixed byte LeftRightSwing2sCompliment[4];
 
    [FieldOffset(48)]
    public fixed byte UpDownSwingManual[1];
    [FieldOffset(49)]
    public fixed byte Allergon[1];
    [FieldOffset(50)]
    public fixed byte Unknown50[2];// Both 00 - 2s compilment to offset 42)
    [FieldOffset(52)]
    public fixed byte LeftRightSwing[4];
 
    [FieldOffset(56)]
    public fixed byte Unknown56[3]; // 111 - 2s compliment to offset 64)
    [FieldOffset(59)]
    public fixed byte UpDownSwing2sCompliment[2];   
    [FieldOffset(61)]
    public fixed byte FanSpeed2sCompliment[3];
 
    [FieldOffset(64)]
    public fixed byte Unknown64[3]; // 000 - 2s compliment to offset 56)
    [FieldOffset(67)]
    public fixed byte UpDownSwing[2];
    [FieldOffset(69)]
    public fixed byte FanSpeed[3];
 
    [FieldOffset(72)]
    public fixed byte Mode2sCompliment[3];
    [FieldOffset(75)]
    public fixed byte OnOff2sCompliment[1];
    [FieldOffset(76)]
    public fixed byte Temp2sCompliment[4];
 
    [FieldOffset(80)]
    public fixed byte Mode[3];
    [FieldOffset(83)]
    public fixed byte OnOff[1];
    [FieldOffset(84)]
    public fixed byte Temp[4];
   }using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
 
[StructLayout(LayoutKind.Explicit, Pack = 1)]
public unsafe struct IRPacket
{
    [FieldOffset(0)]
    public fixed byte Start[40];
    // 01001010 01110101 11000011 01100100 10011011  hard coded does not seem to change ever
     
    [FieldOffset(40)]
    public fixed byte UpDownSwingManual2sCompiment[1];
    [FieldOffset(41)]
    public fixed byte Allergon2sCompliment[1];
    [FieldOffset(42)]
    public fixed byte Unknown42[2]; // Both 11  (2s compilment to offset 50)
    [FieldOffset(44)]
    public fixed byte LeftRightSwing2sCompliment[4];
 
    [FieldOffset(48)]
    public fixed byte UpDownSwingManual[1];
    [FieldOffset(49)]
    public fixed byte Allergon[1];
    [FieldOffset(50)]
    public fixed byte Unknown50[2];// Both 00 - 2s compilment to offset 42)
    [FieldOffset(52)]
    public fixed byte LeftRightSwing[4];
 
    [FieldOffset(56)]
    public fixed byte Unknown56[3]; // 111 - 2s compliment to offset 64)
    [FieldOffset(59)]
    public fixed byte UpDownSwing2sCompliment[2];   
    [FieldOffset(61)]
    public fixed byte FanSpeed2sCompliment[3];
 
    [FieldOffset(64)]
    public fixed byte Unknown64[3]; // 000 - 2s compliment to offset 56)
    [FieldOffset(67)]
    public fixed byte UpDownSwing[2];
    [FieldOffset(69)]
    public fixed byte FanSpeed[3];
 
    [FieldOffset(72)]
    public fixed byte Mode2sCompliment[3];
    [FieldOffset(75)]
    public fixed byte OnOff2sCompliment[1];
    [FieldOffset(76)]
    public fixed byte Temp2sCompliment[4];
 
    [FieldOffset(80)]
    public fixed byte Mode[3];
    [FieldOffset(83)]
    public fixed byte OnOff[1];
    [FieldOffset(84)]
    public fixed byte Temp[4];
   }
