/************************************************************************************************

 Author        : CAC (China Applications Support Team)         

 Date          : October 2012

 File          : ADE7953_SPI.h

 Hardware      : ADE7953, ADuC7060

 Description   : Test with ADE7953
 			     Config ADE7953 to read phase A current rms, voltage rms, energy, period, in linecycle mode

*************************************************************************************************/

#define  AIGAIN                     0x4380 
#define  AVGAIN                     0x4381 
#define  BIGAIN                     0x4382 
#define  BVGAIN                     0x4383 
#define  CIGAIN                     0x4384 
#define  CVGAIN                     0x4385 
#define  NIGAIN                     0x4386 
#define  AIRMSOS                    0x4387 
#define  AVRMSOS                    0x4388 
#define  BIRMSOS                    0x4389 
#define  BVRMSOS                    0x438A 
#define  CIRMSOS                    0x438B 
#define  CVRMSOS                    0x438C 
#define  NIRMSOS                    0x438D 
#define  AVAGAIN                    0x438E 
#define  BVAGAIN                    0x438F 
#define  CVAGAIN                    0x4390 
#define  AWGAIN                     0x4391 
#define  AWATTOS                    0x4392 
#define  BWGAIN                     0x4393 
#define  BWATTOS                    0x4394 
#define  CWGAIN                     0x4395 
#define  CWATTOS                    0x4396 
#define  AVARGAIN                   0x4397 
#define  AVAROS                     0x4398  
#define  BVARGAIN                   0x4399  
#define  BVAROS                     0x439A  
#define  CVARGAIN                   0x439B  
#define  CVAROS                     0x439C  
#define  AFWGAIN                    0x439D  
#define  AFWATTOS                   0x439E  
#define  BFWGAIN                    0x439F  
#define  BFWATTOS                   0x43A0  
#define  CFWGAIN                    0x43A1  
#define  CFWATTOS                   0x43A2  
#define  AFVARGAIN                  0x43A3  
#define  AFVAROS                    0x43A4  
#define  BFVARGAIN                  0x43A5  
#define  BFVAROS                    0x43A6  
#define  CFVARGAIN                  0x43A7  
#define  CFVAROS                    0x43A8  
#define  VATHR1                     0x43A9  
#define  VATHR0                     0x43AA  
#define  WTHR1                      0x43AB  
#define  WTHR0                      0x43AC  
#define  VARTHR1                    0x43AD  
#define  VARTHR0                    0x43AE    
#define  VANOLOAD                   0x43B0   
#define  APNOLOAD                   0x43B1   
#define  VARNOLOAD                  0x43B2   
#define  VLEVEL                     0x43B3      
#define  DICOEFF                    0x43B5   
#define  HPFDIS                     0x43B6   
#define  ISUMLVL                    0x43B8   
#define  ISUM                       0x43BF    
#define  AIRMS                      0x43C0    
#define  AVRMS                      0x43C1    
#define  BIRMS                      0x43C2    
#define  BVRMS                      0x43C3    
#define  CIRMS                      0x43C4    
#define  CVRMS                      0x43C5    
#define  NIRMS                      0x43C6    

#define  Run                        0xE228
   //Billable Registers 
#define  AWATTHR                    0xE400 
#define  BWATTHR                    0xE401 
#define  CWATTHR                    0xE402 
#define  AFWATTHR                   0xE403 
#define  BFWATTHR                   0xE404 
#define  CFWATTHR                   0xE405 
#define  AVARHR                     0xE406 
#define  BVARHR                     0xE407 
#define  CVARHR                     0xE408 
#define  AFVARHR                    0xE409 
#define  BFVARHR                    0xE40A 
#define  CFVARHR                    0xE40B 
#define  AVAHR                      0xE40C 
#define  BVAHR                      0xE40D
#define  CVAHR                      0xE40E 

//Configuration and Power Quality Registers

#define  IPEAK                      0xE500    
#define  VPEAK                      0xE501    
#define  STATUS0                    0xE502    
#define  STATUS1                    0xE503    
#define  AIMAV                      0xE504    
#define  BIMAV                      0xE505    
#define  CIMAV                      0xE506    
#define  OILVL                      0xE507    
#define  OVLVL                      0xE508    
#define  SAGLVL                     0xE509    
#define  MASK0                      0xE50A    
#define  MASK1                      0xE50B    
#define  IAWV                       0xE50C    
#define  IBWV                       0xE50D  
#define  ICWV                       0xE50E  
#define  INWV                       0xE50F  
#define  VAWV                       0xE510  
#define  VBWV                       0xE511  
#define  VCWV                       0xE512  
#define  AWATT                      0xE513 
#define  BWATT                      0xE514 
#define  CWATT                      0xE515 
#define  AVAR                       0xE516 
#define  BVAR                       0xE517 
#define  CVAR                       0xE518 
#define  AVA                        0xE519 
#define  BVA                        0xE51A 
#define  CVA                        0xE51B 
#define  CHECKSUM                   0xE51F 
#define  VNOM                       0xE520  
//16bit register
#define  PHSTATUS                   0xE600  
#define  ANGLE0                     0xE601  
#define  ANGLE1                     0xE602  
#define  ANGLE2                     0xE603   
#define  PERIODADE                  0xE607  
#define  PHNOLOAD                   0xE608  
#define  LINECYC                    0xE60C  
#define  ZXTOUT                     0xE60D  
#define  COMPMODE                   0xE60E  
#define  Gain                       0xE60F  
#define  CFMODE                     0xE610  
#define  CF1DEN                     0xE611  
#define  CF2DEN                     0xE612  
#define  CF3DEN                     0xE613  
#define  APHCAL                     0xE614  
#define  BPHCAL                     0xE615  
#define  CPHCAL                     0xE616  
#define  PHSIGN                     0xE617  
#define  CONFIG                     0xE618  
//8 bit  register
#define  MMODE                      0xE700  
#define  ACCMODE                    0xE701  
#define  LCYCMODE                   0xE702  
#define  PEAKCYC                    0xE703  
#define  SAGCYC                     0xE704  
#define  CFCYC                      0xE705  
#define  HSDC_CFG                   0xE706  
#define  Version                    0xE707 
#define  LPOILVL                    0xEC00 
#define  CONFIG2                    0xEC01
 
void ADE7878Reset(void); 
void ADE7878PSM0(void);
void ADE7878SPICfg(void);
void ADE7878Cfg(void);
 void PowerMeasurment(void);

