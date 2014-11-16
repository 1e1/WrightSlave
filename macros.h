#ifndef CoreSlave_Macros_h
#define CoreSlave_Macros_h


/** SETUP ALIAS **/
#define LONGBYTES(b)                    static const prog_uchar  b[] PROGMEM
#define LONGBYTEN(b, n)                 static const prog_uchar b[n] PROGMEM
#define LONGSTRING(str)                 static const prog_char str[] PROGMEM
/** === **/



/** DEBUGGING TOOLS **/
#ifndef DEBUG
  #define LOG(...)
  #define LOGLN(...)
  #define WAIT(ms)
#endif
/** === **/



/** BUSY TOOLS ** /
#ifdef BUSYLED_PIN
  #define BUSYLED_HIGH digitalWrite(BUSYLED_PIN, HIGH);
  #define BUSYLED_WORK analogWrite(BUSYLED_PIN, 63);
  #define BUSYLED_IDLE analogWrite(BUSYLED_PIN, 15);
  #define BUSYLED_NONE digitalWrite(BUSYLED_PIN, LOW);
#else
  #define BUSYLED_HIGH;
  #define BUSYLED_WORK;
  #define BUSYLED_IDLE;
  #define BUSYLED_NONE;
#endif
/** === **/



// SDDD HHHH (DST[0..1], day[0..6], hour[0..23])
#define FMASK_DST(dst)        byte(dst << 7)
#define FMASK_DAY(dayOfWeek)  byte(dayOfWeek << 4)
#define FMASK_HOUR(hour)      byte(hour)
/** === **/



/** FAST TRIGO **/
#define FACTOR_PI_UINT12(x) byte((50*x) >> 4)
#define FACTOR_PI_UINT8(x)  byte((804*x) >> 8)
/** === **/



/** LOOP **/
#define FOREACH(idxtype, idxpvar, col, colsiz ) idxtype* idxpvar; for( idxpvar=col ; idxpvar < (col + (colsiz)) ; idxpvar++)
#define ARRAYLEN( ary ) ( sizeof(ary)/sizeof(*ary) )
#define FAST_STRCMP(x, y)  (*(x) != *(y) ? \
    ((unsigned char) *(x) - (unsigned char) *(y)) : \
    strcmp((x), (y)))
#define FAST_STRCMP_P(x, y)  (*(x) != *(y) ? \
    ((unsigned char) *(x) - (unsigned char) *(y)) : \
    strcmp_P((x), (y)))
/*
FOREACH (char, p1, c1, strlen(c1) ) {
  printf("loop 1 : %c\n",*p1);
}
FOREACH (int, p2, c2, ARRAYLEN(c2) ){
  printf("loop 2 : %d\n",*p2);
}
*/
/** === **/



/** BIT **/
#define bitRead_uint8_t(value, bit)             byte(((value) >> (bit)) & byte(1))
#define bitSet_uint8_t(value, bit)              ((value) |=  (byte(1) << (bit)))
#define bitClear_uint8_t(value, bit)            ((value) &= ~(byte(1) << (bit)))
#define bitWrite_uint8_t(value, bit, bitvalue)  (bitvalue ? bitSet_uint8_t(value, bit) : bitClear_uint8_t(value, bit))

#define bitRead_boolean(value, bit)             byte(((value) >> (bit)) & byte(1))
#define bitSet_boolean(value, bit)              ((value) |=  (byte(1) << (bit)))
#define bitClear_boolean(value, bit)            ((value) &= ~(byte(1) << (bit)))
#define bitWrite_boolean(value, bit, bitvalue)  (bitvalue ? bitSet_boolean(value, bit) : bitClear_boolean(value, bit))
/** === **/



#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


#endif
