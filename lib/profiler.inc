/*
    Copyright 2010 by Mathias Mamsch
    This file is part of the DOORS Standard Library 

    The DOORS Standard Library  is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DOORS Standard Library  is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the DOORS Standard Library.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
\file profiler.inc
\brief Profiling functions for speed measurement

This file contains functions that will let you measure the excution speed of your code. To measure the execution speed 
of the dxl code the \em getTickCount_ function is used which returns the amount of milliseconds that have passed since 
the system booted. 

There are three problems with profiling. To have a good estimate for the executions speed, you need to call your 
function multiple times. The first problem is that if you don't know in advance how long your function will take to 
execute you don't know how often to call your functions for measurement. The second problem is that especially for 
functions with a high execution speed the call of the profile function itself will be an overhead that distorts the 
measurement. The third problem is, that execution speed is dependent on the hardware so to make the results comparable
one needs to find a measurement result, that is not dependent on the hardware. 

The profiler.inc tries to solve these problems as followed: 
- it will determine how often a function must be called to get an execution time of more than 500ms. After it determined
the repeat count it will do a configurable amount of measurements and determine the average execution speed of the function.
- it will measure the speed for an empty function call on the current platform. This is used to: 
    - determine the overhead of the profile function call and subtract that from the result
    - get an hardware independent measurement result by relating the execution speed to the speed of an empty function call.
      This way the execution speed will be shown in EFC (empty function call multiples). 
- to get a more exact measurement the profile function will let you define your own base function. For example if the function 
  you are profiling does some data setup before calling the function under test, you can specify an empty function with only the
  data setup as a base function. This way the overhead is measured better. 
  
*/

//! \brief Contains the time that is needed for an empty function call
real grProfiler_EmptyFunctionCallTime = 0.0;


int profile_findFactor ( void fn() ) {
    int count = 1; 
    int n1,n2
    
    n1 = getTickCount_()
    fn()
    n2 = getTickCount_() 
    
    while (true) {
             
        if      (n2-n1 ==  0) count = count * 20
        else if (n2-n1 <= 10) count = count * 10 
        else if (n2-n1 <= 50) count = count *  3 
        else                  count = count *  2
        
        n1 = getTickCount_()
        for i in 0:count do fn()
        n2 = getTickCount_() 
        
        if (n2-n1 > 100) break
    } 
    return count * 5
}

string profiler_formatReal (real n, int comma) {
    real fac = pow(10.0,realOf comma)
    return ((realOf intOf (n * fac)) / fac) ""
}

string formatMSTime (real mstime) {
    real EFCs = (grProfiler_EmptyFunctionCallTime == 0.0) ? 0.0 : (mstime / grProfiler_EmptyFunctionCallTime)
    string EFCString = (EFCs == 0.0) ? "" : " (= " profiler_formatReal(EFCs,3) " EFC)"
    if (mstime < 1E-3)                   return profiler_formatReal(mstime*1000000.0,3) " ns" EFCString
    if (mstime > 1E-3 && mstime <= 1E-0) return profiler_formatReal(mstime*1000.0,3   ) " us" EFCString
    if (mstime > 1E0  && mstime <= 1E3 ) return profiler_formatReal(mstime,3)           " ms" EFCString
    return  (profiler_formatReal(mstime / 1000.0, 3)) " s" EFCString
}

real profile ( string name, int factor, int runs, void fn(), real base, bool bPrint) 
{
    int i 
    int j 
    int sum = 0

    for j in 1:runs do {
        int nr1 = getTickCount_ ()
        for i in 0:factor do fn();
        int nr2 = getTickCount_ ()
        sum += nr2-nr1    
        if ((nr2-nr1) < 100) {print "You need to set your factor higher for function " name "! Runtime is to small to measure!"; halt }
    }
    
    real timePerRun = (realOf sum) / ( realOf runs) / (realOf factor) - base
    
    if (bPrint) print "Name: " name " - Average run:" (formatMSTime timePerRun) "\n"
    return timePerRun
}

real profile ( string name, int factor, int runs, void fn(), real base) { return profile (name,factor,runs,fn, base, false) }
real profile ( string name, int factor, int runs, void fn() )           { return profile (name,factor,runs,fn, 0.0 , false) }


/*! \brief Measures the execution time of a function \em fn and returns (and/or) prints the results
    \return The estimated time in [ms] for one call of the function \a fn()
    \param name   The name of the profile run
    \param avg    The number of times that the measurement will be repeated (results will be averaged)
    \param fn()   The function under test (FUT) 
    \param base   The overhead time to subtract (default: empty function call time)
    \param bPrint \em true if the results should be printed to the standard output
*/
real profile (string name, int avg, void fn(), real base, bool bPrint) {
    int fac = profile_findFactor (fn)
    return profile ( name, fac, avg, fn, base, bPrint ) 
}

void profile_empty_func() { }

/*! \brief Same as above but bPrint defaults to \em true.
*/
real profile (string name, int avg, void fn(), real base)      { return profile (name, avg, fn, base, true) }

/*! \brief Same as above but \em base defaults to \em 1 EFC (empty function call).
*/
real profile (string name, int avg, void fn(), bool bPrint) { return profile (name, avg, fn, grProfiler_EmptyFunctionCallTime,  bPrint) }

/*! \brief Same as above but \em base defaults to 1 EFC (empty function call) and \em bPrint defaults to \em true.
*/
real profile (string name, int avg, void fn()) { return profile (name, avg, fn, grProfiler_EmptyFunctionCallTime, true) }

// initialize the empty function call time
grProfiler_EmptyFunctionCallTime = profile ( "Base Run", (profile_findFactor profile_empty_func), 1, profile_empty_func, 0.0, true)




