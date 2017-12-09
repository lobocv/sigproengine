/*! \file domain_conversions.hpp
    \authors  Calvin Lobo
    \date August 2006
	\brief Conversion routines between sample index, time and depth.

	Due to finite antenna separation, conversion from time to depth is non-linear (hyperbolic).
	This library contains common functions to convert between these domains.

	Definitions:

		Zero Time: 		Time at which the Tx fires.
		First Break: 	Time (approximate) at which the Rx receives the first signal from the Tx (travel time through air).
		Tx Time:		Elapsed time since zero time.
		Rx Time:		Elapsed time since first break.
*/

#ifndef __DOMAIN_CONVERSIONS
#define __DOMAIN_CONVERSIONS

#include <math.h>
#include <float.h>

#include "sigpro_constants.h"

namespace DepthTimeConversions {



	/**	\brief Convert a time measured from zero-time to time measured from first break.

	\param	dTxTime_ns			Time measured from zero-time.
	\param	dAntSep_m			The antenna separation in meters. 

	\return Time referenced from first break (Rx time)
	*/
	inline double TxTime2RxTime(double dTxTime_ns, double dAntSep_m)
	{
		//	dAntSep_m / LIGHTSPEED_MPNS is time in air Tx to Rx
		return dTxTime_ns - (dAntSep_m / LIGHTSPEED_MPNS);
	}



	/**	\brief Convert time measured from first break to time measured from zero-time.

	\param	dRxTime_ns			Time measured from first break.
	\param	dAntSep_m			The antenna separation in meters. 

	\return Time measured from zero-time
	*/
	inline double RxTime2TxTime(double dRxTime_ns, double dAntSep_m) {
		//	dAntSep_m / LIGHTSPEED_MPNS is time in air Tx to Rx
		return dRxTime_ns + (dAntSep_m / LIGHTSPEED_MPNS);
	}



	/**	\brief	Conversion of sample index to Rx time. 

	\param	dSampleIdx				Zero-based index of the sample. 
	\param	dAntSep_m				The antenna separation in meters. 
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break time sample cnt (1 based). 

	\return Tx time at the specified sample index.
	*/
	double SampleIdx2TxTime(double dSampleIdx, double dAntSep_m, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		double dRxTime = (dSampleIdx + 1.0 - dFirstBreakSampleNum) * dSampleDeltaT_ns;
		return dRxTime + (dAntSep_m / LIGHTSPEED_MPNS);
	}
	
	/**	\brief	Conversion of Tx time between transmitter and receiver to sample index. 

	\param	TxTime_ns				Tx time.
	\param	dAntSep_m				The antenna separation in meters. 
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break sample cnt (1 based). 

	\return Sample index at the specified Tx time.
	*/
	double TxTime2SampleIdx(double TxTime_ns, double dAntSep_m, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		double dRxTime = TxTime_ns - (dAntSep_m / LIGHTSPEED_MPNS);
		return (dRxTime/dSampleDeltaT_ns) + dFirstBreakSampleNum - 1.0;//=dSampleIdx;
	}


	/**	\brief	Convert depth to Tx time. 

	\param	dDepth_m		Depth in meters. 
	\param	dAntSep_m		The antenna separation in meters.
	\param	dVel_mpns		Radar velocity in m/ns.

	\return Depth at the specified Tx time.
	*/
	double Depth2TxTime(double dDepth_m, double dVel_mpns, double dAntSep_m)
	{
		return 2.0 * sqrt(pow(dDepth_m,2) + pow((dAntSep_m/2),2)) / dVel_mpns; //t=2(d^2+(a/2)^2)^0.5
	}


	/**	\brief	Convert depth to Rx time. 

	\param	dDepth_m		Depth in meters. 
	\param	dAntSep_m		The antenna separation in meters.
	\param	dVel_mpns		Radar velocity in m/ns.

	\return Depth at the specified Rx time.
	*/
	double Depth2RxTime(double dDepth_m, double dVel_mpns, double dAntSep_m)
	{
		double dTxTime_ns = Depth2TxTime(dDepth_m, dVel_mpns, dAntSep_m);
		return TxTime2RxTime(dTxTime_ns, dAntSep_m);
	}


	/**	\brief	Convert depth to sample index. 

	\param	dDepth_m				Flight time from transmitter to receiver in ns.
	\param	dAntSep_m				The antenna separation in meters.
	\param	dVel_mpns				Radar velocity in m/ns.
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break sample cnt (1 based). 

	\return Sample index at the specified depth.
	*/
	double Depth2SampleIdx(double dDepth_m, double dVel_mpns, double dAntSep_m, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		double dTxTime = Depth2TxTime(dDepth_m, dVel_mpns, dAntSep_m);
		double dRxTime = dTxTime - (dAntSep_m / LIGHTSPEED_MPNS);
		return (dFirstBreakSampleNum - 1.0) + (dRxTime / dSampleDeltaT_ns);
	}
	

	/**	\brief	Convert flight time to depth. 

	\param	dTxTime_ns			Tx-referenced time.
	\param	dAntSep_m			The antenna separation in meters. 
	\param	dVel_mpns			Radar velocity in m/ns.

	\return Depth at the specified Tx time.
	*/
	double TxTime2Depth(double dTxTime_ns, double dVel_mpns, double dAntSep_m)
	{
		int neg = 0;
		double retval, dAntPowCmp, dHypotomsPowCmp;
	
		// if(0.0 > dTxTime_ns) //-ve flight times are not solvable (duh)
		//	return -DBL_MAX; //most negative

		// DA 2008-09-29 change to return meaningful negative data for negative flight times.
		if (dTxTime_ns < 0.0) {
			neg = 1;
			dTxTime_ns = -dTxTime_ns;
		}

		if (dAntSep_m / dVel_mpns >= dTxTime_ns)
			return 0.0;
			
		dAntPowCmp = pow((dAntSep_m / 2.0), 2); //(a/2)^2
		dHypotomsPowCmp = pow((dVel_mpns * dTxTime_ns / 2.0), 2); //(vt/2)^2
		if(dAntPowCmp > dHypotomsPowCmp)
			return -DBL_MAX; //most negative
		else {
			retval = sqrt(dHypotomsPowCmp - dAntPowCmp); //d^2=(vt/2)^2-(a/2)^2
			if (neg)
				return -retval;
			else
				return retval;
		}
	}



	/**	\brief	Convert flight time to depth. 

	\param	dRxTime_ns			Rx-referenced time.
	\param	dAntSep_m			The antenna separation in meters. 
	\param	dVel_mpns			Radar velocity in m/ns.

	\return Depth at the specified Rx time.
	*/
	double RxTime2Depth(double dRxTime_ns, double dVel_mpns, double dAntSep_m){
		double dTxTime_ns = RxTime2TxTime(dRxTime_ns, dAntSep_m);
		return TxTime2Depth(dTxTime_ns, dVel_mpns, dAntSep_m);

	}

	/**	\brief	Conversion of sample index to depth. 

	\param	dSampleIdx				Zero-based index of the sample. 
	\param	dVel_mpns				Radar velocity in m/ns.
	\param	dAntSep_m				The antenna separation in meters. 
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break sample cnt (1 based). 

	\return Sample index at the specified depth.
	*/
	double SampleIdx2Depth(double dSampleIdx, double dVel_mpns, double dAntSep_m, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		double ret;
	
		double dTxTime_ns = SampleIdx2TxTime(dSampleIdx, dAntSep_m, dSampleDeltaT_ns, dFirstBreakSampleNum);
		ret = TxTime2Depth(dTxTime_ns, dVel_mpns, dAntSep_m);
		if(0 < dTxTime_ns)
			return ret;
		else {
			if(ret<0)
				return ret;
			return -ret;
		}
	}


	/**	\brief Get the sample index (zero based) of a given time in ns measured from first break.

	\param	dRxTime_ns				Time offset from first break.
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break sample cnt (1 based). 

	\return Zero based sample index that occured at time (measured from first break) in ns
	*/
	inline double RxTime2SampleIdx(double dRxTime_ns, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		return dFirstBreakSampleNum - 1.0 + (dRxTime_ns / dSampleDeltaT_ns);	 
	}


	/**	\brief Get the time in ns (measured from first break) of a given sample index (zero based).

	\param	dSampleIdx				The zero based sample index of signal
	\param	dSampleDeltaT_ns		The sample delta t ns. 
	\param	dFirstBreakSampleNum	The first break sample cnt (1 based).

	\return Time of sample in ns (measured from first break) Can be a negative value.
	*/
	inline double SampleIdx2RxTime(double dSampleIdx, double dSampleDeltaT_ns, double dFirstBreakSampleNum)
	{
		return (dSampleIdx + 1.0 - dFirstBreakSampleNum) * dSampleDeltaT_ns;
	}


}

#endif