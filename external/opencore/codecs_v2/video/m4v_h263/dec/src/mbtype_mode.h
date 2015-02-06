
const static int MBtype_mode[] =
{
    MODE_INTER,
    MODE_INTER_Q,
    MODE_INTER4V,
    MODE_INTRA,
    MODE_INTRA_Q,
#ifdef PV_ANNEX_IJKT_SUPPORT
    MODE_INTER4V_Q,
#endif
    MODE_SKIPPED
};
#ifdef PV_ANNEX_IJKT_SUPPORT
const static int16 DQ_tab_Annex_T_10[32] = {0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
const static int16 DQ_tab_Annex_T_11[32] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, -5};
const static int16 MQ_chroma_QP_table[32] = {0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 13,
        14, 14, 14, 14, 14, 15, 15, 15, 15, 15
                                            };
#endif

