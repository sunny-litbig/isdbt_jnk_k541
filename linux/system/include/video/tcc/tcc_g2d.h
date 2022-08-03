/************************************************************************
*    Telechips Graphic Engine header file
*    ------------------------------------------------
*
*    FUNCTION    : Graphic Engine drvier
*    MODEL       :
*    CPU NAME    : TCC897X
*
*    START DATE    : 2014. 9. 16.
*    MODIFY DATE :
*    DEVISION    : DEPT. SYSTEM 3 TEAM
*                : TELECHIPS, INC.
************************************************************************/
#ifndef __GRE2D_H__
#define __GRE2D_H__

#include "autoconf.h"

#ifdef CONFIG_ARCH_TCC897X
#include <mach/bsp.h>
#else
#endif

#include "tcc_Gre2d_type.h"

/*------------------------------------------------------------------
Gre2d_SetFCh_address
 graphic engine Front End channel address 0,1,2 setting
-------------------------------------------------------------------*/
extern void Gre2d_SetFCh_address(void *virt_addr, G2D_CHANNEL ch, unsigned int add0, unsigned int add1, unsigned int add2);



/*------------------------------------------------------------------
Gre2d_SetFCh_position
 graphic engine Front channel position settig
 
 frameps_x, frameps_y : frame pixsel size
 poffset_x, poffset_y : pixsel offset
 imageps_x, imageps_y : imagme pixel size
 winps_x, winps_y : window pixsel offset
-------------------------------------------------------------------*/
extern void Gre2d_SetFCh_position(void *virt_addr, G2D_CHANNEL ch, unsigned int frameps_x, unsigned int frameps_y, 
                                unsigned int poffset_x, unsigned int poffset_y, unsigned int imageps_x, unsigned int imageps_y, unsigned int winps_x, unsigned int winps_y );



/*------------------------------------------------------------------
Gre2d_SetFCh_control
 graphic engine Front channel control setting
 mode : flip, rotate
 data_form : rgb, yuv, alpha-rgb
-------------------------------------------------------------------*/
extern void Gre2d_SetFCh_control(void *virt_addr, G2D_CHANNEL ch, G2D_MABC_TYPE MABC, unsigned char LUTE, unsigned char SSUV, G2D_OP_MODE mode, G2D_ZF_TYPE ZF, G2D_FMT_CTRL data_form);


/*------------------------------------------------------------------
Gre2d_SetFCh_Chroma_key
 graphic engine Front channel chroma key Set
 
-------------------------------------------------------------------*/
extern void Gre2d_SetFCh_Chroma_key(void *virt_addr, G2D_CHANNEL ch, unsigned char RY, unsigned char GU, unsigned char BV);



/*------------------------------------------------------------------
Gre2d_SetFCh_Arithmetic_par
 graphic engine Front channel Arithmetic parameter setting
-------------------------------------------------------------------*/
extern void Gre2d_SetFCh_Arithmetic_par(void *virt_addr, G2D_CHANNEL ch, unsigned char RY, unsigned char GU, unsigned char BV);



/*------------------------------------------------------------------
Gre2d_src_ctrl
 graphic engine sorce control
-------------------------------------------------------------------*/
extern void Gre2d_src_ctrl(void *virt_addr, G2D_SRC_CTRL reg);



/*-------- Source Operator pattern set -------*/


/*------------------------------------------------------------------
Gre2d_operator_set
 graphic engine operator 0, 1 setting 
-------------------------------------------------------------------*/
extern void Gre2d_operator_set(void *virt_addr, G2D_OP_TYPE op_set, unsigned short alpha , unsigned char RY, unsigned char GU, unsigned char BV);




/*------------------------------------------------------------------
Gre2d_operator_ctrl
 graphic engine operator control register setting
-------------------------------------------------------------------*/
extern void Gre2d_operator_ctrl(void *virt_addr, G2D_OP_TYPE op_set, G2D_OP_ACON ACON1, G2D_OP_ACON ACON0, G2D_OP_CCON CCON1, G2D_OP_CCON CCON0, G2D_OP_ATUNE ATUNE, G2D_OP1_CHROMA CSEL,GE_ROP_TYPE op );


/*-------- BACK END CHANNEL DESTINATION SETTIG. -------*/

/*------------------------------------------------------------------
Gre2d_SetBCh_address
  graphic engine BACK End channel address 0,1,2 setting
-------------------------------------------------------------------*/
extern void Gre2d_SetBCh_address(void *virt_addr, G2D_CHANNEL ch, unsigned int add0, unsigned int add1, unsigned int add2);



/*------------------------------------------------------------------
Gre2d_SetBCh_position
 graphic engine BACK END channel position settig
 
 frameps_x, frameps_y : frame pixsel size
 poffset_x, poffset_y : pixsel offset
-------------------------------------------------------------------*/
extern void Gre2d_SetBCh_position(void *virt_addr, G2D_CHANNEL ch, unsigned int frameps_x, unsigned intframeps_y, unsigned int poffset_x, unsigned int poffset_y);



/*------------------------------------------------------------------
Gre2d_SetBCh_control
 graphic engine Back END channel control setting
 mode : flip, rotate
 data_form : rgb, yuv, alpha-rgb
-------------------------------------------------------------------*/
extern void Gre2d_SetBCh_control(void *virt_addr, G2D_BCH_CTRL_TYPE *reg);

/*------------------------------------------------------------------
Gre2d_SetDitheringMatrix
 graphic engine Dithering Matix Setting
-------------------------------------------------------------------*/
extern void Gre2d_SetDitheringMatrix(void *virt_addr, unsigned short *Matrix);


/*------------------------------------------------------------------
Gre2d_Grp_enable
 graphic engine channel enable control
-------------------------------------------------------------------*/
extern void Gre2d_Grp_enable(void *virt_addr, G2D_EN grp_enalbe, unsigned char int_en);


/*------------------------------------------------------------------
Gre2d_Grp_check
 graphic engine transger check
-------------------------------------------------------------------*/
extern unsigned int Gre2d_Grp_check(void *virt_addr);


/*------------------------------------------------------------------
Gre2d_Grp_int_ctrl
 graphic engine interrupt control 
 wr : write / read
 int_irq : interrupt request 
 int_flg : flag bit
-------------------------------------------------------------------*/
extern G2D_INT_TYPE Gre2d_Grp_int_ctrl(void *virt_addr, unsigned char wr, G2D_INT_TYPE flag, unsigned char int_irq, unsigned char int_flg);
#endif//__GRE2D_H__






