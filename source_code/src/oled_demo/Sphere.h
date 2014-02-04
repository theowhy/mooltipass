/*
 * bitmap sphere
 */

#define SPHERE_WIDTH 64
#define SPHERE_HEIGHT 64

const struct {
    uint8_t width;
    uint8_t height;
    uint8_t depth;
    uint16_t dataSize;
    uint16_t data[1024];
} image_sphere __attribute__((__progmem__)) = {
    SPHERE_WIDTH, SPHERE_HEIGHT, 4, 1024,
    {
   /* 0 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 1 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 2 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 3 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 4 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 5 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 6 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0001,  0x2333,  
    0x3321,  0x1000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 7 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0001,  0x3577,  0x7766,  
    0x6655,  0x5431,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 8 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0268,  0x8887,  0x7777,  
    0x6666,  0x5554,  0x4210,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 9 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0002,  0x6888,  0x8888,  0x8877,  
    0x7766,  0x6555,  0x4432,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 10 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0059,  0x9999,  0x9998,  0x8888,  
    0x7776,  0x6655,  0x4443,  0x3100,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 11 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x2899,  0x9999,  0x9999,  0x9888,  
    0x8777,  0x6665,  0x5444,  0x3320,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 12 */ 0x0000,  0x0000,  0x0000,  0x0004,  0x999a,  0xaaaa,  0xaa99,  0x9998,  
    0x8877,  0x7665,  0x5544,  0x3332,  0x1000,  0x0000,  0x0000,  0x0000,  
   
   /* 13 */ 0x0000,  0x0000,  0x0000,  0x0059,  0xaaaa,  0xaaaa,  0xaaaa,  0x9999,  
    0x8887,  0x7766,  0x5544,  0x4332,  0x2100,  0x0000,  0x0000,  0x0000,  
   
   /* 14 */ 0x0000,  0x0000,  0x0000,  0x049a,  0xaaab,  0xbbbb,  0xbaaa,  0xaa99,  
    0x9888,  0x7766,  0x6554,  0x4333,  0x2210,  0x0000,  0x0000,  0x0000,  
   
   /* 15 */ 0x0000,  0x0000,  0x0000,  0x3aaa,  0xabbb,  0xbbbb,  0xbbbb,  0xaaa9,  
    0x9988,  0x7776,  0x6554,  0x4433,  0x2210,  0x0000,  0x0000,  0x0000,  
   
   /* 16 */ 0x0000,  0x0000,  0x0002,  0x9aab,  0xbbbb,  0xcccc,  0xcbbb,  0xbaaa,  
    0x9998,  0x8776,  0x6555,  0x4433,  0x2221,  0x0000,  0x0000,  0x0000,  
   
   /* 17 */ 0x0000,  0x0000,  0x0008,  0xaabb,  0xbccc,  0xcccc,  0xcccb,  0xbbaa,  
    0xa998,  0x8877,  0x6655,  0x4433,  0x3221,  0x1000,  0x0000,  0x0000,  
   
   /* 18 */ 0x0000,  0x0000,  0x005a,  0xabbb,  0xcccc,  0xdddd,  0xcccc,  0xbbba,  
    0xa999,  0x8877,  0x6655,  0x4443,  0x3221,  0x1000,  0x0000,  0x0000,  
   
   /* 19 */ 0x0000,  0x0000,  0x019a,  0xabbc,  0xccdd,  0xdddd,  0xddcc,  0xcbbb,  
    0xaa99,  0x8877,  0x6655,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 20 */ 0x0000,  0x0000,  0x06aa,  0xbbcc,  0xcddd,  0xeeee,  0xdddc,  0xccbb,  
    0xaa99,  0x8887,  0x7665,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 21 */ 0x0000,  0x0000,  0x19aa,  0xbbcc,  0xddde,  0xeeee,  0xeddd,  0xccbb,  
    0xaa99,  0x9887,  0x7665,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 22 */ 0x0000,  0x0000,  0x5aab,  0xbbcc,  0xddee,  0xeffe,  0xeedd,  0xccbb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3222,  0x1100,  0x0000,  0x0000,  
   
   /* 23 */ 0x0000,  0x0000,  0x9aab,  0xbccd,  0xdeee,  0xffff,  0xeeed,  0xdccb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3222,  0x1100,  0x0000,  0x0000,  
   
   /* 24 */ 0x0000,  0x0003,  0x9aab,  0xbccd,  0xdeef,  0xffff,  0xfeed,  0xdccb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3222,  0x1100,  0x0000,  0x0000,  
   
   /* 25 */ 0x0000,  0x0005,  0x9aab,  0xbccd,  0xdeef,  0xffff,  0xfeed,  0xdccb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3222,  0x1100,  0x0000,  0x0000,  
   
   /* 26 */ 0x0000,  0x0008,  0x9aab,  0xbccd,  0xdeee,  0xffff,  0xeedd,  0xdccb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 27 */ 0x0000,  0x0019,  0x9aab,  0xbccd,  0xddee,  0xeffe,  0xeedd,  0xccbb,  
    0xbaa9,  0x9887,  0x7665,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 28 */ 0x0000,  0x0029,  0x9aab,  0xbbcc,  0xddee,  0xeeee,  0xeddd,  0xccbb,  
    0xaa99,  0x8887,  0x7665,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 29 */ 0x0000,  0x0039,  0x99aa,  0xbbcc,  0xcddd,  0xeeee,  0xdddc,  0xcbbb,  
    0xaa99,  0x8877,  0x6655,  0x5443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 30 */ 0x0000,  0x0048,  0x99aa,  0xbbbc,  0xccdd,  0xdddd,  0xddcc,  0xcbba,  
    0xaa99,  0x8877,  0x6655,  0x4443,  0x3221,  0x1100,  0x0000,  0x0000,  
   
   /* 31 */ 0x0000,  0x0048,  0x999a,  0xabbb,  0xcccc,  0xdddd,  0xcccc,  0xbbba,  
    0xa998,  0x8877,  0x6655,  0x4433,  0x3221,  0x1000,  0x0000,  0x0000,  
   
   /* 32 */ 0x0000,  0x0048,  0x899a,  0xaabb,  0xbccc,  0xcccc,  0xccbb,  0xbbaa,  
    0x9998,  0x8776,  0x6655,  0x4433,  0x2211,  0x1000,  0x0000,  0x0000,  
   
   /* 33 */ 0x0000,  0x0048,  0x8999,  0xaaab,  0xbbbc,  0xcccc,  0xbbbb,  0xbaaa,  
    0x9988,  0x8776,  0x6554,  0x4433,  0x2211,  0x1000,  0x0000,  0x0000,  
   
   /* 34 */ 0x0000,  0x0038,  0x8899,  0x9aaa,  0xbbbb,  0xbbbb,  0xbbba,  0xaaa9,  
    0x9888,  0x7766,  0x6554,  0x4333,  0x2211,  0x1000,  0x0000,  0x0000,  
   
   /* 35 */ 0x0000,  0x0037,  0x8889,  0x99aa,  0xaabb,  0xbbbb,  0xbaaa,  0xa999,  
    0x9887,  0x7766,  0x5544,  0x4332,  0x2111,  0x0000,  0x0000,  0x0000,  
   
   /* 36 */ 0x0000,  0x0027,  0x7888,  0x9999,  0xaaaa,  0xaaaa,  0xaaaa,  0x9998,  
    0x8877,  0x7665,  0x5544,  0x3332,  0x2111,  0x0000,  0x0000,  0x0000,  
   
   /* 37 */ 0x0000,  0x0017,  0x7788,  0x8999,  0x99aa,  0xaaaa,  0xa999,  0x9988,  
    0x8877,  0x6665,  0x5444,  0x3322,  0x2110,  0x0000,  0x0000,  0x0000,  
   
   /* 38 */ 0x0000,  0x0006,  0x7778,  0x8889,  0x9999,  0x9999,  0x9999,  0x8888,  
    0x7776,  0x6655,  0x5443,  0x3322,  0x1110,  0x0000,  0x0000,  0x0000,  
   
   /* 39 */ 0x0000,  0x0004,  0x6777,  0x8888,  0x8999,  0x9999,  0x9988,  0x8887,  
    0x7766,  0x6555,  0x4443,  0x3222,  0x1110,  0x0000,  0x0000,  0x0000,  
   
   /* 40 */ 0x0000,  0x0002,  0x6677,  0x7788,  0x8888,  0x8888,  0x8888,  0x8777,  
    0x7666,  0x5554,  0x4433,  0x2221,  0x1100,  0x0000,  0x0000,  0x0000,  
   
   /* 41 */ 0x0000,  0x0000,  0x5666,  0x7777,  0x8888,  0x8888,  0x8887,  0x7776,  
    0x6665,  0x5544,  0x4333,  0x2211,  0x1000,  0x0000,  0x0000,  0x0000,  
   
   /* 42 */ 0x0000,  0x0000,  0x4666,  0x6777,  0x7777,  0x7777,  0x7777,  0x7766,  
    0x6655,  0x5444,  0x3332,  0x2211,  0x1000,  0x0000,  0x0000,  0x0000,  
   
   /* 43 */ 0x0000,  0x0000,  0x1556,  0x6666,  0x7777,  0x7777,  0x7776,  0x6666,  
    0x5555,  0x4443,  0x3322,  0x2111,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 44 */ 0x0000,  0x0000,  0x0455,  0x5666,  0x6666,  0x6666,  0x6666,  0x6655,  
    0x5544,  0x4433,  0x3222,  0x1110,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 45 */ 0x0000,  0x0000,  0x0155,  0x5555,  0x6666,  0x6666,  0x6665,  0x5555,  
    0x5444,  0x4333,  0x2221,  0x1110,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 46 */ 0x0000,  0x0000,  0x0034,  0x5555,  0x5555,  0x5555,  0x5555,  0x5554,  
    0x4443,  0x3332,  0x2211,  0x1100,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 47 */ 0x0000,  0x0000,  0x0004,  0x4445,  0x5555,  0x5555,  0x5555,  0x4444,  
    0x4333,  0x3222,  0x2111,  0x1000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 48 */ 0x0000,  0x0000,  0x0001,  0x4444,  0x4444,  0x4444,  0x4444,  0x4443,  
    0x3333,  0x2222,  0x1111,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 49 */ 0x0000,  0x0000,  0x0000,  0x2344,  0x4444,  0x4444,  0x4444,  0x4333,  
    0x3322,  0x2211,  0x1110,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 50 */ 0x0000,  0x0000,  0x0000,  0x0233,  0x3333,  0x4443,  0x3333,  0x3333,  
    0x2222,  0x2111,  0x1000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 51 */ 0x0000,  0x0000,  0x0000,  0x0023,  0x3333,  0x3333,  0x3333,  0x3222,  
    0x2221,  0x1111,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 52 */ 0x0000,  0x0000,  0x0000,  0x0001,  0x2233,  0x3333,  0x3222,  0x2222,  
    0x2111,  0x1110,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 53 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x1222,  0x2222,  0x2222,  0x2211,  
    0x1111,  0x1000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 54 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0012,  0x2222,  0x2211,  0x1111,  
    0x1110,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 55 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0001,  0x1111,  0x1111,  0x1111,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 56 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0011,  0x1111,  0x1000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 57 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 58 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 59 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 60 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 61 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 62 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
   /* 63 */ 0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
    0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  
   
    }
};
