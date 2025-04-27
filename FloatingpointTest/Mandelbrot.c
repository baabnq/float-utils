#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "Float.h"






int main()
{
    float16 zero = value2fp(0);

    
    const int iXmax = 800;
    const int iYmax = 800;
        
    /*
    const double CxMin=-2;
    const double CxMax= 1;
    const double CyMin=-2.0;
    const double CyMax= 2.0;
    */
    
    float16 CxMin = value2fp(2) | SIGN_MASK;
    float16 CxMax = value2fp(1)            ;
    float16 CyMin = value2fp(2) | SIGN_MASK;
    float16 CyMax = value2fp(2)            ;
    
    /*
    double PixelWidth  = (CxMax-CxMin) / iXmax;
    double PixelHeight = (CyMax-CyMin) / iYmax;
    */
    
    float16 PixelWidth  = div(add(CxMax, CxMin ^ SIGN_MASK), value2fp(iXmax));
    float16 PixelHeight = div(add(CyMax, CyMin ^ SIGN_MASK), value2fp(iYmax));

    const int maxIter=40;

    FILE* fp = fopen("new.ppm","wb");
    fprintf(fp,"P6\n %d\n %d\n 255\n", iXmax, iYmax);

    for(int iY=0; iY<iYmax; iY++)
    for(int iX=0; iX<iXmax; iX++)
    {   
        /*
        double Cx=CxMin + iX*PixelWidth;
        double Cy=CyMin + iY*PixelHeight;
        */
        
        float16 Cx = add(CxMin, mul(PixelWidth,  value2fp(iX)));
        float16 Cy = add(CyMin, mul(PixelHeight, value2fp(iY)));
        
        /*
        double Zx=0.0;
        double Zy=0.0;
        double Zx2=Zx*Zx;
        double Zy2=Zy*Zy;
        */
        
        float16 Zx  = zero;
        float16 Zy  = zero;
        float16 Zx2 = zero;
        float16 Zy2 = zero;
        

        int iter = 0;
        //for (; iter < maxIter && ((Zx2+Zy2) < 4.0); iter++)
        for (; iter < maxIter && comp(value2fp(4), add(Zx2, Zy2)); iter++)
        {
            /*
            Zy=2*Zx*Zy + Cy;
            Zx=Zx2-Zy2 + Cx;
            Zx2=Zx*Zx;
            Zy2=Zy*Zy;
            */
            
            Zy = add(mul(mul(Zx, Zy), value2fp(2)), Cy);
            Zx = add(add(Zx2, Zy2 ^ SIGN_MASK), Cx);
            Zx2 = mul(Zx, Zx);
            Zy2 = mul(Zy, Zy);

            
        };
                
        double gray = ((double)iter / maxIter) * 255.0;

        static unsigned char colors[3];
        colors[0]=(int)gray;
        colors[1]=(int)gray;
        colors[2]=(int)gray;

        fwrite(colors, 1, 3, fp);
    }
    fclose(fp);
    return 0;
}