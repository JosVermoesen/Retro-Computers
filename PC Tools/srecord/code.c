/* http://srecord.sourceforge.net/ */
const unsigned char eprom[] =
{
0x5A, 0x38, 0x30, 0x52, 0x4D, 0x46, 0x31, 0x32, 0x52, 0x00, 0x00, 0x00,
0x1C, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0x57, 0x00, 0x00, 0x00, 0x43, 0x08, 0x00, 0x6C, 0x6F, 0x6F, 0x70, 0x2E,
0x61, 0x73, 0x6D, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x04, 0x00, 0x6C, 0x6F, 0x6F, 0x70, 0x00, 0x4C, 0x41, 0x00, 0x00,
0x00, 0x00, 0x00, 0x04, 0x6C, 0x6F, 0x6F, 0x70, 0x08, 0x6C, 0x6F, 0x6F,
0x70, 0x2E, 0x61, 0x73, 0x6D, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6C,
0x6F, 0x6F, 0x70, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0x01, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned long eprom_termination = 0x00000000;
const unsigned long eprom_start       = 0x00000000;
const unsigned long eprom_finish      = 0x0000006B;
const unsigned long eprom_length      = 0x0000006B;

#define EPROM_TERMINATION 0x00000000
#define EPROM_START       0x00000000
#define EPROM_FINISH      0x0000006B
#define EPROM_LENGTH      0x0000006B
