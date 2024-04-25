/*
 * clarke.h
 *
 *  Created on: Nov 20, 2021
 *      Author: Sunweijun
 */

#ifndef _CLARKE_H_
#define _CLARKE_H_

//! \brief  Contains the public interface to the 
//!         Clarke transform (CLARKE) module routines
//!


// **************************************************************************
// the includes

#include "zxmathfp.h"

#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines



// **************************************************************************
// the typedefs

//! \brief Defines the CLARKE object
//!
typedef struct _CLARKE_Obj_
{
  q15   As;      //!< phase A component
  q15   Bs;      //!< phase B component
  q15   Cs;      //!< phase C component
  q15   alpha;   //!< the alpha component
  q15   beta;    //!< the beta component

} CLARKE_Obj;


// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

extern void CLARKE_run(CLARKE_Obj *obj);


#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of _CLARKE_H_ definition

