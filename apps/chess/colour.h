#ifndef HEISEN_KERNEL_CHESS_GAME_COLOUR_H
#define HEISEN_KERNEL_CHESS_GAME_COLOUR_H


namespace LIBHeisenKernel {
    
    namespace engine {

    // constants

        const int ColourNone = -1;
        const int White = 0;
        const int Black = 1;
        const int ColourNb = 2;

        const int WhiteFlag = 1 << White;
        const int BlackFlag = 1 << Black;

    }  // namespace engine

    // macros

    #define COLOUR_IS_OK(colour)    (((colour)&~1)==0)

    #define COLOUR_IS_WHITE(colour) ((colour)==White)
    #define COLOUR_IS_BLACK(colour) ((colour)!=White)

    #define COLOUR_IS(piece,colour) (FLAG_IS((piece),COLOUR_FLAG(colour)))
    #define FLAG_IS(piece,flag)     (((piece)&(flag))!=0)

    #define COLOUR_OPP(colour)      ((colour)^(White^Black))
    #define COLOUR_FLAG(colour)     ((colour)+1)
}

#endif // HEISEN_KERNEL_CHESS_GAME_COLOUR_H