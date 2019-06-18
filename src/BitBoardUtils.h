//
// Created by louis on 6/6/19.
//

#ifndef OTHELLOPROJECT_BITBOARDUTILS_H
#define OTHELLOPROJECT_BITBOARDUTILS_H

const static unsigned long CORNERS = 0x8100000000000081;
const static unsigned long CORNER_NW = 0x8000000000000000;

const static unsigned long CORNER_NE = 0x0100000000000000;
const static unsigned long CORNER_SE = 0x0000000000000080;
const static unsigned long CORNER_SW = 0x0000000000000001;
const static unsigned long LITTLE_SPOON_NW = 0x0203000000000000;

const static unsigned long LITTLE_SPOON_NE = 0x40c0000000000000;
const static unsigned long LITTLE_SPOON_SE = 0x0000000000000302;
const static unsigned long LITTLE_SPOON_SW = 0x000000000000c040;
const static unsigned long LITTLE_SPOON_SQUARES = 0x42c300000000c342;

const static unsigned long BIG_SPOON_NW = 0x2020e00000000000;
const static unsigned long BIG_SPOON_NE = 0x0404070000000000;
const static unsigned long BIG_SPOON_SE = 0x0000000000070404;
const static unsigned long BIG_SPOON_SW = 0x0000000000e02020;
const static unsigned long BIG_SPOON_SQUARES = 0x2424e70000e72424;

const static unsigned long CORNER_ARRAY[4]{
        CORNER_NW, CORNER_NE, CORNER_SE, CORNER_SW
};

const static unsigned long LITTLE_SPOON_ARRAY[4]{
        LITTLE_SPOON_NW, LITTLE_SPOON_NE, LITTLE_SPOON_SE, LITTLE_SPOON_SW
};

const static unsigned long BIG_SPOON_ARRAY[4]{
        BIG_SPOON_NW, BIG_SPOON_NE, BIG_SPOON_SE, BIG_SPOON_SW
};

const static unsigned long BORDERS = 0xff818181818181ff;
const static unsigned long E_W_BORDERS = 0x8181818181818181;
const static unsigned long N_S_BORDERS = 0xff000000000000ff;

const static unsigned long U_BORDER = 0xff00000000000000;
const static unsigned long D_BORDER = 0x00000000000000ff;
const static unsigned long R_BORDER = 0x0101010101010101;
const static unsigned long L_BORDER = 0x8080808080808080;

const static unsigned long U_L_BORDER = U_BORDER | L_BORDER;
const static unsigned long U_R_BORDER = U_BORDER | R_BORDER;
const static unsigned long D_L_BORDER = D_BORDER | L_BORDER;
const static unsigned long D_R_BORDER = D_BORDER | R_BORDER;

const static unsigned long F_FILE = 0x0404040404040404;
const static unsigned long G_FILE = 0x0202020202020202;
const static unsigned long H_FILE = 0x0101010101010101;
const static unsigned long A8_H1_DIAGONAL = 0x8040201008040201;

const static unsigned long RANKS[64] = {
        0x00000000000000ffULL, 0x00000000000000ffULL, 0x00000000000000ffULL, 0x00000000000000ffULL,
        0x00000000000000ffULL, 0x00000000000000ffULL, 0x00000000000000ffULL, 0x00000000000000ffULL,
        0x000000000000ff00ULL, 0x000000000000ff00ULL, 0x000000000000ff00ULL, 0x000000000000ff00ULL,
        0x000000000000ff00ULL, 0x000000000000ff00ULL, 0x000000000000ff00ULL, 0x000000000000ff00ULL,
        0x0000000000ff0000ULL, 0x0000000000ff0000ULL, 0x0000000000ff0000ULL, 0x0000000000ff0000ULL,
        0x0000000000ff0000ULL, 0x0000000000ff0000ULL, 0x0000000000ff0000ULL, 0x0000000000ff0000ULL,
        0x00000000ff000000ULL, 0x00000000ff000000ULL, 0x00000000ff000000ULL, 0x00000000ff000000ULL,
        0x00000000ff000000ULL, 0x00000000ff000000ULL, 0x00000000ff000000ULL, 0x00000000ff000000ULL,
        0x000000ff00000000ULL, 0x000000ff00000000ULL, 0x000000ff00000000ULL, 0x000000ff00000000ULL,
        0x000000ff00000000ULL, 0x000000ff00000000ULL, 0x000000ff00000000ULL, 0x000000ff00000000ULL,
        0x0000ff0000000000ULL, 0x0000ff0000000000ULL, 0x0000ff0000000000ULL, 0x0000ff0000000000ULL,
        0x0000ff0000000000ULL, 0x0000ff0000000000ULL, 0x0000ff0000000000ULL, 0x0000ff0000000000ULL,
        0x00ff000000000000ULL, 0x00ff000000000000ULL, 0x00ff000000000000ULL, 0x00ff000000000000ULL,
        0x00ff000000000000ULL, 0x00ff000000000000ULL, 0x00ff000000000000ULL, 0x00ff000000000000ULL,
        0xff00000000000000ULL, 0xff00000000000000ULL, 0xff00000000000000ULL, 0xff00000000000000ULL,
        0xff00000000000000ULL, 0xff00000000000000ULL, 0xff00000000000000ULL, 0xff00000000000000ULL,
};

const static unsigned long FILES[64] = {
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
        0x0101010101010101ULL, 0x0202020202020202ULL, 0x040404040404040ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL,
};

const static unsigned long DIAGONALS[64] = {
        0x0000000000000001ULL, 0x0000000000000102ULL, 0x0000000000010204ULL, 0x0000000001020408ULL,
        0x0000000102040810ULL, 0x0000010204081020ULL, 0x0001020408102040ULL, 0x0102040810204080ULL,
        0x0000000000000102ULL, 0x0000000000010204ULL, 0x0000000001020408ULL, 0x0000000102040810ULL,
        0x0000010204081020ULL, 0x0001020408102040ULL, 0x0102040810204080ULL, 0x0204081020408000ULL,
        0x0000000000010204ULL, 0x0000000001020408ULL, 0x0000000102040810ULL, 0x0000010204081020ULL,
        0x0001020408102040ULL, 0x0102040810204080ULL, 0x0204081020408000ULL, 0x0408102040800000ULL,
        0x0000000001020408ULL, 0x0000000102040810ULL, 0x0000010204081020ULL, 0x0001020408102040ULL,
        0x0102040810204080ULL, 0x0204081020408000ULL, 0x0408102040800000ULL, 0x0810204080000000ULL,
        0x0000000102040810ULL, 0x0000010204081020ULL, 0x0001020408102040ULL, 0x0102040810204080ULL,
        0x0204081020408000ULL, 0x0408102040800000ULL, 0x0810204080000000ULL, 0x1020408000000000ULL,
        0x0000010204081020ULL, 0x0001020408102040ULL, 0x0102040810204080ULL, 0x0204081020408000ULL,
        0x0408102040800000ULL, 0x0810204080000000ULL, 0x1020408000000000ULL, 0x2040800000000000ULL,
        0x0001020408102040ULL, 0x0102040810204080ULL, 0x0204081020408000ULL, 0x0408102040800000ULL,
        0x0810204080000000ULL, 0x1020408000000000ULL, 0x2040800000000000ULL, 0x4080000000000000ULL,
        0x0102040810204080ULL, 0x0204081020408000ULL, 0x0408102040800000ULL, 0x0810204080000000ULL,
        0x1020408000000000ULL, 0x2040800000000000ULL, 0x4080000000000000ULL, 0x8000000000000000ULL
};
const static unsigned long ANTI_DIAGONALS[64] = {
        0x8040201008040201ULL, 0x0080402010080402ULL, 0x0000804020100804ULL, 0x0000008040201008ULL,
        0x0000000080402010ULL, 0x0000000000804020ULL, 0x0000000000008040ULL, 0x0000000000000080ULL,
        0x4020100804020100ULL, 0x8040201008040201ULL, 0x0080402010080402ULL, 0x0000804020100804ULL,
        0x0000008040201008ULL, 0x0000000080402010ULL, 0x0000000000804020ULL, 0x0000000000008040ULL,
        0x2010080402010000ULL, 0x4020100804020100ULL, 0x8040201008040201ULL, 0x0080402010080402ULL,
        0x0000804020100804ULL, 0x0000008040201008ULL, 0x0000000080402010ULL, 0x0000000000804020ULL,
        0x1008040201000000ULL, 0x2010080402010000ULL, 0x4020100804020100ULL, 0x8040201008040201ULL,
        0x0080402010080402ULL, 0x0000804020100804ULL, 0x0000008040201008ULL, 0x0000000080402010ULL,
        0x0804020100000000ULL, 0x1008040201000000ULL, 0x2010080402010000ULL, 0x4020100804020100ULL,
        0x8040201008040201ULL, 0x0080402010080402ULL, 0x0000804020100804ULL, 0x0000008040201008ULL,
        0x0402010000000000ULL, 0x0804020100000000ULL, 0x1008040201000000ULL, 0x2010080402010000ULL,
        0x4020100804020100ULL, 0x8040201008040201ULL, 0x0080402010080402ULL, 0x0000804020100804ULL,
        0x0201000000000000ULL, 0x0402010000000000ULL, 0x0804020100000000ULL, 0x1008040201000000ULL,
        0x2010080402010000ULL, 0x4020100804020100ULL, 0x8040201008040201ULL, 0x0080402010080402ULL,
        0x0100000000000000ULL, 0x0201000000000000ULL, 0x0402010000000000ULL, 0x0804020100000000ULL,
        0x1008040201000000ULL, 0x2010080402010000ULL, 0x4020100804020100ULL, 0x8040201008040201ULL
};

unsigned long makeLR(unsigned long piece);

unsigned long makeUD(unsigned long piece);

unsigned long makeDLUR(unsigned long piece);

unsigned long makeULDR(unsigned long piece);

extern unsigned long starMask[64];

extern unsigned long starMaskRook[64];
extern unsigned long starMaskBishop[64];
extern unsigned long starLR[64];
extern unsigned long starUD[64];
extern unsigned long starDLUR[64];
extern unsigned long starULDR[64];
extern unsigned long diagonalMask[64];

extern unsigned long antiDiagonalMask[64];
extern unsigned long rankMask[64];
extern unsigned long fileMask[64];
#endif //OTHELLOPROJECT_BITBOARDUTILS_H

void setupStarMask();
