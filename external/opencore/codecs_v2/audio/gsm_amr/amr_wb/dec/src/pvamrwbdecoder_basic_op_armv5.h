

#ifndef PVAMRWBDECODER_BASIC_OP_ARMV5_H
#define PVAMRWBDECODER_BASIC_OP_ARMV5_H

#ifdef __cplusplus
extern "C"
{
#endif





#if defined(PV_ARM_V5)

    __inline int16 add_int16(int16 var1, int16 var2)
    {
        int32 L_var_out;
        int32 L_var_aux;

        __asm
        {
            mov L_var_out, var1, lsl #16
            mov L_var_aux, var2, lsl #16
            qadd L_var_out, L_var_out, L_var_aux
            mov L_var_out, L_var_out, asr #16

        }
        return L_var_out;
    }


    __inline int16 sub_int16(int16 var1, int16 var2)
    {
        int32 L_var_out;
        int32 L_var_aux;

        __asm
        {
            mov L_var_out, var1, lsl #16
            mov L_var_aux, var2, lsl #16
            qsub L_var_out, L_var_out, L_var_aux
            mov L_var_out, L_var_out, asr #16

        }
        return L_var_out;
    }


    __inline int32 add_int32(int32 L_var1, int32 L_var2)
    {
        int32 L_var_out;

        __asm
        {
            qadd L_var_out, L_var1, L_var2
        }
        return L_var_out;
    }


    __inline int32 mac_16by16_to_int32(int32 L_var3, int16 var1, int16 var2)
    {
        int32 L_var_out;


        __asm
        {
            smulbb L_var_out, var1, var2
            qdadd L_var_out, L_var3, L_var_out
        }
        return L_var_out;
    }

    __inline int32 sub_int32(int32 L_var1, int32 L_var2)
    {
        int32 L_var_out;

        __asm
        {
            qsub L_var_out, L_var1, L_var2
        }
        return L_var_out;
    }

    __inline int32 msu_16by16_from_int32(int32 L_var3, int16 var1, int16 var2)
    {
        int32 L_var_out;


        __asm
        {
            smulbb L_var_out, var1, var2
            qdsub L_var_out, L_var3, L_var_out
        }
        return L_var_out;
    }

    __inline int32 mul_16by16_to_int32(int16 var1, int16 var2)
    {
        int32 L_var_out;

        __asm
        {
            smulbb L_var_out, var1, var2
            qadd L_var_out, L_var_out, L_var_out
        }
        return L_var_out;
    }

    __inline int16 mult_int16(int16 var1, int16 var2)
    {
        int32 L_var_out;

        __asm
        {
            smulbb L_var_out, var1, var2
            mov L_var_out, L_var_out, asr #15
        }
        return L_var_out;
    }


    __inline int16 amr_wb_round(int32 L_var1)
    {
        int32 L_var_out;

        __asm
        {
            qadd L_var_out, L_var1, (int32) 0x00008000L
            mov L_var_out, L_var_out, asr #16
        }
        return L_var_out;
    }



    __inline int16 amr_wb_shl1_round(int32 L_var1)
    {
        int32 L_var_out;

        __asm
        {
            qadd L_var_out, L_var1, L_var1
            qadd L_var_out, L_var_out, (int32) 0x00008000L
            mov L_var_out, L_var_out, asr #16
        }
        return L_var_out;
    }

    __inline int32 mul_32by16(int16 hi, int16 lo, int16 n)
    {
        int32 H_32;
        int32 L_32;
        __asm
        {
            smulbb H_32, hi, n
            smulbb L_32, lo, n
            add H_32, H_32, L_32, asr #15
            qadd H_32, H_32, H_32
        }

        return (H_32);
    }

    __inline  int32 fxp_mac_16by16(const int16 var1, const int16 var2, int32 L_add)
    {
        __asm
        {
            smlabb L_add, var1, var2, L_add
        }
        return (L_add);
    }

    __inline  int32 fxp_mul_16by16(int16 var1, const int16 var2)
    {
        int32 L_mult;
        __asm
        {
            smulbb L_mult, var1, var2
        }
        return (L_mult);
    }

    __inline  int32 fxp_mul32_by_16b(int32 L_var1, const int32 L_var2)
    {
        int32 L_mult;
        __asm
        {
            smulwb L_mult, L_var1, L_var2
        }

        return L_mult;
    }


#endif

#ifdef __cplusplus
}
#endif




#endif   /*  PVAMRWBDECODER_BASIC_OP_ARMV5_H  */

