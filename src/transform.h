#ifndef _TRANSFORM_H_DEFINED_
#define _TRANSFORM_H_DEFINED_

class Transform {
    public:
        static double offsetX;
        static double offsetY;
        static double offsetZ;
        //^^The amount any point gets offset by when put into the offset function.
        static void transform(double in[3]);
        static void offset(double in[3]);

};

#endif
